#ifndef _REV_RENDERER_GI_HELPER_H
#define _REV_RENDERER_GI_HELPER_H

#include "RevRenderEnviromentHelper.h"
#include "CU_Vector.h"
#include <vector>

struct ID3D11Texture3D;
struct ID3D11ShaderResourceView;

class RevRoomManager;
class RevRenderMesh;
class RevRenderTargetCollector;

class RevRendererGI 
{
public:
	RevRendererGI(void);
	~RevRendererGI(void);
	void SetEnviromentSize( const int& aEnvMapSize );
	virtual bool Init();
	virtual void Render(	RevRenderTargetCollector& aRenderMesh, 
							RevRoomManager& aRoomManager );
	void SetValuesInEffect( RevEffect* aEffect );
private:
	RevRenderEnviromentHelper myEnviromentHelper;
	std::vector< unsigned int > myData[ 7 ];
	ID3D11Texture3D*			my3DTextures[ 7 ];
	ID3D11ShaderResourceView*			my3DSRVs[ 7 ];

	CU::Vector4f myMinWorld;
	CU::Vector4f myMaxWorld;
	CU::Vector4f myExtents;
};

#endif