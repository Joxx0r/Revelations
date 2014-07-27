#ifndef _STRUCTURES_H
#define _STRUCTURES_H

#include "CU_Vector.h"
#include "CU_Matrix.h"
#include "RevEffect.h"
#include <d3d11.h>
#include <vector>


struct RevVertexBuffer
{
	RevVertexBuffer() 
	{ 
		myVertexBuffer = NULL; 
		myStartSlot = 0;
		myNrOfBuffers = 0;
		myStride = 0;
		myByteOffset = 0;
		myNrOfVertexes = 0; 
		m_totalSize = 0;
	}
	~RevVertexBuffer() { if(myVertexBuffer) myVertexBuffer->Release(); }
	ID3D11Buffer* myVertexBuffer;
	int myStartSlot;
	int myNrOfBuffers;
	unsigned int myStride;
	unsigned int myByteOffset;
	int myNrOfVertexes;
	int m_totalSize;

};

struct RevIndexData
{
	RevIndexData() { myIndexData = NULL; }
	char* myIndexData;
	int myNrOfIndexes;
	int mySize;
	DXGI_FORMAT myFormat;
};

struct RevIndexBuffer
{
	RevIndexBuffer() { myIndexBuffer = NULL; myByteOffset = 0; myIndexBufferFormat = DXGI_FORMAT_UNKNOWN; }
	~RevIndexBuffer() { if(myIndexBuffer) myIndexBuffer->Release(); }
	ID3D11Buffer* myIndexBuffer;
	DXGI_FORMAT myIndexBufferFormat;
	int myByteOffset;

};

struct RevInputData
{
	RevInputData() 
	{ 
		m_effectTech = RevEffect::_NORMAL;  
		m_instancedEffectTech = RevEffect::_RENDER_INSTANCED;
	};

	~RevInputData() 
	{  
	};

	std::vector< D3D11_INPUT_ELEMENT_DESC > myElements;
	std::vector< D3D11_INPUT_ELEMENT_DESC > m_instancedElements;
	RevEffect::EffectTech m_effectTech;
	RevEffect::EffectTech m_instancedEffectTech;
};

struct RevInputLayout
{
	RevInputLayout() { myInputLayout = NULL; }
	~RevInputLayout() { if(myInputLayout) myInputLayout->Release(); }

	ID3D11InputLayout* myInputLayout;
};

struct RevVertexPosCol
{
	CU::Vector3f	myPos;
	CU::Vector4f	myColor;
};

struct RevVertexPosUVIndex
{
	CU::Vector3f myPos;
	CU::Vector2f myUV;
	float m_index;
};

struct RevVertexPosUV
{
	CU::Vector3f myPos;
	CU::Vector2f myUV;
};

struct RevVertexPos
{
	CU::Vector3f myPos;
};

struct RevPointLight
{
	CU::Vector4f	myPos;
	CU::Vector4f	myColor;
	float			myRadius;
};


struct RevVertexPosUVNormal
{
	CU::Vector3f myPos;
	CU::Vector2f myUV;
	CU::Vector3f myNormal;
};

struct RevVertexPosColorUV
{
	CU::Vector3f myPos;
	CU::Vector4f myColor;
	CU::Vector2f myUV;
};

struct RevVertexPosNormUVTan
{
	CU::Vector3f myPos;
	CU::Vector3f myNormal;
	CU::Vector2f myUV;
	CU::Vector3f myTan;
};

struct RevVertexPosNormUVTanBi
{
	CU::Vector3f myPos;
	CU::Vector3f myNormal;
	CU::Vector2f myUV;
	CU::Vector3f myTan;
	CU::Vector3f myBi;
};

struct RevVertexPosColNormBiTan
{
	CU::Vector3f myPos;
	CU::Vector4f myColor;
	CU::Vector3f myNormal;
	CU::Vector3f myBitan;
	CU::Vector3f myTangent;

};

struct RevVertexPosWeightBonesNormUV
{
	CU::Vector3f myPos;
	unsigned char myWeights[4];
	unsigned char myBones[4];
	CU::Vector3f myNormal;
	CU::Vector2f myUV;
};

