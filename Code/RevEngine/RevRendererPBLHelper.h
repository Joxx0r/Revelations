#pragma once

#include "RevRenderBaseHelper.h"

class RevTexture;

class RevRendererPBLHelper : public RevRenderBaseHelper
{
public:
	RevRendererPBLHelper();
	~RevRendererPBLHelper();

	virtual bool Init();
	virtual void Render(RevRenderMesh& aRenderMesh);
	virtual bool Setup();

	inline void SetCurrentDepth(ID3D11DepthStencilView* dsv) { m_dsv = dsv;  }
	RevTexture* GetOutputRender() { return m_render; }

private:
	RevTexture* m_render;
	ID3D11DepthStencilView* m_dsv;
};

