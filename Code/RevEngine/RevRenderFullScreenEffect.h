#pragma once

struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

class RevRenderFullScreenEffect
{
public:
	RevRenderFullScreenEffect(RevFullScreenHelper* fullScreen);
	~RevRenderFullScreenEffect();

	virtual void Init() = 0;

	virtual bool Render(
		ID3D11RenderTargetView* output,
		ID3D11ShaderResourceView* source,
		const float& width,
		const float& height) = 0;

	inline void SetFullScreenHelper(RevFullScreenHelper* fullscreen)
	{
		m_fullScreen = fullscreen;
	}

protected:

	RevFullScreenHelper* m_fullScreen;
};

