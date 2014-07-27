#include "stdafx.h"
#include "RevRendererGIHelper.h"
#include "CU_Vector.h"
#include "RevRenderTargetCollector.h"
#include "RevEffect.h"
#include "RevEngineMain.h"
#include "RevRoomManager.h"

RevRendererGI::RevRendererGI(void)
{
	for(int i = 0; i < 7; i++) { my3DTextures[ i ] = NULL; }
	for(int i = 0; i < 7; i++) { my3DSRVs[ i ] = NULL; }

}


RevRendererGI::~RevRendererGI(void)
{
	for(int i = 0; i < 7; i++) { if( my3DTextures[ i ] != NULL ) my3DTextures[ i ]->Release(); }
	for(int i = 0; i < 7; i++) { if( my3DSRVs[ i ] != NULL ) my3DSRVs[ i ]->Release(); }
}

void RevRendererGI::SetEnviromentSize( const int& aEnvMapSize )
{
	myEnviromentHelper.SetEnviromentSize( aEnvMapSize );
	myEnviromentHelper.SetRenderTech( RevEffect::_NORMAL );
}

bool RevRendererGI::Init()
{
	return myEnviromentHelper.Init();
}

void RevRendererGI::Render( RevRenderTargetCollector& aRenderMesh, 
							RevRoomManager& aRoomManager )
{

	RevRoomManager& manager = aRoomManager;

	CU::Vector3f min;
	min = manager.GetMin();
	myMinWorld = min;

	CU::Vector3f max;
	max = manager.GetMax();
	myMaxWorld = max;

	CU::Vector3f extents = max - min;
	myExtents = extents;

	
	float stepsPerSize = 2.0f;
	CU::Vector3f halfExtents = extents / stepsPerSize;

	std::vector< RevRenderMesh >& meshes = aRenderMesh.GetRenderMeshes();
	
	for(int nBands = 0; nBands < 7; nBands++) 
	{
		const int nVectors = ( extents.x / stepsPerSize ) * ( extents.y / stepsPerSize ) * ( extents.z / stepsPerSize );
		myData[ nBands ].reserve( nVectors );
		for( int vIndex = 0; vIndex < nVectors; vIndex++ )
		{
			unsigned int value = 0;
			myData[ nBands ].push_back( value );
		}
	}
	
	int zIndex = 0;
	int xIndex = 0;
	int yIndex = 0;



	for(float z = 1, zIndex = 0; z <= (extents.z - 1); z += stepsPerSize, zIndex++ )
	{
		for(float y = 1, yIndex = 0; y <= (extents.y - 1); y += stepsPerSize, yIndex++ )
		{
			for(float x = 1, xIndex = 0; x <= (extents.x - 1); x += stepsPerSize, xIndex++ )
			{
				CU::Vector3f pos = min + CU::Vector3f(x,y,z);
				const int index = xIndex + yIndex * halfExtents.x + zIndex * halfExtents.x * halfExtents.y;
				if( manager.IsPointInsideARoom( pos ) )
				{
					myEnviromentHelper.SetPosition( pos );
					myEnviromentHelper.Setup( );

					for(int i = 0; i < meshes.size(); i++)
					{
						if( meshes[ i ].GetInstances().size() >  0 )
						{
							myEnviromentHelper.Render( meshes[ i ] );
						}
					}
					RevEngineMain::GetContext()->OMSetRenderTargets(0,0,0);

					myEnviromentHelper.ComputeSHAndSaveToVector( &myData[0], index );
				}
				else
				{
					myEnviromentHelper.SaveBlankSHToVector( &myData[0], index );
				}
			}
		}
	}
	
	for(int i = 0; i < 7; i++)
	{
		my3DTextures[i] = NULL;
		D3D11_TEXTURE3D_DESC desc;
		ZeroMemory( &desc, sizeof( D3D11_TEXTURE3D_DESC ) );
		desc.Width	= halfExtents.x;
		desc.Height	= halfExtents.y;
		desc.Depth	= halfExtents.z;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1; 
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory( &data, sizeof(D3D11_SUBRESOURCE_DATA) );
		data.pSysMem = &myData[i][0];
		data.SysMemPitch = desc.Width * ( sizeof( unsigned int ) );
		data.SysMemSlicePitch = desc.Width * desc.Height * (sizeof( unsigned int ));

		HRESULT hr = RevEngineMain::GetDevice()->CreateTexture3D( &desc, &data, &my3DTextures[i] );
		assert( hr == S_OK );


		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		srvDesc.Format = desc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE3D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		hr = RevEngineMain::GetDevice()->CreateShaderResourceView(my3DTextures[i], &srvDesc, &my3DSRVs[i]);
		assert( hr == S_OK );
		my3DTextures[i]->Release();
	}
/*	for(int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Clear();
	}
	*/
}


void RevRendererGI::SetValuesInEffect( RevEffect* aEffect )
{
	aEffect->SetTextureVarible("g_TexturecAr", my3DSRVs[ 0 ] );
	aEffect->SetTextureVarible("g_TexturecAg", my3DSRVs[ 1 ] );
	aEffect->SetTextureVarible("g_TexturecAb", my3DSRVs[ 2 ] );
	aEffect->SetTextureVarible("g_TexturecBr", my3DSRVs[ 3 ] );
	aEffect->SetTextureVarible("g_TexturecBg", my3DSRVs[ 4 ] );
	aEffect->SetTextureVarible("g_TexturecBb", my3DSRVs[ 5 ] );
	aEffect->SetTextureVarible("g_TexturecC",  my3DSRVs[ 6 ] );

	aEffect->SetVector4Varible(RevEffect::_MIN_WORLD, myMinWorld );
	aEffect->SetVector4Varible(RevEffect::_MAX_WORLD, myMaxWorld );
	aEffect->SetVector4Varible(RevEffect::_WORLD_EXTENTS, myExtents );
}