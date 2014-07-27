#include "stdafx.h"
#include "RevTexture.h"
#include "RevEngineMain.h"

#include <d3dx11.h>
#pragma optimize("", off )

DXGI_FORMAT MAKE_SRGB( DXGI_FORMAT format )
{
	/*    if( !DXUTIsInGammaCorrectMode() )
	return format;*/

	switch( format )
	{
	case DXGI_FORMAT_R8G8B8A8_TYPELESS:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	case DXGI_FORMAT_BC1_TYPELESS:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case DXGI_FORMAT_BC2_TYPELESS:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case DXGI_FORMAT_BC3_TYPELESS:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_UNORM_SRGB;

	};

	return format;
}

DXGI_FORMAT MAKE_TYPELESS( DXGI_FORMAT format )
{
	/*    if( !DXUTIsInGammaCorrectMode() )
	return format;
	*/
	switch( format )
	{
	case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
	case DXGI_FORMAT_R8G8B8A8_UINT:
	case DXGI_FORMAT_R8G8B8A8_SNORM:
	case DXGI_FORMAT_R8G8B8A8_SINT:
		return DXGI_FORMAT_R8G8B8A8_TYPELESS;

	case DXGI_FORMAT_BC1_UNORM_SRGB:
	case DXGI_FORMAT_BC1_UNORM:
		return DXGI_FORMAT_BC1_TYPELESS;
	case DXGI_FORMAT_BC2_UNORM_SRGB:
	case DXGI_FORMAT_BC2_UNORM:
		return DXGI_FORMAT_BC2_TYPELESS;
	case DXGI_FORMAT_BC3_UNORM_SRGB:
	case DXGI_FORMAT_BC3_UNORM:
		return DXGI_FORMAT_BC3_TYPELESS;
	};

	return format;
}


RevTexture::RevTexture(void)
{
}

