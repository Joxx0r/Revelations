#include "stdafx.h"
#include "RevRenderNormalHelper.h"
#include "RevEngineMain.h"

RevRenderNormalHelper::RevRenderNormalHelper(void)
{
	myDSV = NULL;
	myRTVs = NULL;
	myNrOfRTVS = 0;
}


RevRenderNormalHelper::~RevRenderNormalHelper(void)
{
}

bool RevRenderNormalHelper::Init()
{
	return true;
}

void RevRenderNormalHelper::Render( RevRenderMesh& aRenderMesh )
{
	RenderInstancedInstances( aRenderMesh );
}

bool RevRenderNormalHelper::Setup( )
{
	RevEngineMain::GetEffectContainer().SetCameraMatrixes( myCameraData );
	RevEngineMain::GetContext()->OMSetRenderTargets( myNrOfRTVS,  &myRTVs, myDSV );
	return true;
}

bool RevRenderNormalHelper::SetupRTVAndDSV(	
		const int& aNrOfRTVs,
		ID3D11RenderTargetView* aRTV,
		ID3D11DepthStencilView* aDSV )
{
	myRTVs		= aRTV;
	myDSV		= aDSV;
	myNrOfRTVS	= aNrOfRTVs;
	return true;
}

bool RevRenderNormalHelper::SetupRenderTech(	const int& aRenderTech )
{
	myEffectTechnique = aRenderTech;
	return true;
}