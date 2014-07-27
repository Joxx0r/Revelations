#ifndef _REV_EFFECT_CONTAINER_H
#define _REV_EFFECT_CONTAINER_H

#include "CU_Matrix.h"
#include <vector>

class RevEffect;
struct RevCameraData;
struct ID3D11ShaderResourceView;

struct RevHolderStructure
{
	RevHolderStructure( const char* aFilePath, 
						RevEffect* aEffect ) 
	:myEffect( aEffect ),
	myFilePath( aFilePath)
	{
	}

	const char* myFilePath;
	RevEffect*	myEffect;
};

class RevEffectContainer
{
public:
	RevEffectContainer(void);
	~RevEffectContainer(void);

	RevEffect* GetEffect( const char* aFilePath, bool aIsFullScreenShaderFlag=false );

	void SetCameraMatrixes( const RevCameraData& aData );
	void SetTexture(	const char* aResourceName, 
						ID3D11ShaderResourceView* aRSV );

	void SetTextureOnFullScreenShaders( const char* aResourceName, 
								ID3D11ShaderResourceView* aRSV );
	void SetFloatOnAllEffects( const int& aVaribleID,
								const float& aScalar,
								bool aOnlyOnFullScreenFlag = false);

	void SetVector3OnAllEffects(	const int& aVectorIndex,
									const CU::Vector3f& aPos );
	
private:
	bool GetEffectFromContainer(	RevEffect** aEffect,
									const char* aFilePath );
	bool AttemptToAllocateMemoryFromEffect( RevEffect** aEffect,
											const char* aFilePath );
	void SetMatrixOnAllEffects( const int& aVaribleID,
								CU::Matrix44f& aMatrix );



	std::vector< RevHolderStructure >  myEffects;
	std::vector< RevHolderStructure >  myFullScreenShaders;
};

#endif