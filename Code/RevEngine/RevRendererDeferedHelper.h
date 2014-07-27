#ifndef _REV_RENDER_DEFERED_HELPER_H
#define _REV_RENDER_DEFERED_HELPER_H

#include "RevRenderBaseHelper.h"

class RevTexture;
	
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11BlendState;

class RevRendererDeferedHelper  : public RevRenderBaseHelper
{
public:
	RevRendererDeferedHelper(void);
	~RevRendererDeferedHelper(void);
	virtual bool Init();
	bool InitRTVSAndDSV( const float& aWidth, const float& aHeight );
	virtual void Render( RevRenderMesh& aRenderMesh );
	virtual bool Setup( );
	void SetTexturesToShader( );
	RevTexture* GetAlbedoTexture() { return myAlbedoTexture; }
	RevTexture* GetDepthTexture() { return myDepthTexture; }
	RevTexture* GetNormalTexture() { return myNormalTexture; }

private:

	RevTexture* myAlbedoTexture;
	RevTexture* myDepthTexture;
	RevTexture* myNormalTexture;

	float		myWidth;
	float		myHeight;

	ID3D11DepthStencilView* myDSV;

	ID3D11BlendState* m_blendState;

};

#endif