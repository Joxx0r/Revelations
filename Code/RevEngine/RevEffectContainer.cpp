#include "StdAfx.h"
#include "RevEffectContainer.h"
#include "RevEffect.h"
#include "RevStructures.h"

RevEffectContainer::RevEffectContainer(void)
{

}

RevEffectContainer::~RevEffectContainer(void)
{
	myFullScreenShaders.clear();
	for(unsigned int i = 0; i < myEffects.size(); i++)
	{
		delete myEffects[i].myEffect;
	}
	myEffects.clear();
}

RevEffect* RevEffectContainer::GetEffect( const char* aFilePath, bool aIsFullScreenShaderFlag )
{
	RevEffect* rEffect = NULL;
	if( GetEffectFromContainer( &rEffect, aFilePath ) == false )
	{
		AttemptToAllocateMemoryFromEffect( &rEffect, aFilePath );
		if( rEffect != NULL && aIsFullScreenShaderFlag == true )
		{
			RevHolderStructure holder( aFilePath, rEffect );
			myFullScreenShaders.push_back( holder );
		}
	}

	return rEffect;
}

void RevEffectContainer::SetCameraMatrixes( const RevCameraData& aData )
{
	CU::Matrix44f view = aData.myViewMatrix;
	CU::Matrix44f proj = aData.myProjectionMatrix;

	SetFloatOnAllEffects( RevEffect::_Z_FAR_VARIBLE, aData.myZFar );
	SetFloatOnAllEffects( RevEffect::_Z_NEAR_VARIBLE, aData.myZNear );


	SetMatrixOnAllEffects( RevEffect::_VIEW, view );
	SetMatrixOnAllEffects( RevEffect::_PROJ, proj );
	SetMatrixOnAllEffects( RevEffect::_VIEW_PROJ, view * proj );

	SetVector3OnAllEffects( RevEffect::_CAMERA_POS, view.GetPosition3() );

	SetMatrixOnAllEffects( RevEffect::_INVERSE_VIEW, view.Real4X4Inverse() );
	SetMatrixOnAllEffects( RevEffect::_INVERSE_VIEW_PROJ, (view * proj).Real4X4Inverse() );
}

void RevEffectContainer::SetTexture(	
	const char* aResourceName, 
	ID3D11ShaderResourceView* aRSV )
{
	for(int effectIndex = 0; effectIndex < myEffects.size(); effectIndex++)
	{
		myEffects[ effectIndex ].myEffect->SetTextureVarible( aResourceName,  aRSV );
	}
}


void RevEffectContainer::SetFloatOnAllEffects( const int& aVaribleID,
												const float& aScalar,
												bool aOnlyOnFullScreenFlag )
{
	assert( -1 < aVaribleID &&  aVaribleID < RevEffect::_NUM_OF_FLOAT_VARIABLES );
	if( aOnlyOnFullScreenFlag == false )
	{
		for(int effectIndex = 0; effectIndex < myEffects.size(); effectIndex++)
		{
			myEffects[ effectIndex ].myEffect->SetFloatVarible( aVaribleID, aScalar );
		}
	}
	else
	{
		for(int effectIndex = 0; effectIndex < myFullScreenShaders.size(); effectIndex++)
		{
			myFullScreenShaders[ effectIndex ].myEffect->SetFloatVarible( aVaribleID, aScalar );
		}
	}
}

void RevEffectContainer::SetVector3OnAllEffects(	const int& aVectorIndex,
									const CU::Vector3f& aPos )
{
	assert( -1 < aVectorIndex &&  aVectorIndex < RevEffect::_NUM_OF_VECTOR_VARIABLES );
	for(int effectIndex = 0; effectIndex < myEffects.size(); effectIndex++)
	{
		myEffects[ effectIndex ].myEffect->SetVector3Varible( aVectorIndex, aPos );
	}
}


bool RevEffectContainer::GetEffectFromContainer(	RevEffect** aEffect,
												const char* aFilePath )
{
	for(unsigned int effectIndex = 0; effectIndex < myEffects.size(); effectIndex++)
	{
		if( myEffects[ effectIndex ].myFilePath == aFilePath )
		{
			*aEffect = myEffects[ effectIndex ].myEffect;
			return true;
		}
	}
	return false;
}	

bool RevEffectContainer::AttemptToAllocateMemoryFromEffect( 
	RevEffect** aEffect,
	const char* aFilePath )
{
	RevEffect* effect = new RevEffect( );
	if( effect->Init( aFilePath ) == false )
	{
		delete effect; effect = NULL;
		return false;
	}
	RevHolderStructure holder( aFilePath, effect );
	myEffects.push_back( holder );
	*aEffect = effect;
	return true;
}

void RevEffectContainer::SetMatrixOnAllEffects( 
	const int& aVaribleID,
	CU::Matrix44f& aMatrix )
{
	assert( -1 < aVaribleID &&  aVaribleID < RevEffect::_NUM_OF_MATIRX_VARIABLES );
	for(int matrixIndex = 0; matrixIndex < myEffects.size(); matrixIndex++)
	{
		myEffects[ matrixIndex ].myEffect->SetMatrixVarible( aVaribleID, aMatrix );
	}
}

