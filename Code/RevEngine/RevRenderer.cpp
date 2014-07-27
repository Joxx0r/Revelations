#include "StdAfx.h"
#include "RevRenderer.h"
#include "RevScene.h"
#include "RevEngineMain.h"
#include "RevEffect.h"
#include "CU_VectorMath.h"
#include "RevTexture.h"
#include "RevRenderAO.h"
#include "RevRenderFullScreenEffect.h" 

//Tempo
#include "RevInstance.h"
#include "RevModel.h"
#include <bitset>
#include <cstdlib>
#include <time.h>

#include <d3dx11effect.h>
#include <string>
#include <d3dx11.h>
#include <iostream>     // std::cout
#include <fstream>

#include <D3Dcompiler.h>
#include <assert.h>


RevRenderer::RevRenderer(void)
{
	myScene = NULL;
	myComputeNewEnvMapFlag = true;
	myRandomTexture = NULL;
	myTimeTreshold = 0.0f;
	m_aoRender = nullptr;
	myScene = nullptr;
	myRandomTexture = nullptr;

}

RevRenderer::~RevRenderer(void)
{
	delete m_aoRender; m_aoRender = nullptr;
	delete myScene; myScene = nullptr;
	delete myRandomTexture; myRandomTexture = nullptr;
}

bool RevRenderer::Init( )
{
	myScene = new RevScene();
	myScene->Init( );
	myEnviromentHelper.SetEnviromentSize( 512 );
	myEnviromentHelper.Init( );
	myNormalRenderHelper.Init( );
	myFullScreenHelper.Init( );
	myDofManager.Init( RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight(), &myFullScreenHelper );
	myDeferedHelper.InitRTVSAndDSV( RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight() );
	myFullScreenHelper.GetEffect()->SetVector2Varible(RevEffect::_WINDOW_SIZE, CU::Vector2f( RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight() ) );
	myGIRender.SetEnviromentSize( 128 );
	myGIRender.Init( );
	m_lightRenderHelper.Init();
	srand(static_cast<unsigned int>(time(0)));
	m_aoRender = new RevRenderAO(&myFullScreenHelper);
	m_aoRender->Init();
	return true;
}	

bool RevRenderer::Update( const float& aDeltaTime) 
{
	if (!myScene)
	{
		return false;
	}

	myScene->Update( aDeltaTime );
	myTimeTreshold += aDeltaTime;
	return true;
}

bool RevRenderer::Render( )
{	
	OnFrameSetup( 1, RevEngineMain::GetEngineRTV(), RevEngineMain::GetEngineDSV() );
	RevCameraData data  = myScene->GetSceneCameraData();
	myFullScreenHelper.GetEffect()->SetVector4ArrayVarible( RevEffect::_FRUSTUM_CORNERS, &data.myFrustumCorners[0], 4 );

	myScene->Render();
	RenderFrame(_DEFERED, data);
	RenderFrame(_PBL_LIGHTS, data);
	return true;
}

RevScene* RevRenderer::GetRendererScene( )
{
	assert( myScene != NULL );
	return myScene;
}

void RevRenderer::SetScene( RevScene* aScene )
{
	myScene =  aScene;
}

RevRenderTargetCollector& RevRenderer::GetRevRenderTargetCollector()
{
	return myRenderTargetCollector;
}

void RevRenderer::OnAllItemsLoaded( )
{
}

void RevRenderer::RenderFrame(	const int& aRenderTech,
								const RevCameraData& aCameraData )
{
	assert( aRenderTech > -1 && _NUM_OF_FRAME_TECHS > aRenderTech );

	switch( aRenderTech )
	{
	case _NORMAL:
		RenderNormalInstances( aCameraData );
		break;
	case _CUBEMAP:
		RenderCubeMapInstances();
		break;
	case _DEFERED:
		RenderDeferedInstances( aCameraData );
		break;
	case _REFLECTION:
		RenderReflectionInstances( aCameraData );
		break;
	case _POINTLIGHT:
		RenderPointLights( aCameraData );
		break;
	case _PBL_LIGHTS:
		RenderPBLLighs(aCameraData);
		break;
	case _AO:
		RenderAO();
		break;
	default:
		break;
	}

}

