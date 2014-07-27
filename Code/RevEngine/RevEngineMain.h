#ifndef _REV_ENGINE_MAIN_H
#define _REV_ENGINE_MAIN_H

#include "RevModelFactory.h"
#include "RevEffectContainer.h"
#include "RevInstanceFactory.h"
#include "RevRenderer.h"
#include "RevLightContainer.h"
#include <windows.h>
#include <d3d11.h>

struct RevSetupData;
class RevBaseInstance;
class RevCamera;
class RevTexture;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;

class RevEngineMain
{
public:
	bool Update( const float& aDeltaTime );
	bool Render( );
	void OnNextFrame( );
	
	static ID3D11DeviceContext* GetContext();
	static ID3D11Device*		GetDevice();
	static RevModelFactory&		GetModelFactory();
	static RevEffectContainer&	GetEffectContainer();	

	static RevBaseInstance*			GenerateNewInstance( const int& type );

	static ID3D11RenderTargetView*		GetEngineRTV();
	static ID3D11DepthStencilView*		GetEngineDSV();
	static ID3D11ShaderResourceView*	GetEngineDSVRSV();

	static CU::Vector2f					GetCursorLoc();

	static RevRenderTargetCollector&	GetRenderTargetCollector();

	static float						GetAspectRatio();
	static float						GetWindowWidth();
	static float						GetWindowHeight();

	static void							AddInstanceToScene(RevBaseInstance* aInstance);
	static void							RemoveInstanceFromScene(RevBaseInstance* aInstance);
	static void							ChangeCameraToScene( RevCamera* newCamera );
	static void							ResetOMDS(); 

	static RevLightContainer&			GetLightContainer();

	static void AddLightInstance(const RevPointLight& light);

	static void AllModelsLoaded( );

	static bool Create( RevSetupData& aSetupData,
						HWND& aBaseWindowHandle,
						WNDPROC aWindowsMSGHandler,
						HINSTANCE aHInstance );

	static void ClearDepth(ID3D11DepthStencilView* dsv );
	static void ClearRTV(ID3D11RenderTargetView* rtv);
	static void ClearTex(RevTexture* tex);

	static void	Destroy( );

	static RevEngineMain*	GetInstance( );

	static RevRenderer&		GetRenderer();
	
	HWND&					GetHWND();

private:
	ID3D11DeviceContext*	GetD3DContext();
	ID3D11Device*			GetD3DDevice();
	RevModelFactory&		GetEngineModelFactory();
	RevEffectContainer&		GetEngineEffectContaner( );
		
	bool Init(	WNDPROC aWindowsMSGHandler,
				HINSTANCE aHInstance );
	bool InitDevice( );
	bool InitWindow(	WNDPROC aWindowsMSGHandler,
		HINSTANCE aHInstance);
	bool InitSwapChain( );	
	bool InitRTV( );
	bool InitDSV();
	bool InitViewPort( );

	RevEngineMain(	HWND& aWindowHandle,
					RevSetupData& aSetupData);
	virtual ~RevEngineMain(void);

	static RevEngineMain*		myEngineInstance;

	ID3D11DeviceContext*		myDeviceContext;
	ID3D11Device*				myDevice;

	HWND&						myWindowHandle;
	RevSetupData&				mySetupData;

	IDXGISwapChain*				mySwapChain;
	ID3D11RenderTargetView*		myRenderTargetView;
	D3D11_VIEWPORT 				myViewPort;

	RevModelFactory				myModelFactory;
	RevInstanceFactory			m_instanceFactory;

	RevRenderer					myRenderer;
	RevEffectContainer			myEffectContainer;
	
	ID3D11DepthStencilView*		myDSV;
	ID3D11ShaderResourceView*	myDSVRSV;

	RevLightContainer			m_lightContainer;
};

#endif