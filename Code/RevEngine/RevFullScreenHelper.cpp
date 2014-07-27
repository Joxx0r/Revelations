#include "stdafx.h"
#include "RevFullScreenHelper.h"
#include "RevEngineMain.h"
#include "RevEffect.h"
#include "RevEffectContainer.h"
#include <d3dx11.h>
#include <d3dx11effect.h>

const D3D11_INPUT_ELEMENT_DESC postProcessLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC NewPostProcessLayout[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "INDEX", 0, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

#pragma optimize("", off )

RevFullScreenHelper::RevFullScreenHelper()
{
	myEffect = NULL;
	myIsUseCustomViewport = false;
}

RevFullScreenHelper::~RevFullScreenHelper()
{
	myVertexLayout->Release();
}

bool RevFullScreenHelper::Init( )
{
	myEffect = RevEngineMain::GetEffectContainer().GetEffect("Shaders\\FullScreen.txt", true);
	assert( myEffect != NULL );
	InitVertexBuffer();
	return(true);
}

void RevFullScreenHelper::ProcessSmall(std::string aTechniqueName)
{

	// Setup the viewport to match the backbuffer

	ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
	d3dContext->IASetInputLayout(myVertexLayout);
	d3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	d3dContext->IASetVertexBuffers( myVB.myStartSlot,myVB.myNrOfBuffers , &(myVB.myVertexBuffer), &myVB.myStride, &myVB.myByteOffset );



	//CU::Vector2f invTargetSize(1.0f/aTargetSize.x,1.0f/aTargetSize.y);
	//CU::Vector2f invSourceSize(1.0f/aSourceSize.x,1.0f/aSourceSize.y);

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* tech = myEffect->GetTechniqueByName(aTechniqueName.c_str());
	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		tech->GetPassByIndex( p )->Apply( 0, RevEngineMain::GetContext() );
		RevEngineMain::GetContext()->Draw( 4,0);
	}

}


bool RevFullScreenHelper::InitVertexBuffer()
{
	RevVertexDataIndex data( 4 );
	data.myStride=sizeof(RevVertexPosUVIndex);
	data.mySize=data.myStride*data.myNrOfVertexes;
	data.myVertexData = new char[data.mySize] ();

	RevVertexPosUVIndex quad[4];

	quad[0].myPos.Set(-1.0f, 1.0f, 0.5f);	
	quad[0].myUV.Set( 0.0f, 0.0f );
	quad[0].m_index = 0.0f;

	quad[1].myPos.Set( 1.0f, 1.0f, 0.5f);
	quad[1].myUV.Set( 1.0f, 0.0f );
	quad[1].m_index = 3.0f;

	quad[2].myPos.Set( -1.0f, -1.0f, 0.5f);
	quad[2].myUV.Set( 0.0f, 1.0f );
	quad[2].m_index = 1.0f;

	quad[3].myPos.Set( 1.0f, -1.0f, 0.5f);
	quad[3].myUV.Set( 1.0f, 1.0f );
	quad[3].m_index = 2.0f;

	memcpy(data.myVertexData,quad,data.mySize);

	D3DX11_PASS_DESC passDescription;
	HRESULT hr;


	myEffect->GetTechnique(RevEffect::_NORMAL)->GetPassByIndex( 0 )->GetDesc( &passDescription );

	hr = RevEngineMain::GetDevice()->CreateInputLayout(&NewPostProcessLayout[0], ARRAYSIZE( NewPostProcessLayout ), passDescription.pIAInputSignature,
		passDescription.IAInputSignatureSize, &myVertexLayout );
	if( FAILED( hr ) )
		return false;		

	// Set the input layout

	RevEngineMain::GetContext()->IASetInputLayout( myVertexLayout );


	D3D11_BUFFER_DESC vertexBufferDescription;
	vertexBufferDescription.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDescription.ByteWidth = data.mySize;
	vertexBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDescription.CPUAccessFlags = 0;
	vertexBufferDescription.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferInitData;
	vertexBufferInitData.pSysMem =static_cast<void*>(data.myVertexData);

	hr = RevEngineMain::GetDevice()->CreateBuffer( &vertexBufferDescription, &vertexBufferInitData, &(myVB.myVertexBuffer) );
	if( FAILED( hr ) )
	{
		delete [] data.myVertexData;
		return false;
	}

	myVB.myByteOffset=0;
	myVB.myNrOfBuffers=1;
	myVB.myStartSlot=0;
	myVB.myStride=data.myStride;

	return(true);
}