void RevRenderer::RenderNormalInstances( const RevCameraData& aCameraData )
{
	myNormalRenderHelper.SetRenderTech( RevEffect::_NORMAL );
	myNormalRenderHelper.SetCameraData( aCameraData );
	myNormalRenderHelper.SetupRTVAndDSV( 1, RevEngineMain::GetEngineRTV(), RevEngineMain::GetEngineDSV() );
	RevEngineMain::GetRenderTargetCollector().Render( &myNormalRenderHelper );
}

void RevRenderer::RenderDeferedInstances(  const RevCameraData& aCameraData )
{
	myDeferedHelper.SetRenderTech( RevEffect::_RENDER_INSTANCED );
	myDeferedHelper.SetCameraData( aCameraData );
	RevEngineMain::GetRenderTargetCollector().Render( &myDeferedHelper );
	RevEngineMain::GetContext()->OMSetRenderTargets(0,0,0);
	myDeferedHelper.SetTexturesToShader( );
}

void RevRenderer::RenderCubeMapInstances( )
{
	ID3D11ShaderResourceView *const pSRV[1] = {NULL};
	RevEngineMain::GetContext()->PSSetShaderResources(0, 1, pSRV);
	myEnviromentHelper.SetPosition( CU::Vector3f(0,0,0) );
	myEnviromentHelper.SetRenderTech( RevEffect::_NORMAL );
	RevEngineMain::GetRenderTargetCollector().Render( &myEnviromentHelper );
	RevEngineMain::GetContext()->OMSetRenderTargets(0,0,0);
	myEnviromentHelper.ComputeSHAndSaveToEffect( myFullScreenHelper.GetEffect() );

	RevEngineMain::GetEffectContainer().SetTexture( "g_CubeMap", myEnviromentHelper.GetSRV() );
}

void RevRenderer::RenderReflectionInstances( const RevCameraData& aCameraData )
{
	for(int i = 0; i < myInstances.size(); i++)
	{
		myInstances[i]->Render( );
	}
	myNormalRenderHelper.SetRenderTech( RevEffect::_RENDER_REFLECTION );
	myNormalRenderHelper.SetCameraData( aCameraData );
	myNormalRenderHelper.SetupRTVAndDSV( 1, RevEngineMain::GetEngineRTV(), RevEngineMain::GetEngineDSV() );
	RevEngineMain::GetRenderTargetCollector().Render( &myNormalRenderHelper );
}

