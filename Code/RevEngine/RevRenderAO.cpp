#include "stdafx.h"
#include "RevRenderAO.h"
#include "RevTexture.h"

RevRenderAO::RevRenderAO(RevFullScreenHelper* fullScreen)
:m_fullScreen(fullScreen)
{
	m_randomTexture = nullptr;
	m_aoTexture = nullptr;
}


RevRenderAO::~RevRenderAO()
{
	delete m_aoTexture; m_aoTexture = nullptr;
	delete m_randomTexture; m_randomTexture = nullptr;
}

void RevRenderAO::Init()
{
	if (!m_fullScreen)
	{
		return;
	}
	RevEffect* effect = m_fullScreen->GetEffect();
	if (!effect)
	{
		return;
	}

	m_aoTexture = new RevTexture(
		RevEngineMain::GetWindowWidth(),
		RevEngineMain::GetWindowHeight(),
		DXGI_FORMAT_R32_FLOAT,
		true,
		true,
		false);

	m_randomTexture = new RevTexture();

	if (!
		m_randomTexture->LoadTexture(
		"Models\\RandomTexture.jpg",
		false))
	{
		delete m_randomTexture; m_randomTexture = nullptr;
		delete m_aoTexture; m_aoTexture = nullptr;
	}

	effect->SetTextureVarible("g_RandomTexture", m_randomTexture->GetSRV());

}

bool RevRenderAO::Render(
	const float& width, 
	const float& height)
{
	if (!m_fullScreen)
	{
		return false;
	}

	m_fullScreen->Process(
		"SSAO", 
		m_aoTexture->GetRTV(), 
		width, 
		height, 
		nullptr, 
		width, 
		height);
	return true;
}

RevTexture* RevRenderAO::GetAOTexture()
{
	return m_aoTexture;	
}