struct RevVertexPosWeightBonesNormUVTan
{
	CU::Vector3f myPos;
	unsigned char myWeights[4];
	unsigned char myBones[4];
	CU::Vector3f myNormal;
	CU::Vector2f myUV;
	CU::Vector3f myTan;
};


struct RevVertexPosWeightBonesNormUVTanBi
{
	CU::Vector3f myPos;
	unsigned char myWeights[4];
	unsigned char myBones[4];
	CU::Vector3f myNormal;
	CU::Vector2f myUV;
	CU::Vector3f myTan;
	CU::Vector3f myBi;
};

struct RevVertexPosColNorm
{
	CU::Vector3f myPos;
	CU::Vector4f myColor;
	CU::Vector3f myNorm;
};


struct RevModelRenderEssentials
{
	RevModelRenderEssentials( D3D_PRIMITIVE_TOPOLOGY aTopology )
	{
		myTopology		= aTopology;
		myIndexCount	= 0;
		myIndexStart	= 0;
		myVertexStart	= 0;
	}

	int myIndexCount;
	int myIndexStart;
	int myVertexStart;
	D3D_PRIMITIVE_TOPOLOGY myTopology;
};

struct RevCameraData
{
	CU::Matrix44f	myViewMatrix;
	CU::Matrix44f	myProjectionMatrix;
	float			myZFar;
	float			myZNear;
	CU::Vector4f	myFrustumCorners[ 4 ];
};

const D3D11_INPUT_ELEMENT_DESC VertexPosColLayout[] = 
{ 
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	12, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosUV[] = 
{ 
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosColNormBiTanLayout[] = 
{ 
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	12, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	28, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BITAN",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	40, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	52, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosColNormLayout[] = 
{ 
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 28,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosLayout[] = 
{ 
	{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InstancedVertexLayout[] =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,			0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,			0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,			0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,			0, 32,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BINORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT,			0, 44,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "WORLD",	    0, DXGI_FORMAT_R32G32B32A32_FLOAT,		1, 0,	D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",	    1, DXGI_FORMAT_R32G32B32A32_FLOAT,		1, 16,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	{ "WORLD",      2, DXGI_FORMAT_R32G32B32A32_FLOAT,		1, 32,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
};

struct RevVertexData
{
	RevVertexData( const int& aNrOfVerts )
		:myNrOfVertexes( aNrOfVerts)
	{ myVertexData = NULL; }

	RevVertexPosColNormBiTan* myVertexData;
	const int myNrOfVertexes;
	int mySize;
	int myStride;
};

struct RevVertexDataIndex
{
	RevVertexDataIndex( const int& aNrOfVerts )
		:myNrOfVertexes( aNrOfVerts)
	{ myVertexData = NULL; }

	char* myVertexData;
	const int myNrOfVertexes;
	int mySize;
	int myStride;
};


struct RevVertexDataNormal
{
	RevVertexDataNormal( const int& aNrOfVerts )
		:myNrOfVertexes( aNrOfVerts)
	{ 
		myVertexData = nullptr; 
		mySize = 0;
		myStride = 0;
	}

	char* myVertexData;
	int myNrOfVertexes;
	int mySize;
	int myStride;
};

struct RevVertexIndexmapKey
{
	int myKeys[8];

	RevVertexIndexmapKey()
	{
		for(int i=0;i<8;i++)
		{
			myKeys[i]=-1;
		}
	}


	bool operator!=(const RevVertexIndexmapKey& aRightKey)
	{
		for(int i=0;i<8;i++)
		{
			if(myKeys[i]!=aRightKey.myKeys[i]) return(true);
		}
		return(false);
	}


	bool operator==(const RevVertexIndexmapKey& aRightKey)
	{
		for(int i=0;i<8;i++)
		{
			if(myKeys[i]!=aRightKey.myKeys[i]) return(false);
		}
		return(true);
	}
};

enum RevAnimationType
{
	ABSOLUTE_TRANSFORM,
	RELATIVE_TRANSFORM
};


#endif