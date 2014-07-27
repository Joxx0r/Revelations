#ifndef _REV_RENDER_ENVIROMENT_HELPER_H
#define _REV_RENDER_ENVIROMENT_HELPER_H

#include "RevRenderBaseHelper.h"
#include <DirectXMath.h>

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11Texture2D;

class RevTexture;
class RevEffect;

class RevRenderEnviromentHelper : public RevRenderBaseHelper
{
public:
	RevRenderEnviromentHelper(void);
	~RevRenderEnviromentHelper(void);
	void SetEnviromentSize( const int& aEnvMapSize );
	virtual bool Init();
	virtual bool Setup( );

	virtual void Render( RevRenderMesh& aRenderMesh );
	
	ID3D11ShaderResourceView*	GetSRV( );
	void ComputeSHAndSaveToEffect( RevEffect* aEffect );
	void ComputeSHAndSaveToVector( std::vector< unsigned int >* aData, const int& aIndex );
	void ComputeSHIrradEnvMapConstants( float* pSHCoeffsRed, float* pSHCoeffsGreen, float* pSHCoeffsBlue, RevEffect* aEffectToSave );
	void ComputeSHIrradEnvMapConstantsAndSaveToVector( float* pSHCoeffsRed, float* pSHCoeffsGreen, float* pSHCoeffsBlue, std::vector< unsigned int >* aData, const int& aIndex  );
	void SaveBlankSHToVector( std::vector< unsigned int >* aData, const int& aIndex );

	void SetPosition( const CU::Vector3f& aPosition );
	ID3D11Texture2D* GetCubeTexture( );
	unsigned int PackFloat( CU::Vector4f& aVector );
private:
	int myEnvSize;



	ID3D11ShaderResourceView*	mySRV;
	ID3D11RenderTargetView*		myCubeMapRTVS[ 6 ];
	ID3D11DepthStencilView*		myDSVs[6];
	ID3D11Texture2D*			myCubeMapTexture;


	CU::Matrix44f				myViewMatrixes[ 6 ];
	CU::Matrix44f				myProjMatrix;
};

#endif