#include "StdAfx.h"
#include "RevModelFactory.h"
#include "RevModel.h"
#include "RevStructures.h"
#include "RevDefines.h"
#include "RevEffect.h"
#include "RevTerrain.h"
#include "DirectXPackedVector.h"
#include "RevFile.h"
#include "RevTexture.h"
#include "RevCast.h"
#include <fstream>
#include <vector>
#include <DirectXMath.h>
#include <algorithm> 

RevModelFactory::RevModelFactory(void)
{
}

RevModelFactory::~RevModelFactory(void)
{
	for(auto modelIndex : m_createdModels )
	{
		delete modelIndex;	
	}
	m_createdModels.clear();
}
/*
RevModel* RevModelFactory::GetSphereModel(	const float& aRadius,
								const float&,
								const float& aR, 
								const float& aG,
								const float& aB )
{	
	RevModel* model = new RevModel();
	model->InitShader( "Shaders\\Object_PosCol.txt" );


	const float radius		= aRadius;
	const int slices		= (int)15;
	const int polygonSize	 = (int)slices-1;
	
	RevVertexData vbData( slices * slices + 2 );
	vbData.myStride = sizeof( RevVertexPosCol );
	vbData.mySize = vbData.myStride * vbData.myNrOfVertexes;
	const int n = vbData.myNrOfVertexes;
	
	//Vertex initialization
	/*RevVertexPosCol* vertexvbData.myVertexData = new RevVertexPosCol[ vbData.myNrOfVertexes ];
	float theta = 0.0f;
	float phi = 0;

	vbData.myVertexData[ 0 ].myPos = CU::Vector3f(0,0,-2000);
	vbData.myVertexData[ 0 ].myColor = CU::Vector4f(1,1,1,1);
	int id = 0;
	for (int i = 0; i < slices; i++)
	{
		theta += XM_PI / (slices);
		phi = 0;
		for (int j = 0; j < slices; j++)
		{
				phi +=(XM_PI*2/ (slices));
			CU::Vector3f pos = CU::Vector3f(
				(float)sinf(theta) * (float)cosf(phi),
				(float)cosf(theta),
				(float)sinf(theta)* (float)sinf(phi)) ;

			const int index = i * slices + j;
			vbData.myVertexData[index + 1].myPos = pos;
			vbData.myVertexData[index + 1].myColor = CU::Vector4f( aR, aG, aB, 1 );
			id++;
		
		}
		
	}

	vbData.myVertexData[ vbData.myNrOfVertexes - 1 ].myPos = CU::Vector3f(0, 0, -1);

//	vbData.myVertexData[ vbData.myNrOfVertexes ].myPos = CU::Vector3f(0,0,1);





	//vbData.myVertexData		= new char[ vbData.mySize ];
	//memcpy( vbData.myVertexData, &vertex, vbData.mySize );

	//Index initialization
	RevIndexData ibData;
	ibData.myNrOfIndexes =  slices * slices * 6 + slices * 3 * 2;
	ibData.mySize = sizeof( UINT ) * ibData.myNrOfIndexes;
	ibData.myFormat = DXGI_FORMAT_R32_UINT;
	
	//UINT* indexes = new UINT[ polygonSize * polygonSize * 6 ];

	ibData.myIndexData = new UINT[ 3 * (slices - 1) +  ];
	std::vector<UINT> vs;
	
	int indexCount=0;
	for(int i = 1; i < slices; i++)
	{
		ibData.myIndexData[ indexCount ]	 =	0;
		ibData.myIndexData[ indexCount + 1 ]  = i+1;
		ibData.myIndexData[ indexCount + 2 ] =  i;
		indexCount += 3;
	}
	
	
	for (int i = 0; i < polygonSize-2; i++)
	{
		for (int j = 0; j < slices; j++)
		{
			ibData.myIndexData[indexCount + 2] = 1 + (UINT)(i * (slices+1) + j);
			ibData.myIndexData[indexCount + 1] = 1 + (UINT)(i * (slices+1)  + j + 1);
			ibData.myIndexData[indexCount + 0] = 1 + (UINT)((i + 1) * (slices+1)  + j);
			ibData.myIndexData[indexCount + 5] = 1 + (UINT)(i * (slices+1)  + (j + 1));
			ibData.myIndexData[indexCount + 4] = 1 + (UINT)((i + 1) * (slices+1)  + (j + 1));
			ibData.myIndexData[indexCount + 3] = 1 + (UINT)((i + 1) * (slices+1)  + (j));
			indexCount += 6;
		}
	}
	

	
	for(UINT i = 0; i < slices; ++i)
	{
		meshData.Indices.push_back(southPoleIndex);
		meshData.Indices.push_back(baseIndex+i);
		meshData.Indices.push_back(baseIndex+i+1);
	}

	/*
	for(int i = 0; i < slices - 2; i++)
	{
		ibData.myIndexData[ indexCount ]	 = vbData.myNrOfVertexes - 1;
		ibData.myIndexData[ indexCount + 1 ] = vbData.myNrOfVertexes - (i + 3);
		ibData.myIndexData[ indexCount + 2 ] = vbData.myNrOfVertexes - (i + 2);
		indexCount += 3;
	}
	
//	ibData.myIndexData = new char[ ibData.mySize  ];
//	memcpy( ibData.myIndexData, &indexes, ibData.mySize );

	RevInputData data( const_cast<D3D11_INPUT_ELEMENT_DESC*>( VertexPosColLayout ) );
	data.myNrOfElements = ARRAYSIZE( VertexPosColLayout );
	
	RevModelRenderEssentials essentials;
	essentials.myVertexStart = 0;
	essentials.myIndexStart = 0;
	essentials.myIndexCount = ibData.myNrOfIndexes;

	if( model->Init(	vbData,
					ibData,
					data,
					essentials ) == false)
	{
		delete model; model = NULL;
	}
	
	return model;
}*/

