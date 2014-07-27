#ifndef _REV_TEXTURE_H
#define _REV_TEXTURE_H

struct ID3D11ShaderResourceView;
struct ID3D11UnorderedAccessView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;

struct D3DX11_IMAGE_LOAD_INFO;
struct D3DX11_IMAGE_INFO;

#include "d3dx11tex.h"

class RevTexture
{
public:
	RevTexture(void);
	RevTexture(int aWidth,
					   int aHeight,
					   int aFormat,
					   bool aShaderResource,
					   bool aRenderTarget,
					   bool asUAV);
	~RevTexture(void);
	bool LoadTexture( const char* aTextureName,bool aUseSRGBFlag);

	ID3D11ShaderResourceView* GetSRV();
	ID3D11RenderTargetView* GetRTV();
	ID3D11RenderTargetView** GetRTVP();

	CU::Vector2f GetImageSize();

private:
	bool SetupLoadingData(D3DX11_IMAGE_LOAD_INFO& someInfo,D3DX11_IMAGE_INFO& sourceInfo, const char* aFileName);
	bool CompensateForSRGB(D3DX11_IMAGE_LOAD_INFO& someInfo,ID3D11Texture2D*& aTexture);
	bool BindTextureToView(D3DX11_IMAGE_LOAD_INFO& someInfo,ID3D11Texture2D*& aTexture,const bool& aUseSRGBFlag);
	bool LoadTextureAsShaderResourceView(const char* aTextureName,const bool& aUseSRGBFlag);

	ID3D11ShaderResourceView* myShaderResourceView;
	ID3D11RenderTargetView* myRenderTargetView;
	ID3D11UnorderedAccessView* myUnorderedAcessView;
	D3DX11_IMAGE_INFO myImageInfo;
};

#endif
