#include "stdafx.h"
#include "RevDofManager.h"
#include "RevTexture.h"
#include "RevEngineMain.h"
#include "RevEffect.h"

RevDofManager::RevDofManager(void)
{
	myDownSample2x2 = NULL;
	myDownSample4x4 = NULL;
	myDownSample16x16 = NULL;

	myHelper = NULL;
}


RevDofManager::~RevDofManager(void)
{
	delete myDownSample2x2;
	delete myDownSample4x4;
	delete myDownSample16x16;
	delete myDofCompute;
}

void RevDofManager::Init( const int& aWidth, const int& aHeight, RevFullScreenHelper* aHelper )
{

	myDofCompute = new RevTexture(
			aWidth, 
			aHeight, 
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
			true,
			true,
			false);

	myDownSample2x2 = new RevTexture( 
			aWidth / 2, 
			aHeight / 2, 
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
			true,
			true,
			false);

	myDownSample4x4 = new RevTexture( 
			aWidth / 4, 
			aHeight / 4, 
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
			true,
			true,
			false);

	myDownSample16x16 = new RevTexture( 
			aWidth / 16, 
			aHeight / 16, 
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
			true,
			true,
			false);

	myDownSample8x8 = new RevTexture( 
			aWidth / 8, 
			aHeight / 8, 
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, 
			true,
			true,
			false);


	myHelper = aHelper;

	myWidth		= aWidth;
	myHeight	= aHeight;

}

void RevDofManager::Process( ID3D11ShaderResourceView* aSource, ID3D11RenderTargetView* aTarget )
{
	assert( myHelper != NULL );
	myHelper->Process( "DepthOfFieldComputePass", myDofCompute->GetRTV(), myWidth ,myHeight, aSource, myWidth, myHeight );
	myHelper->Process( "DownSample2x2", myDownSample2x2->GetRTV(), myWidth / 2, myHeight / 2, myDofCompute->GetSRV(), myWidth, myHeight );
	myHelper->Process( "DownSample2x2", myDownSample4x4->GetRTV(), myWidth / 4, myHeight / 4, myDownSample2x2->GetSRV(), myWidth / 2, myHeight / 2 );
	myHelper->Process( "DownSample2x2", myDownSample8x8->GetRTV(), myWidth / 8, myHeight / 8, myDownSample4x4->GetSRV(), myWidth / 4, myHeight / 4 );
	myHelper->Process( "DownSample2x2", myDownSample16x16->GetRTV(), myWidth / 16, myHeight / 16, myDownSample8x8->GetSRV(), myWidth / 8, myHeight / 8 );
	RevEngineMain::GetContext()->OMSetRenderTargets(0,0,0);

	RevEffect* fullScreenEffect = myHelper->GetEffect();

	fullScreenEffect->SetTextureVarible( "g_DepthOfFieldTex", myDofCompute->GetSRV() );
	fullScreenEffect->SetTextureVarible( "g_DepthOfFieldTexDownSample", myDownSample16x16->GetSRV() );
	fullScreenEffect->SetVector4Varible(  RevEffect::_PIXEL_SIZE, CU::Vector4f( 1 / myWidth, 1 / myHeight, 0,0 ) );
	fullScreenEffect->SetVector4Varible(  RevEffect::_PIXEL_DOWN_SAMPLE_SIZE, CU::Vector4f( 1 / (myWidth / 16), 1 / (myHeight / 16), 0,0 ) );
	fullScreenEffect->SetVector4Varible( RevEffect::_DOF_PARAMS, CU::Vector4f( 0.0f, 3.0f, 25.0f, 0.5f ) );

	myHelper->Process("DepthOfFieldComputePass", aTarget, myWidth, myHeight, NULL, 0,0 );


}
