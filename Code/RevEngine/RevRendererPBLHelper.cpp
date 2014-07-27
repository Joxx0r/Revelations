#include "stdafx.h"
#include "RevRendererPBLHelper.h"
#include "RevTexture.h"

RevRendererPBLHelper::RevRendererPBLHelper()
{
	m_render = nullptr;
}

RevRendererPBLHelper::~RevRendererPBLHelper()
{
	delete m_render; m_render = nullptr;
}

bool RevRendererPBLHelper::Init()
{
	m_render = new RevTexture((int)RevEngineMain::GetWindowWidth(), (int)RevEngineMain::GetWindowHeight(), DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, true, true, false);
}

void RevRendererPBLHelper::Render(RevRenderMesh& aRenderMesh)
{

}

bool RevRendererPBLHelper::Setup()
{
	ID3D11DeviceContext* context = RevEngineMain::GetContext();
	if (!context || !m_render || m_dsv )
	{
		return false;
	}

	ID3D11RenderTargetView* rtv = m_render->GetRTV();
	if (!rtv)
	{
		return false;
	}

	context->OMSetRenderTargets(0, 0, 0);
	RevEngineMain::ClearTex(m_render);
	context->OMSetRenderTargets(1, &rtv, m_dsv);

	return true;
}