RevTexture::RevTexture(int aWidth,
					   int aHeight,
					   int aFormat,
					   bool aShaderResource,
					   bool aRenderTarget,
					   bool asUAV)
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width     = aWidth;
	texDesc.Height    = aHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = (DXGI_FORMAT)aFormat;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage     = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags =0;
	if(aShaderResource==true)
	{
		texDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	if(aRenderTarget==true)
	{
		texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	if(asUAV==true)
	{
		texDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags      = 0;


	ID3D11Texture2D* texture;

	HRESULT hr;

	hr=RevEngineMain::GetDevice()->CreateTexture2D(&texDesc, 0, &texture);
	if( FAILED( hr ) )
	{
		return;
	}

	if(aRenderTarget==true)
	{

		D3D11_RENDER_TARGET_VIEW_DESC RTDesc;

		RTDesc.Format = texDesc.Format;
		RTDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		RTDesc.Texture2D.MipSlice = 0;
		hr=RevEngineMain::GetDevice()->CreateRenderTargetView(texture, &RTDesc, &myRenderTargetView);
		if( FAILED( hr ) )
		{
			return;
		}
	}
	else
	{
		myRenderTargetView=NULL;
	}


	if(aShaderResource==true)
	{

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = 1;
		hr=RevEngineMain::GetDevice()->CreateShaderResourceView(texture, &srvDesc, &myShaderResourceView);
		if( FAILED( hr ) )
		{
			return;
		}
	}
	else
	{
		myShaderResourceView=NULL;
	}

	if(asUAV==true)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		uavDesc.Format = texDesc.Format;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;
		hr=RevEngineMain::GetDevice()->CreateUnorderedAccessView(texture, &uavDesc, &myUnorderedAcessView);
		if( FAILED( hr ) )
		{
			return;
		}
	}
	else
	{
		myUnorderedAcessView=NULL;
	}

	texture->Release();

}

RevTexture::~RevTexture(void)
{
}

bool RevTexture::LoadTexture( const char* aTextureName,bool aUseSRGBFlag)
{
	return LoadTextureAsShaderResourceView( aTextureName, aUseSRGBFlag );
}

ID3D11ShaderResourceView* RevTexture::GetSRV()
{
	return myShaderResourceView;
}

ID3D11RenderTargetView* RevTexture::GetRTV()
{
	return myRenderTargetView;
}

ID3D11RenderTargetView** RevTexture::GetRTVP()
{
	return &myRenderTargetView;
}

CU::Vector2f RevTexture::GetImageSize()
{
	CU::Vector2f imageSize( myImageInfo.Width, myImageInfo.Height );
	return imageSize;
}

bool RevTexture::SetupLoadingData(	D3DX11_IMAGE_LOAD_INFO& someInfo,
						D3DX11_IMAGE_INFO& sourceInfo, 
						const char* aFileName)
{
	HRESULT result = D3DX11GetImageInfoFromFile( aFileName, NULL, &sourceInfo, &result );

	someInfo.pSrcInfo = &sourceInfo;
	someInfo.Format = sourceInfo.Format;
	return !(FAILED(result));
}

bool RevTexture::CompensateForSRGB(	D3DX11_IMAGE_LOAD_INFO& someInfo,
						ID3D11Texture2D*& aTexture)
{
	if( someInfo.pSrcInfo->ResourceDimension == D3D10_RESOURCE_DIMENSION_TEXTURE2D )
	{
		HRESULT hr = S_OK;
		ID3D11Texture2D* copyTexure = NULL;
		D3D11_TEXTURE2D_DESC copyDescription;
		aTexture->GetDesc( &copyDescription );
		copyDescription.Format = MAKE_TYPELESS( copyDescription.Format );
		hr = RevEngineMain::GetDevice()->CreateTexture2D( &copyDescription, NULL, &copyTexure );
		if( FAILED( hr ) )
			return false;

		RevEngineMain::GetContext()->CopyResource( copyTexure, aTexture );
		aTexture->Release();
		aTexture = copyTexure;
		return(true);
	}
	return(false);
}

bool RevTexture::BindTextureToView(	D3DX11_IMAGE_LOAD_INFO& someInfo,
						ID3D11Texture2D*& aTexture,
						const bool& aUseSRGBFlag)
{
	HRESULT hr = S_OK;


	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResViewDescription;
	if( aUseSRGBFlag)
		shaderResViewDescription.Format = MAKE_SRGB( someInfo.Format );
	else
		shaderResViewDescription.Format = someInfo.Format;

	if( someInfo.pSrcInfo->ResourceDimension == D3D11_RESOURCE_DIMENSION_TEXTURE2D )
	{
		shaderResViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		shaderResViewDescription.Texture2D.MostDetailedMip = 0;
		shaderResViewDescription.Texture2D.MipLevels = someInfo.pSrcInfo->MipLevels;

		if( someInfo.pSrcInfo->MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE )
		{
			shaderResViewDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
		}
	}
	else
	{
		assert( 0 && "INVALID TEXTURE FORMAT");
		return false;
		//assert(0 && "INVALID TEXTURE FORMAT");
	}

	hr = RevEngineMain::GetDevice()->CreateShaderResourceView( aTexture, &shaderResViewDescription, &myShaderResourceView );
	aTexture->Release();
	if( FAILED( hr ) )
		return false;
	return(true);


}
bool RevTexture::LoadTextureAsShaderResourceView(
									const char* aTextureName, 
									const bool&  aUseSRGBFlag)
{
	
	D3DX11_IMAGE_LOAD_INFO loadingInfo;
	HRESULT hr = S_OK;

	bool srgbFlag = aUseSRGBFlag;

	SetupLoadingData(loadingInfo,myImageInfo,aTextureName);

	ID3D11Texture2D* currentTexture = NULL;
	hr = D3DX11CreateTextureFromFile( RevEngineMain::GetDevice(), aTextureName, &loadingInfo, NULL, reinterpret_cast<ID3D11Resource**>(&currentTexture), NULL );
	if( FAILED( hr ) )
		return false;

	if(srgbFlag)
	{
		if(CompensateForSRGB(loadingInfo,currentTexture)==false)
		{
			srgbFlag=false;
		}
	}

	if(BindTextureToView(loadingInfo,currentTexture,srgbFlag)==false)
	{
		return(false);
	}
	return(true);
}

#pragma optimize("", on )