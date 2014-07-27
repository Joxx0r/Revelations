#include "StdAfx.h"
#include "RevModel.h"
#include "RevEngineMain.h"
#include "RevEffect.h"
#include "RevAnimation.h"

int RevModel::ModelIDCounter = 0;

RevModel::RevModel(void)
:myRenderEssentials( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST )
{
	myModelID = ModelIDCounter;
	ModelIDCounter++;
	myEffect = NULL;
	myChilds.reserve( 100 );
	myIsNullObjectFlag = true;
	m_modelType = _NORMAL;
	m_bones.reserve( NR_OF_BONES );
	memset(m_instanceData,0,NR_OF_MAX_INSTANCED_INSTANCES_PER_MODEL*sizeof(RevInstanceData));
	m_animation = nullptr;

	m_modelFilePath = "TEST";
}

RevModel::~RevModel(void)
{
	delete m_animation; m_animation = nullptr;
}

bool RevModel::Init(RevVertexDataNormal& aVBData,
					RevIndexData& aIBData,
					RevInputData& aInputData,
					RevModelRenderEssentials& aEssentials )
{
	if( InitVB( aVBData ) == false ) return false;
	if( InitIB( aIBData ) == false ) return false;
	if( InitInputLayout( aInputData ) == false ) return false;
	myRenderEssentials = aEssentials;
	return true;
}


bool RevModel::SetupInstanced( )
{
	assert( m_instancedLayout.myInputLayout != nullptr );
	assert( m_instancedVB.myVertexBuffer != nullptr );

	ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
	d3dContext->IASetInputLayout( m_instancedLayout.myInputLayout );
	d3dContext->IASetPrimitiveTopology( myRenderEssentials.myTopology );

	ID3D11Buffer *vertBuffers[2] = 
	{myVB.myVertexBuffer,m_instancedVB.myVertexBuffer };

	UINT uiVertexStrides[2] = {myVB.myStride,m_instancedVB.myStride};
	UINT uOffsets[2] = {0,0};

	d3dContext->IASetVertexBuffers(		0,
		2,
		vertBuffers, 
		uiVertexStrides,
		uOffsets);

	d3dContext->IASetIndexBuffer(	myIB.myIndexBuffer, 
		myIB.myIndexBufferFormat, 																	
		myIB.myByteOffset);

	return true;
}


#pragma optimize("", off)

bool RevModel::UpdateInstancedVBBuffer(const int& nInstances, const unsigned int& size )
{
	if( m_instancedVB.myVertexBuffer != nullptr )
	{
		m_instancedVB.myVertexBuffer->Release();
		m_instancedVB.myVertexBuffer = nullptr;
	}
	D3D11_BUFFER_DESC bufferDesc =
	{
		size,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER|D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};

	HRESULT hr = RevEngineMain::GetDevice()->CreateBuffer( &bufferDesc, NULL, &m_instancedVB.myVertexBuffer );

	RevEngineMain::GetContext()->UpdateSubresource(
		m_instancedVB.myVertexBuffer,
		D3D11CalcSubresource(0,0,1),
		NULL,(void*)((byte*)m_instanceData),
		size,
		1);

	return true;
}

#pragma optimize("", off)

bool RevModel::RenderInstanced( const int& n )
{
	SetupInstanced();
	ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
	D3DX11_TECHNIQUE_DESC   techDesc;
	memset( &techDesc, 0, sizeof( D3DX11_TECHNIQUE_DESC ) );
	ID3DX11EffectTechnique* tech = myEffect->GetTechnique( RevEffect::_RENDER_INSTANCED );
	tech->GetDesc( &techDesc );
	myTextures.Render( myEffect );
	for(UINT passIndex = 0; passIndex < techDesc.Passes; passIndex++)
	{
		tech->GetPassByIndex(passIndex)->Apply(0, d3dContext );
		d3dContext->DrawIndexedInstanced(myRenderEssentials.myIndexCount, n, 0,0,0);
	}
	return true;
}

