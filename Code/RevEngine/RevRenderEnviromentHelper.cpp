#include "stdafx.h"
#include "RevRenderEnviromentHelper.h"
#include "RevEngineMain.h"
#include "RevRenderMesh.h"
#include "RevModel.h"
#include "RevEffect.h"
#include <DirectXMath.h>
#include <d3dx9math.h>
#include <assert.h>
#include <math.h>
#include <d3dx11.h>

RevRenderEnviromentHelper::RevRenderEnviromentHelper(void)
{
	myEnvSize = 0;
	for(int i = 0; i < 6; i++) { myCubeMapRTVS[ i ] = NULL; }
	for(int i = 0; i < 6; i++) { myDSVs[ i ] = NULL; }
	myCubeMapTexture = NULL;
	mySRV = NULL;;

}


RevRenderEnviromentHelper::~RevRenderEnviromentHelper(void)
{
	for(int i = 0; i < 6; i++) { if( myCubeMapRTVS[ i ] != NULL )	 myCubeMapRTVS[ i ]->Release();  }
	for(int i = 0; i < 6; i++) { if( myDSVs[ i ] != NULL )	 myDSVs[ i ]->Release();  }
	if( mySRV != NULL ) mySRV->Release();
}

void RevRenderEnviromentHelper::SetEnviromentSize( const int& aEnvMapSize )
{
	myEnvSize = aEnvMapSize;
}	

