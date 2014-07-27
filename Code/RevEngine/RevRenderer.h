#ifndef _REV_RENDERER_H
#define _REV_RENDERER_H

#include "RevRenderEnviromentHelper.h"
#include "RevRenderTargetCollector.h"
#include "RevRenderNormalHelper.h"
#include "RevFullScreenHelper.h"
#include "RevRendererDeferedHelper.h"
#include "RevDofManager.h"
#include "RevRendererGIHelper.h"
#include "RevReflectionRenderHelper.h"
#include "RevRenderLightHelper.h"

//Temp
#include "CU_Matrix.h"
#include "RevStructures.h"
#include "CU_Vector.h"
#include <vector>

struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct RevCameraData;

class RevTexture;
class RevInstance;
class RevScene;
class RevModel;
class RevRenderFullScreenEffect;
class RevRenderAO;

class RevRenderer
{
public:
	enum RenderFrameTechs
	{
		_NORMAL,
		_CUBEMAP,
		_DEFERED,
		_REFLECTION,
		_POINTLIGHT,
		_PBL_LIGHTS,
		_FULLSCREEN_EFFECTS,
		_AO,
		_NUM_OF_FRAME_TECHS
	};
	RevRenderer(void);
	~RevRenderer(void);
	bool Init( );
	bool Update( const float& aDeltaTime );
	bool Render( );
	
	RevScene* GetRendererScene( );
	void SetScene( RevScene* aScene );

	RevRenderTargetCollector& GetRevRenderTargetCollector();

	void AddInstanceToSHScene( RevInstance* aInstance );
	void OnAllItemsLoaded( );
	RevReflectionRenderHelper& GetReflectionHelper() { return myReflectionRender; }
	
private:
	void RenderFrame(	const int& aRenderTech,
						const RevCameraData& aCameraData );
	void RenderNormalInstances(  const RevCameraData& aCameraData );
	void RenderDeferedInstances(  const RevCameraData& aCameraData );
	void RenderCubeMapInstances( );
	void RenderReflectionInstances( const RevCameraData& aCameraData );
	void RenderEnviromentMap( const RevCameraData& aCameraData );
	void RenderPointLights( RevCameraData aCameraData );
	void RenderPBLLighs(RevCameraData aCameraData);
	void RenderFullScreenEffects(RevCameraData aCameraData);
	void RenderCompletePass( const RevCameraData& aCameraData );
	void RenderAO();

	void OnFrameSetup(	const int& aNrOfRenderTargets,
						ID3D11RenderTargetView* aEngineRTV,
						ID3D11DepthStencilView* aDSVBuffer );
	void ClearColorRTV( ID3D11RenderTargetView* aRTV );
	void ClearDepth( ID3D11DepthStencilView* aDSVBuffer );

	RevScene*					myScene;

	RevRenderTargetCollector	myRenderTargetCollector;

	RevRenderEnviromentHelper	myEnviromentHelper;
	RevRenderNormalHelper		myNormalRenderHelper;

	RevFullScreenHelper			myFullScreenHelper;
	RevRendererDeferedHelper	myDeferedHelper;
	RevDofManager				myDofManager;

	RevTexture*					myRandomTexture;


	RevRendererGI				myGIRender;
	RevReflectionRenderHelper	myReflectionRender;

	//Temp
	bool						myComputeNewEnvMapFlag;
	float						myTimeTreshold;
	int							myCount;
	CU::Matrix44f				myViewMatrixes[ 6 ];
	CU::Matrix44f				myProjectionMatrix;
	CU::Vector3f				myCameraPos;

	std::vector< RevInstance* >		myInstances;
	std::vector< RevPointLight >		myPoses;

	RevModel*						myPointLightModel;
	RevTexture*					myAccuBuffer;
	RevTexture*					myCompleteBuffer;

	int							m_count;

	RevRenderLightHelper		m_lightRenderHelper;
	RevRenderAO*				m_aoRender;

};

#endif