bool RevModel::Render(	CU::Matrix44f& aWorldMatrix,
						const int& aRenderTech )
{
	assert( aRenderTech > -1 &&  aRenderTech < RevEffect::_NUM_OF_TECHS );
	if( myIsNullObjectFlag == false )
	{
		ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
	
		myEffect->SetMatrixVarible( RevEffect::_WORLD, aWorldMatrix );
		
		d3dContext->IASetPrimitiveTopology( myRenderEssentials.myTopology );
		d3dContext->IASetInputLayout( myInputLayout.myInputLayout );
		d3dContext->IASetVertexBuffers( myVB.myStartSlot, myVB.myNrOfBuffers, &myVB.myVertexBuffer, &myVB.myStride, &myVB.myByteOffset );
		d3dContext->IASetIndexBuffer( myIB.myIndexBuffer, myIB.myIndexBufferFormat, myIB.myByteOffset );
		
		D3DX11_TECHNIQUE_DESC   techDesc;
		ID3DX11EffectTechnique* tech = myEffect->GetTechnique( aRenderTech );
		tech->GetDesc( &techDesc );
		myTextures.Render( myEffect );

		for(UINT passIndex = 0; passIndex < techDesc.Passes; passIndex++)
		{
			tech->GetPassByIndex(passIndex)->Apply(0, d3dContext );
			d3dContext->DrawIndexed(myRenderEssentials.myIndexCount,myRenderEssentials.myIndexStart, myRenderEssentials.myVertexStart);
		}
	}

	return true;
}

void RevModel::SetupIBVB()
{
	ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
	if (!d3dContext)
	{
		return;
	}

	d3dContext->IASetPrimitiveTopology(myRenderEssentials.myTopology);
	d3dContext->IASetInputLayout(myInputLayout.myInputLayout);
	d3dContext->IASetVertexBuffers(myVB.myStartSlot, myVB.myNrOfBuffers, &myVB.myVertexBuffer, &myVB.myStride, &myVB.myByteOffset);
	d3dContext->IASetIndexBuffer(myIB.myIndexBuffer, myIB.myIndexBufferFormat, myIB.myByteOffset);
}

#pragma optimize("", off )

bool RevModel::RenderNormalNoSetupOrChild(const int& renderTech)
{
	if (myIsNullObjectFlag == false)
	{
		ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
		if (!d3dContext)
		{
			return false;
		}

		d3dContext->IASetPrimitiveTopology(myRenderEssentials.myTopology);
		d3dContext->IASetInputLayout(myInputLayout.myInputLayout);
		d3dContext->IASetVertexBuffers(myVB.myStartSlot, myVB.myNrOfBuffers, &myVB.myVertexBuffer, &myVB.myStride, &myVB.myByteOffset);
		d3dContext->IASetIndexBuffer(myIB.myIndexBuffer, myIB.myIndexBufferFormat, myIB.myByteOffset);


		D3DX11_TECHNIQUE_DESC   techDesc;
		ID3DX11EffectTechnique* tech = myEffect->GetTechnique(renderTech);
		tech->GetDesc(&techDesc);
		myTextures.Render(myEffect);

		for (UINT passIndex = 0; passIndex < techDesc.Passes; passIndex++)
		{
			tech->GetPassByIndex(passIndex)->Apply(0, d3dContext);
			d3dContext->DrawIndexed(myRenderEssentials.myIndexCount, myRenderEssentials.myIndexStart, myRenderEssentials.myVertexStart);
		}
	}
	return true;
}

#pragma optimize("", on )


const int& RevModel::GetModelID()
{
	return myModelID;
}

const std::string& RevModel::GetModelFilePath()
{
	return m_modelFilePath;
}

void RevModel::SetModelFilePath( const std::string& filePath )
{
	m_modelFilePath = filePath;
}

void RevModel::AddChild( RevModel* aModel )
{
	myChilds.push_back( aModel );
}

void RevModel::SetModelTransformation( const CU::Matrix44f& aTransformation )
{
	myMatrix = aTransformation;
}

void RevModel::AddTexture( const char* resourceName, ID3D11ShaderResourceView* srv )
{
	myTextures.AddShaderResource( resourceName, srv );
}

void RevModel::SetIsNullObjectFlag( const bool& aState )
{
	myIsNullObjectFlag = aState;
}

const bool& RevModel::GetIsNullObjectFlag()
{
	return myIsNullObjectFlag;
}

void RevModel::SetModelTextures( const RevTextures& aTexture )
{
	myTextures = aTexture;
}

void RevModel::SetBaseModelMatrix( CU::Matrix44f& aMatrix )
{
	CU::Matrix33f m = aMatrix.Get33();
	myMatrix.SetRotation( m * myMatrix.Get33() );
}

void RevModel::SetModelType( const ModelType& modelType )
{
	m_modelType = modelType;
}

void RevModel::SetAnimation( RevAnimation* animation )
{
	m_animation = animation;
}

std::vector<RevModel*>& RevModel::GetChilds( )
{
	return myChilds;
}

RevEffect* RevModel::GetEffect()
{
	return myEffect;
}

RevInstanceData* RevModel::GetInstanceData()
{
	if( myIsNullObjectFlag == false )
	{
		return &m_instanceData[0];
	}

	for( auto childModel : myChilds )
	{
		if( childModel->GetIsNullObjectFlag() == false )
		{
			return childModel->GetInstanceData();
		}
	}

	return nullptr;
}

