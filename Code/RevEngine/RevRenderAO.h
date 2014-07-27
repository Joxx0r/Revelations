#pragma once

class RevTexture;
class RevRenderFullScreenEffect;

class RevRenderAO
{
public:
	RevRenderAO(RevFullScreenHelper* fullScreen);
	~RevRenderAO();

	virtual void Init();

	virtual bool Render(
		const float& width,
		const float& height);

	RevTexture* GetAOTexture();

protected:

	RevFullScreenHelper*	m_fullScreen;

	RevTexture*				m_aoTexture;

	RevTexture*				m_randomTexture;

};