void RevFullScreenHelper::Process(std::string aTechniqueName,ID3D11RenderTargetView* aTarget,int aWidth,int aHeight,ID3D11ShaderResourceView* aSource, int a2Width, int a2Height)
{
	RenderQuad(aTechniqueName,aTarget,CU::Vector2i(aWidth,aHeight),aSource,CU::Vector2i(a2Width,a2Height));
}

void RevFullScreenHelper::Process(std::string aTechniqueName,ID3D11RenderTargetView* aTarget,int aWidth,int aHeight )
{
	RenderQuad(aTechniqueName,aTarget,CU::Vector2i(aWidth,aHeight),nullptr,CU::Vector2i(0,0));
}

void RevFullScreenHelper::SetViewPort(	const float& aX, 
										const float& aY,
										const float& aTopX, 
										const float& aTopY )
{
	D3D11_VIEWPORT vp;
	vp.Width	= aX;
	vp.Height	= aY;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = aTopX;
	vp.TopLeftY = aTopY;

	myViewPort = vp;
	myIsUseCustomViewport = true;
}

RevEffect* RevFullScreenHelper::GetEffect( )
{
	assert( myEffect != NULL );
	return myEffect;
}

void RevFullScreenHelper::RenderQuad(std::string aTechniqueName,ID3D11RenderTargetView* aTarget,CU::Vector2i aTargetSize,ID3D11ShaderResourceView* aSource,CU::Vector2i aSourceSize)
{
	D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
	UINT nViewPorts = 1;
	RevEngineMain::GetContext()->RSGetViewports( &nViewPorts, vpOld );

	// Setup the viewport to match the backbuffer
	if(myIsUseCustomViewport==true)
	{
		RevEngineMain::GetContext()->RSSetViewports( 1, &myViewPort );
		myIsUseCustomViewport = false;
	}
	else
	{
		D3D11_VIEWPORT vp;
		vp.Width =(float)aTargetSize.x;
		vp.Height = (float)aTargetSize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		RevEngineMain::GetContext()->RSSetViewports( 1, &vp );
	}

	RevEngineMain::GetContext()->IASetInputLayout(myVertexLayout);
	RevEngineMain::GetContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	RevEngineMain::GetContext()->IASetVertexBuffers( myVB.myStartSlot,myVB.myNrOfBuffers , &(myVB.myVertexBuffer), &myVB.myStride, &myVB.myByteOffset );


	CU::Vector2f invTargetSize(1.0f/aTargetSize.x,1.0f/aTargetSize.y);
	CU::Vector2f invSourceSize(1.0f/aSourceSize.x,1.0f/aSourceSize.y);

	RevEngineMain::GetContext()->OMSetRenderTargets( 1, &aTarget, NULL);
	if( aSource != NULL )
	{
		myEffect->GetEffect()->GetVariableByName("g_FullScreenColorTex")->AsShaderResource()->SetResource(aSource);
	}
	
	myEffect->GetEffect()->GetVariableByName("g_InvTargetSize")->AsVector()->SetRawValue(&invTargetSize,0,sizeof(CU::Vector2f));
	myEffect->GetEffect()->GetVariableByName("g_InvSourceSize")->AsVector()->SetRawValue(&invSourceSize,0,sizeof(CU::Vector2f));

	D3DX11_TECHNIQUE_DESC techDesc;
	ID3DX11EffectTechnique* tech = myEffect->GetTechniqueByName( aTechniqueName.c_str() );
	assert( tech->IsValid() == true );
	tech->GetDesc( &techDesc );
	for( UINT p = 0; p < techDesc.Passes; ++p )
	{
		tech->GetPassByIndex( p )->Apply( 0, RevEngineMain::GetContext() );
		RevEngineMain::GetContext()->Draw( 4,0);
	}

	RevEngineMain::GetContext()->RSSetViewports( nViewPorts, vpOld );

	RevEngineMain::ResetOMDS();
}

#pragma optimize("", on )