bool RevRenderEnviromentHelper::Init()
{
	assert( myEnvSize != 0 );
	
	HRESULT hr = E_FAIL;

	const int mipLevels = 8;
	if( myEnvSize == 0) return false;

	D3D11_TEXTURE2D_DESC dstex;
    dstex.Width = myEnvSize;
    dstex.Height = myEnvSize;
    dstex.MipLevels = 1;
    dstex.ArraySize = 6;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;
    dstex.Format = DXGI_FORMAT_D32_FLOAT;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dstex.CPUAccessFlags = 0;
    dstex.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	

    // Create the depth stencil view for the entire cube
    D3D11_DEPTH_STENCIL_VIEW_DESC DescDS;
	DescDS.Flags = 0;
    DescDS.Format = DXGI_FORMAT_D32_FLOAT;
    DescDS.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
    DescDS.Texture2DArray.FirstArraySlice = 0;
    DescDS.Texture2DArray.ArraySize = 6;
    DescDS.Texture2DArray.MipSlice = 0;
	for(int i = 0; i < 6; i++)
	{
		ID3D11Texture2D* depthTex;
		RevEngineMain::GetDevice()->CreateTexture2D( &dstex, NULL, &depthTex );
		// Create the depth stencil view for single face rendering
		DescDS.Texture2DArray.ArraySize = 1;
		hr = RevEngineMain::GetDevice()->CreateDepthStencilView( depthTex, &DescDS, &myDSVs[ i ] );
	}


	dstex.ArraySize = 6;

    dstex.ArraySize = 6;
    dstex.SampleDesc.Count = 1;
    dstex.SampleDesc.Quality = 0;
    dstex.Format = DXGI_FORMAT_D32_FLOAT;
    dstex.Usage = D3D11_USAGE_DEFAULT;
    dstex.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    dstex.CPUAccessFlags = 0;
    dstex.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    // Create the cube map for env map render target
    dstex.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    dstex.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    dstex.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE;
    dstex.MipLevels = mipLevels;



    hr = RevEngineMain::GetDevice()->CreateTexture2D( &dstex, NULL, &myCubeMapTexture );

    // Create the 6-face render target view
    D3D11_RENDER_TARGET_VIEW_DESC DescRT;
    DescRT.Format = dstex.Format;
    DescRT.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
    DescRT.Texture2DArray.FirstArraySlice = 0;
    DescRT.Texture2DArray.ArraySize = 6;
    DescRT.Texture2DArray.MipSlice = 0;

    // Create the one-face render target views
    DescRT.Texture2DArray.ArraySize = 1;
    for( int i = 0; i < 6; ++i )
    {
        DescRT.Texture2DArray.FirstArraySlice = i;
		hr = RevEngineMain::GetDevice()->CreateRenderTargetView( myCubeMapTexture, &DescRT, &myCubeMapRTVS[i] );
    }

    // Create the shader resource view for the cubic env map
    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
    SRVDesc.Format = dstex.Format;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    SRVDesc.TextureCube.MipLevels = mipLevels;
    SRVDesc.TextureCube.MostDetailedMip = 0;
	hr = RevEngineMain::GetDevice()->CreateShaderResourceView( myCubeMapTexture, &SRVDesc, &mySRV );



	CU::Vector3f pos(0,0,0);
	CU::Vector3f at[6];
	at[0] = CU::Vector3f(pos.x + 1.0f, pos.y, pos.z);
	at[1] = CU::Vector3f(pos.x - 1.0f, pos.y, pos.z);
	at[2] = CU::Vector3f(pos.x, pos.y + 1.0f, pos.z);
	at[3] = CU::Vector3f(pos.x, pos.y - 1.0f, pos.z);
	at[4] = CU::Vector3f(pos.x, pos.y, pos.z + 1.0f);
	at[5] = CU::Vector3f(pos.x, pos.y, pos.z - 1.0f);

	CU::Vector3f up(0, 1, 0);
	CU::Vector3f ups[6];
	ups[0] = up;
	ups[1] = up;
	ups[2] = CU::Vector3f(0, 0, -1);
	ups[3] = CU::Vector3f(0, 0, 1);
	ups[4] = up;
	ups[5] = up;


	 D3DXMATRIX 				cameraMatrixes[ 6 ];
   float fHeight = 0.0f;
    D3DXVECTOR3 vEyePt = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;

    vLookDir = D3DXVECTOR3( at[0].x, at[0].y, at[0].z);
    vUpDir = D3DXVECTOR3(ups[0].x, ups[0].y, ups[0].z);
	D3DXMatrixLookAtLH( &cameraMatrixes[0], &vEyePt, &vLookDir, &vUpDir );
    vLookDir = D3DXVECTOR3( at[1].x, at[1].y, at[1].z);
    vUpDir = D3DXVECTOR3(ups[1].x, ups[1].y, ups[1].z);
    D3DXMatrixLookAtLH( &cameraMatrixes[1], &vEyePt, &vLookDir, &vUpDir );
    vLookDir = D3DXVECTOR3( at[2].x, at[2].y, at[2].z);
    vUpDir = D3DXVECTOR3(ups[2].x, ups[2].y, ups[2].z );
    D3DXMatrixLookAtLH( &cameraMatrixes[2], &vEyePt, &vLookDir, &vUpDir );
    vLookDir = D3DXVECTOR3( at[3].x, at[3].y, at[3].z);
    vUpDir = D3DXVECTOR3(ups[3].x, ups[3].y, ups[3].z);
    D3DXMatrixLookAtLH( &cameraMatrixes[3], &vEyePt, &vLookDir, &vUpDir );
    vLookDir = D3DXVECTOR3(at[4].x, at[4].y, at[4].z);
    vUpDir = D3DXVECTOR3( ups[4].x, ups[4].y, ups[4].z);
    D3DXMatrixLookAtLH( &cameraMatrixes[4], &vEyePt, &vLookDir, &vUpDir );
    vLookDir = D3DXVECTOR3( at[5].x, at[5].y, at[5].z);
    vUpDir = D3DXVECTOR3( ups[5].x, ups[5].y, ups[5].z);

    D3DXMatrixLookAtLH( &cameraMatrixes[5], &vEyePt, &vLookDir, &vUpDir );

	for(int cameraIndex = 0; cameraIndex < 6; cameraIndex++)
	{
		myViewMatrixes[ cameraIndex ].ConvertD3D( reinterpret_cast< float* >( &cameraMatrixes[ cameraIndex ].m ) );
		myViewMatrixes[ cameraIndex ] = myViewMatrixes[ cameraIndex ].Real4X4Inverse();
	}


	D3DXMATRIX m;

	D3DXMatrixPerspectiveFovLH( 
		&m,
		90 *(PI/180.0f),
		1.0f,
		1.0f,
		1000.0f);

	myProjMatrix.ConvertD3D( reinterpret_cast<float*>( m.m ));
	

	return true;
}