CU::Matrix44f& RevModel::GetModelMatrix()
{
	return myMatrix;
}

const RevModel::ModelType& RevModel::GetModelType()
{
	return m_modelType;
}

std::vector< int >& RevModel::GetBoneList()
{
	return m_boneList;
}

std::vector< RevBone >& RevModel::GetBones()
{
	return m_bones;
}

RevAnimation* RevModel::GetAnimation()
{
	return m_animation;
}

bool RevModel::InitVB( RevVertexDataNormal& aVBData )
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.ByteWidth				= aVBData.mySize;
	bufferDesc.Usage					= D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags				= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags			= 0;
	bufferDesc.MiscFlags				= 0;
	bufferDesc.StructureByteStride		= 0;

	D3D11_SUBRESOURCE_DATA bufferSubDataDesc;
	ZeroMemory(&bufferSubDataDesc, sizeof(D3D11_SUBRESOURCE_DATA));
	bufferSubDataDesc.pSysMem			= static_cast<void*>(aVBData.myVertexData);
	bufferSubDataDesc.SysMemPitch		= 0;
	bufferSubDataDesc.SysMemSlicePitch	= 0;

	HRESULT hr = E_FAIL;
	hr = RevEngineMain::GetDevice()->CreateBuffer(
		&bufferDesc, 
		&bufferSubDataDesc, 
		&myVB.myVertexBuffer);

	myVB.myStride			= aVBData.myStride;
	myVB.myByteOffset		= 0;
	myVB.myNrOfBuffers		= 1;
	myVB.myStartSlot		= 0;
	myVB.m_totalSize		= aVBData.mySize;
	myVB.myNrOfVertexes		= aVBData.myNrOfVertexes;

	m_instancedVB.myStride			= sizeof( RevInstanceData );
	m_instancedVB.myByteOffset		= 0;
	m_instancedVB.myNrOfBuffers		= 1;
	m_instancedVB.myStartSlot		= 0;
	m_instancedVB.m_totalSize		= 0;
	m_instancedVB.myNrOfVertexes	= aVBData.myNrOfVertexes;
	
	return true;
}

bool RevModel::InitIB( RevIndexData& aIBData )
{	
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
	bufferDesc.Usage						= D3D11_USAGE_DEFAULT;;
	bufferDesc.ByteWidth					= aIBData.mySize;
	bufferDesc.BindFlags					= D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags				= 0;
	bufferDesc.MiscFlags					= 0;
	bufferDesc.StructureByteStride			= 0;	

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof(D3D11_SUBRESOURCE_DATA));
	initData.pSysMem = aIBData.myIndexData;

	HRESULT hr =  RevEngineMain::GetDevice()->CreateBuffer(&bufferDesc, 
		&initData, 
		&myIB.myIndexBuffer);

	if(FAILED(hr))
	{
		return false;
	}

	myIB.myIndexBufferFormat		= aIBData.myFormat;
	myIB.myByteOffset				= 0;

	return true;

}

bool RevModel::InitInputLayout( RevInputData& aInputData )
{
	if( myEffect == NULL )
	{
		assert( 0 && "DID NOT INIT AN EFFECT TO MODEL ");
		return false;
	}

	HRESULT hr = E_FAIL;

	D3DX11_PASS_DESC   passDesc;
	hr = myEffect->GetTechnique(aInputData.m_effectTech)->GetPassByIndex(0)->GetDesc(&passDesc);
	
	if(FAILED(hr))
	{
		return false;
	}

	hr = RevEngineMain::GetDevice()->CreateInputLayout(&aInputData.myElements[0],
		aInputData.myElements.size(),
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&myInputLayout.myInputLayout);

	if(FAILED(hr))
	{
		return false;
	}

	if(!myEffect->IsTechniqueValid( aInputData.m_instancedEffectTech ) )
	{
		//does not support instancing
		return true;
	}

	D3DX11_PASS_DESC   instancedPassDesc;
	hr = myEffect->GetTechnique( aInputData.m_instancedEffectTech )->GetPassByIndex(0)->GetDesc(&instancedPassDesc);

	if(FAILED(hr))
	{
		return false;
	}

	hr = RevEngineMain::GetDevice()->CreateInputLayout(&aInputData.m_instancedElements[0],
		aInputData.m_instancedElements.size(),
		instancedPassDesc.pIAInputSignature,
		instancedPassDesc.IAInputSignatureSize,
		&m_instancedLayout.myInputLayout);

	if(FAILED(hr))
	{
		return false;
	}

	return true;
}

bool RevModel::InitShader( const char* aFilePath )
{
	myEffect = RevEngineMain::GetEffectContainer().GetEffect( aFilePath );
	return myEffect != NULL;
}