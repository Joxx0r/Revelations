#include "stdafx.h"
#include "RevRendererDeferedHelper.h"
#include "RevTexture.h"
#include "RevEngineMain.h"
#include "RevModel.h"

//Temp	
#include "RevEngineMain.h"

RevRendererDeferedHelper::RevRendererDeferedHelper(void)
{
	myAlbedoTexture = NULL;
	myDepthTexture	= NULL;
	myNormalTexture	= NULL;
	myDSV			= NULL;
	m_blendState = nullptr;
}


RevRendererDeferedHelper::~RevRendererDeferedHelper(void)
{
	delete myAlbedoTexture;
	delete myDepthTexture;
	delete myNormalTexture;
	delete m_blendState;
}

bool RevRendererDeferedHelper::Init()
{
	ID3D11Device* device = RevEngineMain::GetDevice();
	if (!device)
	{
		return false;
	}

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	D3D11_RENDER_TARGET_BLEND_DESC& rtvBlend = BlendState.RenderTarget[0];
	rtvBlend.BlendEnable = false;
	device->CreateBlendState(&BlendState, &m_blendState);

	return true;
}

bool RevRendererDeferedHelper::InitRTVSAndDSV( const float& aWidth, const float& aHeight )
{

	myWidth		= aWidth;
	myHeight	= aHeight;

	myNormalTexture = new RevTexture( aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM, true, true, false );
	myDepthTexture = new RevTexture( aWidth, aHeight, DXGI_FORMAT_R32_FLOAT, true, true, false );
	myAlbedoTexture = new RevTexture( aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, true, true, false );

	HRESULT result = E_FAIL;

	ID3D11Texture2D* dsvTex = NULL;

	D3D11_TEXTURE2D_DESC depthBufferInfo;
	ZeroMemory(&depthBufferInfo, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferInfo.Width				= (UINT)aWidth;
	depthBufferInfo.Height				= (UINT)aHeight;
	depthBufferInfo.MipLevels			= 1;
	depthBufferInfo.ArraySize			= 1;
	depthBufferInfo.Format				= DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferInfo.SampleDesc.Count	= 1;
	depthBufferInfo.SampleDesc.Quality	= 0;

	depthBufferInfo.Usage				= D3D11_USAGE_DEFAULT;
	depthBufferInfo.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferInfo.CPUAccessFlags		= 0;
	depthBufferInfo.MiscFlags			= 0;


	result = RevEngineMain::GetDevice()->CreateTexture2D(	&depthBufferInfo, 
		NULL,
		&dsvTex);
	if(result)
	{

		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewInfo;
	ZeroMemory(	&stencilViewInfo,
		sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));


	stencilViewInfo.Format					= DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencilViewInfo.ViewDimension			= D3D11_DSV_DIMENSION_TEXTURE2D;
	result = RevEngineMain::GetDevice()->CreateDepthStencilView( dsvTex, &stencilViewInfo, &myDSV);

	if(FAILED(result))
	{
		dsvTex->Release();
		return false;
	}

	dsvTex->Release();


	return true;	
}

void RevRendererDeferedHelper::Render( RevRenderMesh& aRenderMesh )
{
	RenderInstancedInstances( aRenderMesh );
}

bool RevRendererDeferedHelper::Setup( )
{
	ID3D11RenderTargetView* rtv[] = {  
		myAlbedoTexture->GetRTV(), myDepthTexture->GetRTV(), myNormalTexture->GetRTV() };
	float clearColor[] = { 0,0,0, 1 };
	float clearDepthColor[] = { 1,1,1, 1 };
	float normalClearColor[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	ID3D11DeviceContext* d3dContext = RevEngineMain::GetContext();
	d3dContext->ClearRenderTargetView( rtv[0], clearColor ); //Albedo
	d3dContext->ClearRenderTargetView( rtv[1], clearDepthColor ); //Depth
	d3dContext->ClearRenderTargetView( rtv[2], normalClearColor ); //Normal
	d3dContext->ClearDepthStencilView( myDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f );
	RevEngineMain::GetEffectContainer().SetCameraMatrixes( myCameraData );
	d3dContext->OMSetRenderTargets( 3, rtv, myDSV );

	D3D11_VIEWPORT port;
	ZeroMemory(&port, sizeof(  D3D11_VIEWPORT ) );
	port.Width = myWidth;
	port.Height = myHeight;
	port.MinDepth = 0.0f;
	port.MaxDepth = 1.0f;
	port.TopLeftX = 0;
	port.TopLeftY = 0;

	float blendFactor[4] = { 0.0f,0.0f, 0.0f, 0.0f };
	UINT sampleMask = 0xffffffff;

	RevEngineMain::GetContext()->RSSetViewports( 1, &port );
	RevEngineMain::GetContext()->OMSetBlendState(m_blendState, blendFactor, sampleMask);

	return true;
}

void RevRendererDeferedHelper::SetTexturesToShader( )
{
	RevEngineMain::GetEffectContainer().SetTexture( "g_PixelDiffuseTex", myAlbedoTexture->GetSRV() );
	RevEngineMain::GetEffectContainer().SetTexture( "g_PixelNormalTex", myNormalTexture->GetSRV() );
	RevEngineMain::GetEffectContainer().SetTexture( "g_PixelDepth", myDepthTexture->GetSRV() );
}