#pragma optimize("", off )

RevModel* RevModelFactory::GetSphereSliceDefered( )
{
	RevModel* model = new RevModel();
	model->InitShader( "Shaders\\Fullscreen.txt" );
	model->SetIsNullObjectFlag(false); 
	
	const int VertexSize=12;
	const int PolygonSize = VertexSize-1;

	float theta = -90.0f * (3.14157f / 180.0f);
	float phi = 0.0f;

	std::vector< RevVertexPos > vertexes;
	for (int i = 0; i < VertexSize; i++)
	{
		phi = 0;
		for (int j = 0; j < VertexSize; j++)
		{
			RevVertexPos vertex;
				vertex.myPos =  CU::Vector3f(
				(float)cos(theta) * (float)cos(phi),
				(float)cos(theta) * (float)sin(phi),
				(float)sin(theta) );
			vertexes.push_back( vertex );
			phi +=((360.0f) / (float)PolygonSize) * (DirectX::XM_PI / 180.0f);
		}
		theta += ((180.0f / (float)PolygonSize)* (DirectX::XM_PI / 180.0f));
	}

	RevVertexDataNormal vbData( vertexes.size() );
	vbData.myStride = sizeof( RevVertexPos );
	vbData.mySize = vbData.myStride * vbData.myNrOfVertexes;
	vbData.myVertexData = new char[ vbData.mySize ];

	std::vector< UINT> indicies;
	indicies.reserve(PolygonSize * PolygonSize * 6);
	int indiceCount=0;
	for (int i = 0; i < PolygonSize; i++)
	{
		for (int j = 0; j < PolygonSize; j++)
		{
			indicies.push_back((UINT)((i + 1) * VertexSize + j));
			indicies.push_back((UINT)(i *		VertexSize + j + 1));
			indicies.push_back((UINT)(i *		VertexSize + j));
			indicies.push_back((UINT)((i + 1) * VertexSize + (j))) ;
			indicies.push_back((UINT)((i + 1) * VertexSize + (j + 1)));
			indicies.push_back((UINT)(i *		VertexSize + (j + 1)));
			indiceCount += 6;
		}
	}

// 	for(int i = 0; i < vertexes.size(); i++)
// 	{ 
// 		RevVertexPos v;
// 		memcpy( &v, &vertexes[i], vbData.myStride );
// 		memcpy( &vbData.myVertexData[ offset ], &v, vbData.myStride );
// 		offset += vbData.myStride;
// 	}

	memcpy(&vbData.myVertexData[0], &vertexes[0], vbData.mySize);

	RevIndexData ibData;
	ibData.myNrOfIndexes = indicies.size();
	ibData.mySize = sizeof(UINT) * indicies.size();
	ibData.myFormat = DXGI_FORMAT_R32_UINT; 
	ibData.myIndexData = new char[ibData.mySize];
	
	memcpy(&ibData.myIndexData[0], &indicies[0], ibData.mySize);

	RevInputData data;
	const int nElements = ARRAYSIZE( VertexPosLayout );
	for(int i = 0; i < nElements; i++ )
	{
		data.myElements.push_back( VertexPosLayout[i] );
	}

	data.m_effectTech = RevEffect::_PBL_POINT_LIGHT;

	RevModelRenderEssentials essentials( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	essentials.myVertexStart = 0;
	essentials.myIndexStart = 0;
	essentials.myIndexCount = ibData.myNrOfIndexes;
	essentials.myIndexCount = indicies.size();


	model->Init( vbData, ibData, data, essentials );

	m_createdModels.push_back(model);
	return model;
}

RevModel* RevModelFactory::GetSphereModel(float radius, UINT sliceCount, UINT stackCount, const float& aR, const float& aG, const float& aB)
{
/*	std::vector<RevVertexPosColNorm> verts;
	std::vector<UINT> indices;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	RevVertexPosColNorm topVertex;
	topVertex.myPos = CU::Vector3f(0.0f, +radius, 0.0f);
	topVertex.myNorm = topVertex.myPos.NonSelfNormalize();
	topVertex.myColor = CU::Vector4f(aR,aG,aB,1);
	verts.push_back( topVertex );

	RevVertexPosColNorm bottomVertex;
	bottomVertex.myPos = CU::Vector3f(0.0f, -radius, 0.0f);

	bottomVertex.myNorm = bottomVertex.myPos.NonSelfNormalize();
	bottomVertex.myColor = CU::Vector4f(aR,aG,aB,1);

	float phiStep   = DirectX::XM_PI/stackCount;
	float thetaStep = 2.0f*DirectX::XM_PI/sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for(UINT i = 1; i <= stackCount-1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for(UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			RevVertexPosColNorm v;

			// spherical to cartesian
			v.myPos.x = radius*sinf(phi)*cosf(theta);
			v.myPos.y = radius*cosf(phi);
			v.myPos.z = radius*sinf(phi)*sinf(theta);
			CU::Vector3f pos2 = v.myPos;
			CU::Vector3f normal = pos2.Normalize();
			v.myNorm = normal;
			v.myColor = CU::Vector4f(aR,aG,aB,1);
			verts.push_back( v );
		}
	}

	verts.push_back( bottomVertex );

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for(UINT i = 1; i <= sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i+1);
		indices.push_back(i);
	}
	
	//
	// Compute indices for inner stacks (not connected to poles).
	//
	
	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount+1;
	for(UINT i = 0; i < stackCount-2; ++i)
	{
		for(UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j+1);
			indices.push_back(baseIndex + (i+1)*ringVertexCount + j);

			indices.push_back(baseIndex + (i+1)*ringVertexCount + j);
			indices.push_back(baseIndex + i*ringVertexCount + j+1);
			indices.push_back(baseIndex + (i+1)*ringVertexCount + j+1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)verts.size()-1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;
	
	for(UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex+i);
		indices.push_back(baseIndex+i+1);
	}
	
	RevVertexDataNormal vbData( verts.size() );
	vbData.myNrOfVertexes = verts.size();
	vbData.myStride = sizeof( RevVertexPosColNorm );
	vbData.mySize = vbData.myStride * vbData.myNrOfVertexes;
	vbData.myVertexData = new char[ vbData.mySize ];
	vbData.myData = new RevVertexPosColNorm[ vbData.myNrOfVertexes ];
	int offset = 0;

	//Stl vectors saves certain memory unallign, so need to push the offset
	for(int i = 0; i < verts.size(); i++)
	{ 
		RevVertexPosColNorm v;
		memcpy( &v, &verts[i], vbData.myStride );
		memcpy( &vbData.myVertexData[ offset ], &v, vbData.myStride );
		offset += vbData.myStride;
	}

	
	RevIndexData ibData;
	ibData.myNrOfIndexes =  indices.size();
	ibData.mySize = sizeof( UINT ) * ibData.myNrOfIndexes;
	ibData.myFormat = DXGI_FORMAT_R32_UINT;


	ibData.myIndexData = new char[ ibData.mySize ];
	offset = 0;
	
	for(int i = 0; i < indices.size(); i++)
	{ 
		UINT v;
		memcpy( &v, &indices[i], sizeof( UINT ) );
		memcpy( &ibData.myIndexData[ offset ], &v, sizeof( UINT ) );
		offset += sizeof( UINT );
	}

	//for(int i = 0; i < indices.size(); i++) { ibData.myIndexData[i] = indices[i]; }


	RevInputData data;
	const int nElements = ARRAYSIZE( VertexPosColNormLayout );
	for(int i = 0; i < nElements; i++ )
	{
		data.myElements.push_back( VertexPosColNormLayout[i] );
	}

	RevModelRenderEssentials essentials( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	essentials.myVertexStart = 0;
	essentials.myIndexStart = 0;
	essentials.myIndexCount = ibData.myNrOfIndexes;


	RevModel* model = new RevModel( );
	model->SetIsNullObjectFlag( false );
	model->InitShader( "Shaders\\Object_PosColNorm.txt" );
	model->Init( vbData, ibData, data, essentials );

	return model;*/
	assert( 0 && "NOT IMPLEMENTED YET");
	return nullptr;
}



RevModel* RevModelFactory::CreateBox(float width, float height, float depth, const CU::Vector4f& color)
{
	//
	// Create the vertices.
	//

	RevVertexDataNormal vbData(24);
	vbData.myStride = sizeof( RevVertexPosCol );
	vbData.mySize = vbData.myStride * vbData.myNrOfVertexes;
	vbData.myVertexData = new char[ vbData.mySize ];

	float w2 = 0.5f*width;
	float h2 = 0.5f*height;
	float d2 = 0.5f*depth;
		
	RevVertexPosCol vP[24];

	// Fill in the front face vertex data.
	vP[0].myPos = CU::Vector3f(-w2, -h2, -d2);
	vP[1].myPos = CU::Vector3f(-w2, +h2, -d2);
	vP[2].myPos = CU::Vector3f(+w2, +h2, -d2);
	vP[3].myPos= CU::Vector3f(+w2, -h2, -d2);

	// Fill in the back face vertex data.
	vP[4].myPos = CU::Vector3f(-w2, -h2, +d2);
	vP[5].myPos = CU::Vector3f(+w2, -h2, +d2);
	vP[6].myPos = CU::Vector3f(+w2, +h2, +d2);
	vP[7].myPos = CU::Vector3f(-w2, +h2, +d2);

	// Fill in the top face vertex data.
	vP[8].myPos  = CU::Vector3f(-w2, +h2, -d2);
	vP[9].myPos  = CU::Vector3f(-w2, +h2, +d2);
	vP[10].myPos = CU::Vector3f(+w2, +h2, +d2);
	vP[11].myPos = CU::Vector3f(+w2, +h2, -d2);

	// Fill in the bottom face vertex data.
	vP[12].myPos = CU::Vector3f(-w2, -h2, -d2);
	vP[13].myPos = CU::Vector3f(+w2, -h2, -d2);
	vP[14].myPos = CU::Vector3f(+w2, -h2, +d2);
	vP[15].myPos = CU::Vector3f(-w2, -h2, +d2);

	// Fill in the left face vertex data.
	vP[16].myPos = CU::Vector3f(-w2, -h2, +d2);
	vP[17].myPos = CU::Vector3f(-w2, +h2, +d2);
	vP[18].myPos = CU::Vector3f(-w2, +h2, -d2);
	vP[19].myPos = CU::Vector3f(-w2, -h2, -d2);

	// Fill in the right face vertex data.
	vP[20].myPos = CU::Vector3f(+w2, -h2, -d2);
	vP[21].myPos = CU::Vector3f(+w2, +h2, -d2);
	vP[22].myPos = CU::Vector3f(+w2, +h2, +d2);
	vP[23].myPos = CU::Vector3f(+w2, -h2, +d2);

	for (auto vIndex = 0; vIndex < 24; vIndex++)
	{
		vP[vIndex].myColor = color;
	}

	memcpy(vbData.myVertexData, &vP[0], sizeof(RevVertexPosCol)* 24);

	//
	// Create the indices.
	//

	RevIndexData ibData;
	ibData.myNrOfIndexes =  36;
	ibData.mySize = sizeof( UINT ) * ibData.myNrOfIndexes;
	ibData.myFormat = DXGI_FORMAT_R32_UINT;
	UINT* indexData = new UINT[ ibData.myNrOfIndexes ];
	ibData.myIndexData = new char[ ibData.mySize ];


	// Fill in the front face index data
	indexData[0] = 0; indexData[1] = 1; indexData[2] = 2;
	indexData[3] = 0; indexData[4] = 2; indexData[5] = 3;

	// FindexDatall indexDatan the back face indexDatandex data
	indexData[6] = 4; indexData[7]  = 5; indexData[8]  = 6;
	indexData[9] = 4; indexData[10] = 6; indexData[11] = 7;

	// FindexDatall indexDatan the top face indexDatandex data
	indexData[12] = 8; indexData[13] =  9; indexData[14] = 10;
	indexData[15] = 8; indexData[16] = 10; indexData[17] = 11;

	// FindexDatall indexDatan the bottom face indexDatandex data
	indexData[18] = 12; indexData[19] = 13; indexData[20] = 14;
	indexData[21] = 12; indexData[22] = 14; indexData[23] = 15;

	// FindexDatall indexDatan the left face indexDatandex data
	indexData[24] = 16; indexData[25] = 17; indexData[26] = 18;
	indexData[27] = 16; indexData[28] = 18; indexData[29] = 19;

	// FindexDatall indexDatan the rindexDataght face indexDatandex data
	indexData[30] = 20; indexData[31] = 21; indexData[32] = 22;
	indexData[33] = 20; indexData[34] = 22; indexData[35] = 23;

	memcpy( &ibData.myIndexData, &indexData[0], ibData.mySize );

	RevInputData data;
	for(int i = 0; i < ARRAYSIZE( VertexPosColLayout ); i++ )
	{
		data.myElements.push_back( VertexPosColLayout[i] );
	}

	RevModelRenderEssentials essentials( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	essentials.myVertexStart = 0;
	essentials.myIndexStart = 0;
	essentials.myIndexCount = ibData.myNrOfIndexes;


	RevModel* model = new RevModel( );
	model->InitShader( "Shaders\\Object_PosCol.txt" );
	model->Init( vbData, ibData, data, essentials );
	m_createdModels.push_back(model);
	return model;
}

RevModel* RevModelFactory::CreateModel( const char* aFilePath )
{
	for(auto modelIndex : m_createdModels )
	{
		RevModel* model = modelIndex;
		assert( model != nullptr );
		if( model->GetModelFilePath() == aFilePath )
		{
			return modelIndex;
		}
	}
	
	RevModel* generatedModel = myMeshFactory.LoadModel( aFilePath );	

	if( generatedModel != nullptr )
	{
		m_createdModels.push_back( generatedModel );
	}

	return generatedModel;
}

RevTerrain* RevModelFactory::CreateTerrain( const char* terrainPath )
{
	RevTerrain* terrain = new RevTerrain();
	terrain->SetModelFilePath( terrainPath );
	terrain->m_tileSize = 0.25f;
	terrain->m_terrainHeight = 512.0f;
	const int GridSize=64; // For 4096 size Heightfield
	const int stepsPerGrid=4096/GridSize;
	const int GridSize1=GridSize+1; // For 4096 size Heightfield



	std::vector<float> heightData;
	terrain->m_heightData.reserve(4097*4097);

	FI_File::RevFile file;
	file.Open( terrainPath );
	for(int i=0;i<4097*4097;i++)
	{
		unsigned short temp;
		file.Read( temp );
		terrain->m_heightData.push_back((float)temp/65536.0f*terrain->m_terrainHeight );
	}

	file.Close();
	const float MinimumSize=terrain->m_tileSize;
	const float TileSize=stepsPerGrid*MinimumSize;

	terrain->m_vertexes.reserve( GridSize1 * GridSize1 );

	int count2 = 0;
	RevVertexPosUV vertex;
	float verts[  GridSize1 * GridSize1 * 3 ];
	for(int i=0;i<GridSize1;i++)
	{
		for(int j=0;j<GridSize1;j++)
		{
			int iStep=i*stepsPerGrid;
			int jStep=j*stepsPerGrid;
			vertex.myUV = CU::Vector2f( float(j)/float(GridSize),float(i)/float(GridSize) );
			vertex.myPos = CU::Vector3f(	(j-GridSize/2)*TileSize,
											0.0f,
												(i-GridSize/2)*TileSize );
			terrain->m_vertexes.push_back( vertex );
			verts[count2]	= vertex.myPos.x;
			verts[count2+1]	= vertex.myPos.y;
			verts[count2+2]	= vertex.myPos.z;
			count2 += 3;
		}
	}

	
	int indexes[(GridSize*GridSize*3) * 2];
	int count1=0;

	for(int i=0;i<GridSize;i++)
	{
		for(int j=0;j<GridSize;j++)
		{
			indexes[count1]=i*GridSize1+j;
			indexes[count1+1]=i*GridSize1+(j+1);
			indexes[count1+2]=(i+1)*GridSize1+(j);
			
			indexes[count1+3]=(i+1)*GridSize1+(j+1);
			indexes[count1+4]=(i+1)*GridSize1+(j);
			indexes[count1+5]=(i)*GridSize1+(j+1);

			count1+=6;
		}
	}

//	revstuff.Init( &verts[0], GridSize1 * GridSize1 * 3, &indexes[0], count1 );


	RevVertexDataNormal vData( GridSize1*GridSize1 );

	vData.myStride=sizeof(RevVertexPosUV );
	vData.mySize=vData.myNrOfVertexes*vData.myStride;
	vData.myVertexData= new char [vData.mySize]();
	memcpy(vData.myVertexData,&terrain->m_vertexes[0],vData.mySize);

	RevIndexData indexData;
	indexData.myFormat=DXGI_FORMAT_R32_UINT;
	indexData.myNrOfIndexes=count1;
	indexData.mySize=indexData.myNrOfIndexes * sizeof(int);
	indexData.myIndexData = new char [indexData.mySize]();
	memcpy(indexData.myIndexData,&indexes[0],indexData.mySize);


	terrain->InitShader( "Shaders\\EffectTerrain.txt" );
	
	RevModelRenderEssentials renderEssentials( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	renderEssentials.myIndexCount = indexData.myNrOfIndexes;
	renderEssentials.myIndexStart = 0;
	renderEssentials.myVertexStart = 0;

	ID3D11ShaderResourceView* rsv = CreateQuadPatchR16Texture(terrain);
	terrain->AddTexture( "g_HeightMap", rsv);

	float pixelPerMeter = static_cast<float>(RevEngineMain::GetWindowWidth()) / (45.0f*2);
	float factor = 	pixelPerMeter * 4096;

	RevEffect* effect = terrain->GetEffect();

/*	RevTexture* texOne = new RevTexture();
	texOne->LoadTexture( "Terrain\\Snow\\Snow_albedo_01.dds",true );

	RevTexture* texTwo = new RevTexture();
	texTwo->LoadTexture( "Terrain\\Rock\\Rock_albedo_01.dds",true );

	RevTexture* texThree = new RevTexture();
	texThree->LoadTexture( "Terrain\\Grass\\Grass_albedo_01.dds",true );

	RevTexture* texFour = new RevTexture();
	texFour->LoadTexture( "Terrain\\Ice\\Ice_albedo_01.dds",true );

	terrain->AddTexture( "g_TexOneDiffuse", texOne->GetSRV() );
	terrain->AddTexture( "g_TexTwoDiffuse", texTwo->GetSRV() );
	terrain->AddTexture( "g_TexThreeDiffuse", texThree->GetSRV() );
	terrain->AddTexture( "g_TexFourDiffuse", texFour->GetSRV() );

	RevTexture* blend = new RevTexture();
	blend->LoadTexture( "Level_Crosses_Splat.dds",true );
	terrain->AddTexture( "g_BlendTex", blend->GetSRV() );*/
	/*
	effect->SetFloatVarible( RevEffect::_TEXTILEZEROAMOUNT, factor / texOne->GetImageSize().x);
	effect->SetFloatVarible( RevEffect::_TEXTILEONEAMOUNT, factor / texTwo->GetImageSize().x);
	effect->SetFloatVarible( RevEffect::_TEXTILETWOAMOUNT, factor / texThree->GetImageSize().x);
	effect->SetFloatVarible( RevEffect::_TEXTILETHREEAMOUNT, factor / texFour->GetImageSize().x);
	*/
	RevInputData input;
	input.myElements.push_back(VertexPosUV[0]);
	input.myElements.push_back(VertexPosUV[1]);
	terrain->SetIsNullObjectFlag( false );
	terrain->Init( vData, indexData, input, renderEssentials );
	terrain->SetModelType( RevModel::_TERRAIN );
	m_createdModels.push_back( terrain );
	return(terrain);

}

ID3D11ShaderResourceView* RevModelFactory::CreateQuadPatchR16Texture( RevTerrain* terrain )
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 4097;
	texDesc.Height = 4097;
    texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	std::vector<DirectX::PackedVector::HALF> hmap(terrain->m_heightData.size());

	std::transform(terrain->m_heightData.begin(), 
	terrain->m_heightData.end(), 
					hmap.begin(), 
					DirectX::PackedVector::XMConvertFloatToHalf);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
    data.SysMemPitch = (4097)*sizeof(DirectX::PackedVector::HALF);
    data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	HRESULT result = RevEngineMain::GetDevice()->CreateTexture2D(&texDesc, &data, &hmapTex);

	ID3D11ShaderResourceView* resourceView = NULL;	
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	result = RevEngineMain::GetDevice()->CreateShaderResourceView(hmapTex, &srvDesc, &resourceView);

	// SRV saves reference.
	hmapTex->Release(); 
	hmapTex = NULL;

	return resourceView;
}


void RevModelFactory::CreateRooms( const char* aFilePath, std::vector< RevRoom* >& aRooms )
{
	myMeshFactory.LoadRooms( aFilePath, aRooms );
}