bool RevRenderEnviromentHelper::Setup( )
{
	RevEngineMain::GetContext()->OMSetRenderTargets(0, 0, NULL );
	float clearColor[4] = { 0, 0, 0, 1 };
	for(int cubeMapRTVIndex = 0; cubeMapRTVIndex < 6; cubeMapRTVIndex++ )
	{
		RevEngineMain::GetContext()->ClearRenderTargetView( myCubeMapRTVS[ cubeMapRTVIndex ], clearColor );
	}


	for(int cubeMapDSVIndex = 0; cubeMapDSVIndex < 6; cubeMapDSVIndex++ )
	{
		RevEngineMain::GetContext()->ClearDepthStencilView( myDSVs[ cubeMapDSVIndex ], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0 );
	}

	return true;
}


void RevRenderEnviromentHelper::Render( RevRenderMesh& aRenderMesh )
{

	D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT nViewPorts = 1;
	RevEngineMain::GetContext()->RSGetViewports( &nViewPorts, vpOld );


	D3D11_VIEWPORT  port;
	ZeroMemory(&port, sizeof( D3D11_VIEWPORT ) );
	port.Width = myEnvSize;
	port.Height = myEnvSize;
	port.TopLeftX = 0;
	port.TopLeftY = 0;
	port.MaxDepth = 1.0f;
	port.MinDepth = 0.0f;
	RevEngineMain::GetContext()->RSSetViewports( 1, &port );

	std::vector< RevMesh >& instances = aRenderMesh.GetInstances();
	RevModel* model = aRenderMesh.GetModel();

	for(int cubeMapRTVIndex = 0; cubeMapRTVIndex < 6; cubeMapRTVIndex++ )
	{
		RevEngineMain::GetContext()->OMSetRenderTargets(1, &myCubeMapRTVS[ cubeMapRTVIndex ], myDSVs[ cubeMapRTVIndex ] );
		RevCameraData data;
		data.myViewMatrix		= myViewMatrixes[ cubeMapRTVIndex ].Real4X4Inverse();
		data.myProjectionMatrix = myProjMatrix;
		RevEngineMain::GetEffectContainer().SetCameraMatrixes( data );
		for(int i = 0; i < instances.size(); i++ )
		{
			model->Render( instances[ i ].myWorldMatrix, myRenderTech );
		}
	}
	RevEngineMain::GetContext()->RSSetViewports( 1, vpOld );
}

ID3D11ShaderResourceView*	RevRenderEnviromentHelper::GetSRV( )
{
	return mySRV;
}

void RevRenderEnviromentHelper::ComputeSHAndSaveToEffect( RevEffect* aEffect )
{
	float	cubeMapColors[3][9];
	D3DX11SHProjectCubeMap( RevEngineMain::GetContext(), 3, myCubeMapTexture, &cubeMapColors[0][0], &cubeMapColors[1][0],&cubeMapColors[2][0]  );
	
	ComputeSHIrradEnvMapConstants(&cubeMapColors[0][0], &cubeMapColors[1][0],&cubeMapColors[2][0], aEffect );

}

void RevRenderEnviromentHelper::ComputeSHAndSaveToVector( std::vector< unsigned int >* aData, const int& aIndex )
{
	float	cubeMapColors[3][9];
	D3DX11SHProjectCubeMap( RevEngineMain::GetContext(), 3, myCubeMapTexture, &cubeMapColors[0][0], &cubeMapColors[1][0],&cubeMapColors[2][0]  );
	ComputeSHIrradEnvMapConstantsAndSaveToVector(&cubeMapColors[0][0], &cubeMapColors[1][0],&cubeMapColors[2][0], aData, aIndex );

}

