#include "stdafx.h"
#include "RevRenderLightHelper.h"
#include "RevTexture.h"
#include "RevLightContainer.h"
#include "RevEffect.h"
#include "RevModel.h"

RevRenderLightHelper::RevRenderLightHelper()
{
	m_renderedTexture = nullptr;
	m_pointLightModel = nullptr;
	m_dsv = nullptr;
	m_blendState = nullptr;
}

RevRenderLightHelper::~RevRenderLightHelper()
{
	delete m_renderedTexture; m_renderedTexture = nullptr;
	m_blendState->Release(); 
}

bool RevRenderLightHelper::Init()
{
	m_pointLightModel = RevEngineMain::GetModelFactory().GetSphereSliceDefered();

	m_renderedTexture = new RevTexture(
		RevEngineMain::GetWindowWidth(),
		RevEngineMain::GetWindowHeight(), 
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		true, 
		true,
		false);

	ID3D11Device* device = RevEngineMain::GetDevice();
	if (!device)
	{
		return false;
	}

	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	D3D11_RENDER_TARGET_BLEND_DESC& rtvBlend = BlendState.RenderTarget[0];
	rtvBlend.BlendEnable = true;
	rtvBlend.SrcBlend = D3D11_BLEND_ONE;
	rtvBlend.DestBlend = D3D11_BLEND_ONE;
	rtvBlend.BlendOp = D3D11_BLEND_OP_ADD;
	rtvBlend.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtvBlend.DestBlendAlpha = D3D11_BLEND_ONE;
	rtvBlend.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtvBlend.RenderTargetWriteMask = 0x0F;
	device->CreateBlendState(&BlendState, &m_blendState);

	return true;
}

void RevRenderLightHelper::RenderNoMesh()
{
	if (!m_pointLightModel)
	{
		return;
	}

	RevEffect* shader = m_pointLightModel->GetEffect();
	if (!shader)
	{
		return;
	}

	ID3D11DeviceContext* context = RevEngineMain::GetContext();	
	if (!context)
	{
		return;
	}

	if (!m_dsv)
	{
		return;
	}

	RevLightContainer& lightContainer = RevEngineMain::GetLightContainer();
	std::vector< RevPointLight >& pLights = lightContainer.GetPointLights();
	const int n = pLights.size();
	const CU::Vector2f texSize = m_renderedTexture->GetImageSize();

	float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	UINT sampleMask = 0xffffffff;
	context->OMSetBlendState(m_blendState, blendFactor, sampleMask);

	m_pointLightModel->SetupIBVB();

	for (auto pIndex = 0; pIndex < n; pIndex++)
	{
		shader->SetIntVarible(RevEffect::_LIGHT_INDEX, pIndex);
		m_pointLightModel->RenderNormalNoSetupOrChild(RevEffect::_PBL_POINT_LIGHT);
	}
}

bool RevRenderLightHelper::Setup()
{
	if (!m_pointLightModel)
	{
		return false;
	}

	ID3D11DeviceContext* context = RevEngineMain::GetContext();
	if (!context)
	{
		return false;
	}

	if (!m_renderedTexture)
	{
		return false;
	}

	UINT sampleMask = 0xffffffff;

	RevEngineMain::ClearTex( m_renderedTexture );
	context->OMSetRenderTargets(1, m_renderedTexture->GetRTVP(), m_dsv);
	RevLightContainer& lightContainer = RevEngineMain::GetLightContainer();
	lightContainer.UpdateLightsToShader(m_pointLightModel->GetEffect(), myCameraData);

	float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	RevEngineMain::GetContext()->OMSetBlendState(m_blendState, blendFactor, sampleMask);

	return true;
}