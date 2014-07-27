#include "stdafx.h"
#include "RevReflectionCubeMapStructure.h"
#include "RevRoom.h"
#include "RevEngineMain.h"
#include "RevEffect.h"

RevReflectionCubeMapStructure::RevReflectionCubeMapStructure()
{
	myRoom = NULL;
}

RevReflectionCubeMapStructure::~RevReflectionCubeMapStructure()
{

}

void RevReflectionCubeMapStructure::Init(	RevRoom* aRoom, 
										 const int& aReflectionSize )
{
	myRoom = aRoom;
	myRenderHelper.SetEnviromentSize( aReflectionSize );
	myRenderHelper.Init( );
	myRenderHelper.SetPosition( aRoom->GetCenterPos() );
	myRenderHelper.SetRenderTech( RevEffect::_NORMAL );
}

CU::Vector3f RevReflectionCubeMapStructure::GetBiggestPoint()
{
	return myRoom->GetBiggestPoint();
}

CU::Vector3f RevReflectionCubeMapStructure::GetSmallestPoint()
{
	return myRoom->GetSmallestPoint();
}


CU::Vector3f RevReflectionCubeMapStructure::GetHalfExtents()
{
	return myRoom->GetHalfExtents();
}

RevRenderEnviromentHelper& RevReflectionCubeMapStructure::GetRenderHelper()
{
	return myRenderHelper;
}

void RevReflectionCubeMapStructure::Activate( )
{
	RevEngineMain::GetEffectContainer().SetVector3OnAllEffects( RevEffect::_ENV_BOX_MAX, GetBiggestPoint() );
	RevEngineMain::GetEffectContainer().SetVector3OnAllEffects( RevEffect::_ENV_BOX_MIN, GetSmallestPoint() );
	RevEngineMain::GetEffectContainer().SetVector3OnAllEffects( RevEffect::_ENV_BOX_POS, myRoom->GetCenterPos() );

	RevEngineMain::GetEffectContainer().SetTexture( "g_RefCubeMap", myRenderHelper.GetSRV() );

}