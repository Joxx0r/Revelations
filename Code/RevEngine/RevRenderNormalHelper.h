#ifndef _REV_RENDER_NORMAL_HELPER_H
#define _REV_RENDER_NORMAL_HELPER_H

#include "RevRenderBaseHelper.h"
#include <vector>

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;

class RevRenderNormalHelper : public RevRenderBaseHelper
{
public:
	RevRenderNormalHelper(void);
	~RevRenderNormalHelper(void);
	virtual bool Init();
	virtual void Render( RevRenderMesh& aRenderMesh );
	virtual bool Setup( );
	virtual bool SetupRTVAndDSV(	const int& aNrOfRTVs,
									ID3D11RenderTargetView* aRTV,
									ID3D11DepthStencilView* aDSV );
	virtual bool SetupRenderTech(	const int& aRenderTech );

private:
	int						myNrOfRTVS;
	int						myEffectTechnique;

	ID3D11RenderTargetView* myRTVs;
	ID3D11DepthStencilView* myDSV;
};

#endif