#ifndef _REV_FULL_SCREEN_HELPER_H
#define _REV_FULL_SCREEN_HELPER_H

#include "RevStructures.h"
#include "CU_Vector.h"
#include <string>

struct ID3D11InputLayout;

class RevEffect;

class RevFullScreenHelper
{
public:
	RevFullScreenHelper(void);
	~RevFullScreenHelper(void);
	bool Init( );
	void Process(std::string aTechniqueName,ID3D11RenderTargetView* aTarget,int aWidth,int aHeight,ID3D11ShaderResourceView* aSource, int a2Width, int a2Height);
	void Process(std::string aTechniqueName,ID3D11RenderTargetView* aTarget,int aWidth,int aHeight);
	void SetViewPort(	const float& aX, 
						const float& aY,
						const float& aTopX, 
						const float& aTopY );
	RevEffect* GetEffect( );

private:
	bool InitVertexBuffer();
	void ProcessSmall(std::string aTechniqueName);
	void RenderQuad(std::string aTechniqueName,ID3D11RenderTargetView* aTarget,CU::Vector2i aTargetSize,ID3D11ShaderResourceView* aSource,CU::Vector2i aSourceSize);


	//VertexDataWrapper myVertexDataWrapper;
	RevVertexBuffer myVB;
	RevEffect* myEffect;
	ID3D11InputLayout*      myVertexLayout;
	bool myIsUseCustomViewport;
	D3D11_VIEWPORT myViewPort;

};

#endif