void RevRenderer::RenderPointLights( RevCameraData aCameraData )
{
	ClearColorRTV( myAccuBuffer->GetRTV() );
	ID3D11RenderTargetView* rtv = myAccuBuffer->GetRTV();
	RevEngineMain::GetContext()->OMSetRenderTargets( 1, &rtv, RevEngineMain::GetEngineDSV() );
	RevEngineMain::GetEffectContainer().SetCameraMatrixes( aCameraData );
	
	RevEffect* effect = myPointLightModel->GetEffect();

	for(unsigned int matrixIndex = 0; matrixIndex < myPoses.size(); matrixIndex++)
	{
		RevPointLight& p = myPoses[matrixIndex ];
		CU::Matrix44f wMatrix;
		wMatrix.SetPosition( myPoses[ matrixIndex ].myPos );
	
		CU::Matrix44f scale;
		scale.myMatrix[ 0 ][ 0 ] = myPoses[ matrixIndex ].myRadius;
		scale.myMatrix[ 1 ][ 1 ] = myPoses[ matrixIndex ].myRadius;
		scale.myMatrix[ 2 ][ 2 ] = myPoses[ matrixIndex ].myRadius;
		scale.myMatrix[ 3 ][ 3 ] = 0;
		
		CU::Vector4f viewPos =  CU::Vector4f(myPoses[ matrixIndex ].myPos) * aCameraData.myViewMatrix;
		effect->SetMatrixVarible( RevEffect::_SCALE_MATRIX, scale );
		myPointLightModel->Render( wMatrix, RevEffect::_NORMAL  );
	}
	myPointLightModel->Render( CU::Matrix44f(), RevEffect::_RESET );
	RevEngineMain::GetContext()->OMSetRenderTargets(0,0,0);
	RevEngineMain::GetEffectContainer().SetTexture( "g_AccuTex", myAccuBuffer->GetSRV() );
	
	const float windowWidth = RevEngineMain::GetWindowHeight();
	const float windowHeight = RevEngineMain::GetWindowHeight();

	myFullScreenHelper.SetViewPort(windowWidth / 2, windowHeight / 2, 0, 0 );
	myFullScreenHelper.Process( "Render", RevEngineMain::GetEngineRTV(), RevEngineMain::GetWindowWidth(),  RevEngineMain::GetWindowHeight(), myDeferedHelper.GetDepthTexture()->GetSRV(), RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight());
	
	myFullScreenHelper.SetViewPort(windowWidth / 2, windowHeight / 2, windowWidth / 2, 0 );
	myFullScreenHelper.Process( "RenderNormal", RevEngineMain::GetEngineRTV(), RevEngineMain::GetWindowWidth(),  RevEngineMain::GetWindowHeight(), myDeferedHelper.GetNormalTexture()->GetSRV(), RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight());

	myFullScreenHelper.SetViewPort(windowWidth / 2, windowHeight / 2, 0, windowHeight / 2);
	myFullScreenHelper.Process( "Render", RevEngineMain::GetEngineRTV(), RevEngineMain::GetWindowWidth(),  RevEngineMain::GetWindowHeight(), myAccuBuffer->GetSRV(), RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight());

	myScene->Render( );

	ClearColorRTV( myCompleteBuffer->GetRTV() );
	ClearDepth( RevEngineMain::GetEngineDSV()  );
	myNormalRenderHelper.SetCameraData( aCameraData );
	myNormalRenderHelper.SetRenderTech( RevEffect::_RENDER_LIGHT_RENDERING );
	myNormalRenderHelper.SetupRTVAndDSV( 1, myCompleteBuffer->GetRTV(), RevEngineMain::GetEngineDSV() );
	RevEngineMain::GetRenderTargetCollector().Render( &myNormalRenderHelper );

	myFullScreenHelper.SetViewPort(windowWidth / 2, windowHeight / 2, windowWidth / 2, windowHeight / 2);
	myFullScreenHelper.Process( "Render", RevEngineMain::GetEngineRTV(), RevEngineMain::GetWindowWidth(),  RevEngineMain::GetWindowHeight(), myCompleteBuffer->GetSRV(), RevEngineMain::GetWindowWidth(),RevEngineMain::GetWindowHeight());
	
}

void RevRenderer::RenderPBLLighs(RevCameraData aCameraData)
{
	m_lightRenderHelper.SetCameraData(aCameraData);
	m_lightRenderHelper.SetDepthTexture(RevEngineMain::GetEngineDSV());
	m_lightRenderHelper.Setup();
	m_lightRenderHelper.RenderNoMesh();
	RevEngineMain::ResetOMDS();
}

void RevRenderer::RenderFullScreenEffects(RevCameraData aCameraData)
{
}

void RevRenderer::RenderCompletePass( const RevCameraData& aCameraData )
{

}

void RevRenderer::RenderAO()
{
	RevEngineMain::ResetOMDS();
	m_aoRender->Render(RevEngineMain::GetWindowWidth(), RevEngineMain::GetWindowHeight());
}

void RevRenderer::OnFrameSetup(	const int& aNrOfRenderTargets,
							   ID3D11RenderTargetView* aEngineRTV,
							   ID3D11DepthStencilView* aDSVBuffer )
{
	ClearColorRTV( aEngineRTV );
	ClearDepth( aDSVBuffer );
}

void RevRenderer::ClearColorRTV( ID3D11RenderTargetView* aRTV )
{
	RevEngineMain::GetContext()->OMSetRenderTargets(0,0,0);
	float color[] = { 0,0,0,1 };
	RevEngineMain::GetContext()->ClearRenderTargetView( aRTV, color );
}

void RevRenderer::ClearDepth( ID3D11DepthStencilView* aDSVBuffer )
{
	RevEngineMain::GetContext()->ClearDepthStencilView( aDSVBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f );
}
