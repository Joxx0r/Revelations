#pragma once

#include "RevRenderBaseHelper.h"

class RevTexture;
struct ID3D11DepthStencilView;
struct ID3D11BlendState;

class RevRenderLightHelper : public RevRenderBaseHelper
{
public:
	RevRenderLightHelper();
	~RevRenderLightHelper();

	virtual bool Init();
	virtual void RenderNoMesh();
	virtual bool Setup();

	RevTexture* GetLightTexture() 
	{ return m_renderedTexture;  }

	inline void SetDepthTexture( ID3D11DepthStencilView* dsv )
	{ 
		m_dsv = dsv;
	}

private:
	RevTexture*				m_renderedTexture;
	RevModel*				m_pointLightModel;
	ID3D11DepthStencilView*	m_dsv;

	ID3D11BlendState* m_blendState;

};