void RevRenderEnviromentHelper::ComputeSHIrradEnvMapConstants( float* pSHCoeffsRed, float* pSHCoeffsGreen, float* pSHCoeffsBlue, RevEffect* aEffectToSave )
{
    HRESULT hr;
	RevEffect* saveInEffect = aEffectToSave;

    float* fLight[3] = { pSHCoeffsRed, pSHCoeffsGreen, pSHCoeffsBlue };

    // Lighting environment coefficients
    CU::Vector4f vCoefficients[3];

    // These constants are described in the article by Peter-Pike Sloan titled 
    // "Efficient Evaluation of Irradiance Environment Maps" in the book 
    // "ShaderX 2 - Shader Programming Tips and Tricks" by Wolfgang F. Engel.
    static const float s_fSqrtPI = ( ( float )sqrtf( D3DX_PI ) );
    const float fC0 = 1.0f / ( 2.0f * s_fSqrtPI );
    const float fC1 = ( float )sqrt( 3.0f ) / ( 3.0f * s_fSqrtPI );
    const float fC2 = ( float )sqrt( 15.0f ) / ( 8.0f * s_fSqrtPI );
    const float fC3 = ( float )sqrt( 5.0f ) / ( 16.0f * s_fSqrtPI );
    const float fC4 = 0.5f * fC2;

    int iChannel;
    for( iChannel = 0; iChannel < 3; iChannel++ )
    {
        vCoefficients[iChannel].x = -fC1 * fLight[iChannel][3];
        vCoefficients[iChannel].y = -fC1 * fLight[iChannel][1];
        vCoefficients[iChannel].z = fC1 * fLight[iChannel][2];
        vCoefficients[iChannel].w = fC0 * fLight[iChannel][0] - fC3 * fLight[iChannel][6];
    }


	saveInEffect->SetVector4Varible( RevEffect::_C_AR_, vCoefficients[0] );
	saveInEffect->SetVector4Varible( RevEffect::_C_AG_, vCoefficients[1] );
	saveInEffect->SetVector4Varible( RevEffect::_C_AB_, vCoefficients[2] );


    for( iChannel = 0; iChannel < 3; iChannel++ )
    {
        vCoefficients[iChannel].x = fC2 * fLight[iChannel][4];
        vCoefficients[iChannel].y = -fC2 * fLight[iChannel][5];
        vCoefficients[iChannel].z = 3.0f * fC3 * fLight[iChannel][6];
        vCoefficients[iChannel].w = -fC2 * fLight[iChannel][7];
    }


	saveInEffect->SetVector4Varible( RevEffect::_C_BR_, vCoefficients[0] );
    saveInEffect->SetVector4Varible( RevEffect::_C_BG_, vCoefficients[1] );
	saveInEffect->SetVector4Varible( RevEffect::_C_BB_, vCoefficients[2] );


    vCoefficients[0].x = fC4 * fLight[0][8];
    vCoefficients[0].y = fC4 * fLight[1][8];
    vCoefficients[0].z = fC4 * fLight[2][8];
    vCoefficients[0].w = 1.0f;

	saveInEffect->SetVector4Varible( RevEffect::_C_CC_, vCoefficients[0] );
   
}

