#ifndef _REV_DOV_MANAGER_H
#define _REV_DOV_MANAGER_H

struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

class RevTexture;
class RevFullScreenHelper;

class RevDofManager
{
public:
	RevDofManager(void);
	~RevDofManager(void);
	void Init( const int& aWidth, const int& aHeight, RevFullScreenHelper* aHelper );
	void Process( ID3D11ShaderResourceView* aSource, ID3D11RenderTargetView* aTarget );
private:
	RevTexture*				myDofCompute;
	RevTexture*				myDownSample2x2;
	RevTexture*				myDownSample4x4;
	RevTexture*				myDownSample8x8;
	RevTexture*				myDownSample16x16;

	RevFullScreenHelper*	myHelper;
	
	int						myWidth;
	int						myHeight;
};

#endif