void RevRenderEnviromentHelper::ComputeSHIrradEnvMapConstantsAndSaveToVector( float* pSHCoeffsRed, float* pSHCoeffsGreen, float* pSHCoeffsBlue, std::vector< unsigned int >* aData, const int& aIndex  )
{
	  float* fLight[3] = { pSHCoeffsRed, pSHCoeffsGreen, pSHCoeffsBlue };

    // Lighting environment coefficients
    CU::Vector4f vCoefficients[3];

    // These constants are described in the article by Peter-Pike Sloan titled 
    // "Efficient Evaluation of Irradiance Environment Maps" in the book 
    // "ShaderX 2 - Shader Programming Tips and Tricks" by Wolfgang F. Engel.
    static const float s_fSqrtPI = ( ( float )sqrtf( D3DX_PI ) );
    const float fC0 = 1.0f / ( 2.0f * s_fSqrtPI );
    const float fC1 = ( float )sqrt( 3.0f ) / ( 3.0f * s_fSqrtPI );
    const float fC2 = ( float )sqrt( 15.0f ) / ( 8.0f * s_fSqrtPI );
    const float fC3 = ( float )sqrt( 5.0f ) / ( 16.0f * s_fSqrtPI );
    const float fC4 = 0.5f * fC2;

    int iChannel;
    for( iChannel = 0; iChannel < 3; iChannel++ )
    {
        vCoefficients[iChannel].x = -fC1 * fLight[iChannel][3];
        vCoefficients[iChannel].y = -fC1 * fLight[iChannel][1];
        vCoefficients[iChannel].z = fC1 * fLight[iChannel][2];
        vCoefficients[iChannel].w = fC0 * fLight[iChannel][0] - fC3 * fLight[iChannel][6];
    }


	aData[ 0 ][ aIndex ] =  PackFloat( vCoefficients[0] );
	aData[ 1 ][ aIndex ] =  PackFloat( vCoefficients[1] );
	aData[ 2 ][ aIndex ] =  PackFloat( vCoefficients[2] );


    for( iChannel = 0; iChannel < 3; iChannel++ )
    {
        vCoefficients[iChannel].x = fC2 * fLight[iChannel][4];
        vCoefficients[iChannel].y = -fC2 * fLight[iChannel][5];
        vCoefficients[iChannel].z = 3.0f * fC3 * fLight[iChannel][6];
        vCoefficients[iChannel].w = -fC2 * fLight[iChannel][7];
    }


	aData[ 3 ][ aIndex ] = PackFloat( vCoefficients[0] );
    aData[ 4 ][ aIndex ] = PackFloat( vCoefficients[1] );
	aData[ 5 ][ aIndex ] = PackFloat( vCoefficients[2] ) ;


    vCoefficients[0].x = fC4 * fLight[0][8];
    vCoefficients[0].y = fC4 * fLight[1][8];
    vCoefficients[0].z = fC4 * fLight[2][8];
    vCoefficients[0].w = 1.0f;

	aData[ 6 ][ aIndex ] =  PackFloat( vCoefficients[0] );
}

void RevRenderEnviromentHelper::SaveBlankSHToVector( std::vector< unsigned int >* aData, const int& aIndex )
{
	for(int i = 0; i < 7; i++)
	{
		aData[ i ][ aIndex ] = PackFloat( CU::Vector4f( 0.0f, 0.0f, 0.0f, 0.0f ) );
	}
}

void RevRenderEnviromentHelper::SetPosition( const CU::Vector3f& aPosition )
{
	for(int cubeMapViewMatrixes = 0; cubeMapViewMatrixes < 6; cubeMapViewMatrixes++ )
	{
		myViewMatrixes[ cubeMapViewMatrixes ].SetPosition( aPosition );
	}
}

ID3D11Texture2D* RevRenderEnviromentHelper::GetCubeTexture( )
{
	return myCubeMapTexture;
}

unsigned int RevRenderEnviromentHelper::PackFloat( CU::Vector4f& aVector )
{

	CU::Vector4f v = aVector;
	v.x=min(v.x,1);
	v.y=min(v.y,1);
	v.z=min(v.z,1);
	v.w=min(v.w,1);

	v.x=max(v.x,-1);
	v.y=max(v.y,-1);
	v.z=max(v.z,-1);
	v.w=max(v.w,-1);
	
	v+= CU::Vector4f(1,1,1,1);
	v /= (2.0f);

	unsigned int packed = 0;
	unsigned int part = (unsigned char)( v.w * 255.0f);
	part <<= 24;
	packed |= part;
	part = (unsigned char)( v.z * 255.0f);
	part <<= 16;
	packed |= part;
	part = (unsigned char)( v.y * 255.0f);
	part <<= 8;
	packed |= part;
	part = (unsigned char)( v.x * 255.0f);
	packed |= part;
	return packed;
}
