#include "StdAfx.h"
#include "RevEngineMain.h"
#include "RevSetupData.h"
#include "RevScene.h"
#include "RevTexture.h"

RevEngineMain* RevEngineMain::myEngineInstance = NULL;

bool RevEngineMain::Update(const float& aDeltaTime)
{
	myRenderer.Update(aDeltaTime);
	return true;
}

bool RevEngineMain::Render()
{
	myRenderer.Render();
	return true;
}

void RevEngineMain::OnNextFrame()
{
	//Swaps the backbuffer to another buffer
	mySwapChain->Present(0, 0);
}

ID3D11DeviceContext* RevEngineMain::GetContext()
{
	assert(myEngineInstance != NULL);
	return myEngineInstance->GetD3DContext();
}

ID3D11Device* RevEngineMain::GetDevice()
{
	assert(myEngineInstance != NULL);
	return myEngineInstance->GetD3DDevice();
}

RevModelFactory& RevEngineMain::GetModelFactory()
{
	assert(myEngineInstance != NULL);
	return myEngineInstance->GetEngineModelFactory();
}

RevEffectContainer&	RevEngineMain::GetEffectContainer()
{
	return myEngineInstance->GetEngineEffectContaner();
}

RevBaseInstance* RevEngineMain::GenerateNewInstance(const int& type)
{
	return myEngineInstance->m_instanceFactory.GetNewInstance( type );
}

ID3D11RenderTargetView*		RevEngineMain::GetEngineRTV()
{
	return myEngineInstance->myRenderTargetView;
}

ID3D11DepthStencilView*		RevEngineMain::GetEngineDSV()
{
	return myEngineInstance->myDSV;
}

ID3D11ShaderResourceView*	RevEngineMain::GetEngineDSVRSV()
{
	return myEngineInstance->myDSVRSV;
}

CU::Vector2f				RevEngineMain::GetCursorLoc()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	HWND& hwnd = myEngineInstance->GetHWND();
	ScreenToClient(hwnd, &cursorPos);
	return CU::Vector2f((float)cursorPos.x, (float)cursorPos.y);
}

RevRenderTargetCollector&	RevEngineMain::GetRenderTargetCollector()
{
	return myEngineInstance->myRenderer.GetRevRenderTargetCollector();
}

float RevEngineMain::GetAspectRatio()
{
	return static_cast<float>((float)myEngineInstance->mySetupData.myWidth / (float)myEngineInstance->mySetupData.myHeight);
}

float RevEngineMain::GetWindowWidth()
{
	return myEngineInstance->mySetupData.myWidth;
}

float RevEngineMain::GetWindowHeight()
{
	return myEngineInstance->mySetupData.myHeight;
}

void RevEngineMain::AddInstanceToScene(RevBaseInstance* aInstance)
{
	myEngineInstance->myRenderer.GetRendererScene()->AddInstance(aInstance);
}

void RevEngineMain::RemoveInstanceFromScene(RevBaseInstance* aInstance)
{
	myEngineInstance->myRenderer.GetRendererScene()->RemoveInstance(aInstance);
}

void RevEngineMain::ChangeCameraToScene(RevCamera* newCamera)
{
	myEngineInstance->myRenderer.GetRendererScene()->SetCamera(newCamera);
}

void RevEngineMain::ResetOMDS()
{
	if (!myEngineInstance)
	{
		return;
	}
	ID3D11DeviceContext* context = myEngineInstance->GetD3DContext();
	if (!context)
	{
		return;
	}
	context->OMSetRenderTargets(0, 0, 0);
}

RevLightContainer& RevEngineMain::GetLightContainer()
{
	return myEngineInstance->m_lightContainer;
}

void RevEngineMain::AddLightInstance(const RevPointLight& light)
{
	myEngineInstance->m_lightContainer.RegisterPointLight(light);
}

void RevEngineMain::AllModelsLoaded()
{
	myEngineInstance->myRenderer.GetRevRenderTargetCollector().Init();
}

bool RevEngineMain::Create(RevSetupData& aSetupData,
	HWND& aBaseWindowHandle,
	WNDPROC aWindowsMSGHandler,
	HINSTANCE aHInstance)
{
	delete myEngineInstance; myEngineInstance = NULL;
	myEngineInstance = new RevEngineMain(aBaseWindowHandle, aSetupData);
	if (!myEngineInstance->Init(aWindowsMSGHandler,
		aHInstance))
	{
		assert(0 && "Something went wrong with init engine");
		delete myEngineInstance; myEngineInstance = NULL;
		return false;
	}
	return true;
}

void RevEngineMain::ClearDepth(ID3D11DepthStencilView* dsv)
{
	if (!myEngineInstance || !dsv)
	{ 
		return;
	}
	ID3D11DeviceContext* context = myEngineInstance->GetD3DContext();
	if (!context)
	{
		return;
	}

	context->ClearDepthStencilView(dsv, 0, 1.0f, 0.0f);
}

void RevEngineMain::ClearRTV(ID3D11RenderTargetView* rtv)
{
	if (!myEngineInstance || !rtv)
	{
		return;
	}
	ID3D11DeviceContext* context = myEngineInstance->GetD3DContext();
	if (!context)
	{
		return;
	}

	float color[] = { 0, 0, 0, 1 };
	context->ClearRenderTargetView(rtv, color);
}

void RevEngineMain::ClearTex(RevTexture* tex)
{
	if (!myEngineInstance || !tex)
	{
		return;
	}

	ID3D11RenderTargetView* rtv = tex->GetRTV();
	if (rtv)
	{
		ClearRTV(rtv);
	}
}


void RevEngineMain::Destroy()
{
	delete myEngineInstance; myEngineInstance = NULL;
}

RevEngineMain* RevEngineMain::GetInstance()
{
	return myEngineInstance;
}

RevRenderer&			RevEngineMain::GetRenderer()
{
	return myEngineInstance->myRenderer;
}

ID3D11DeviceContext*	RevEngineMain::GetD3DContext()
{
	return myDeviceContext;
}

RevModelFactory&		RevEngineMain::GetEngineModelFactory()
{
	return myModelFactory;
}

RevEffectContainer&		RevEngineMain::GetEngineEffectContaner()
{
	return myEffectContainer;
}

HWND&					RevEngineMain::GetHWND()
{
	return myWindowHandle;
}

ID3D11Device*			RevEngineMain::GetD3DDevice()
{
	return myDevice;
}

bool RevEngineMain::Init(WNDPROC aWindowsMSGHandler,
	HINSTANCE aHInstance)
{
	if (InitWindow(aWindowsMSGHandler, aHInstance) == false) return false;
	if (InitDevice() == false) return false;
	if (InitSwapChain() == false) return false;
	if (InitRTV() == false) return false;
	if (InitDSV() == false) return false;
	if (InitViewPort() == false) return false;
	if (myRenderer.Init() == false) return false;
	//m_lightContainer.Init();
	InitViewPort();
	return true;
}

bool RevEngineMain::InitDevice()
{
	UINT debugFlags = 0;
	debugFlags = D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL featureLevel;

	HRESULT hr = D3D11CreateDevice(0,	//Pointer to the video adapter to use when creating a device.
		D3D_DRIVER_TYPE_HARDWARE,		//Driver type
		0,								//DLL Rasterizer
		debugFlags,						//Debug Flags
		0,								//Feature level -> null finds best suited d3d feature version
		0,								//Number of feature levels
		D3D11_SDK_VERSION,				//SDK Version of D3D
		&myDevice,						//return of d3d device
		&featureLevel,					//Feature level of the picked device
		&myDeviceContext);

	if (D3D_FEATURE_LEVEL_11_0 != featureLevel)
	{
		assert(0 && "Not initalize d3d11 feature level to DX11, is this intended?");
		return false;
	}

	if (FAILED(hr))
	{
		assert(0 && "Something went wrong when init main d3d device");
		return false;
	}
	return true;
}

bool RevEngineMain::InitWindow(WNDPROC aWindowsMSGHandler,
	HINSTANCE aHInstance)
{
	HINSTANCE hInstance = aHInstance;
	WNDPROC  msgHandler = aWindowsMSGHandler;

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));
	wcex.cbSize = sizeof(WNDCLASSEX); //Size in bytes of the struct


	wcex.style = CS_HREDRAW | CS_VREDRAW; //Different window styles to the window, use 2 and bitshift
	wcex.lpfnWndProc = msgHandler;//A pointer to the window procedure
	wcex.cbClsExtra = 0; //The number of extra bytes to allocate following the window-class structure
	wcex.cbWndExtra = 0; //The number of extra bytes to allocate following the window instance
	wcex.hInstance = hInstance; //A handle to the hinstance
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)(IDI_APPLICATION)); //A handle to the class icon. This member must be a handle to an icon resource
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW); //A handle to the class cursor. This member must be a handle to a cursor resource.
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//A handle to the class background brush
	wcex.lpszMenuName = MAKEINTRESOURCE(IDI_APPLICATION); //A pointer to a null-terminated string or is an atom.
	wcex.lpszClassName = "RevEngine"; //A window title name
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCSTR)(IDI_APPLICATION)); //A handle to a small icon that is associated with the window class

	HRESULT hr = RegisterClassEx(&wcex);
	if (FAILED(hr))
	{
		assert(0 && "Something failed when creating window ");
		return false;
	}

	RECT rc = { 0, 0,
		(UINT)mySetupData.myWidth, (UINT)mySetupData.myHeight };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
	myWindowHandle = CreateWindow("RevEngine", //A null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function.
		mySetupData.myWindowTitle, //The window name.
		WS_OVERLAPPEDWINDOW, //The style of the window being created
		CW_USEDEFAULT,  //The initial horizontal position of the window
		CW_USEDEFAULT, //The initial vertical position of the window
		rc.right - rc.left, //The width, in device units, of the window.
		rc.bottom - rc.top, //The height, in device units, of the window
		NULL, //A handle to the parent or owner window of the window being created
		NULL,  //A handle to a menu, or specifies a child-window identifier depending on the window style
		hInstance, //A handle to the instance of the module to be associated with the window
		NULL);//A pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member)

	if (myWindowHandle == 0)
	{
		assert(0 && "FAILED TO initilize window");
		return false;
	}



	ShowWindow(myWindowHandle, SW_SHOWNORMAL); //Sets the specified window's show state.
	UpdateWindow(myWindowHandle); //The UpdateWindow function updates the client area of the specified window by sending a WM_PAINT message to the window
	return true;

}

bool RevEngineMain::InitSwapChain()
{
	DXGI_MODE_DESC backBufferModeDesc;
	ZeroMemory(&backBufferModeDesc, sizeof(DXGI_MODE_DESC));
	backBufferModeDesc.Height = mySetupData.myHeight; // A value describing the resolution height
	backBufferModeDesc.Width = mySetupData.myWidth; //A value that describes the resolution width
	backBufferModeDesc.RefreshRate.Numerator = 60; //Refresh rate
	backBufferModeDesc.RefreshRate.Denominator = 1; //n unsigned integer value representing the bottom of the rational number
	backBufferModeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //A DXGI_FORMAT structure describing the display format.
	backBufferModeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //A member of the DXGI_MODE_SCANLINE_ORDER enumerated type describing the scanline drawing mode.
	backBufferModeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //A member of the DXGI_MODE_SCALING enumerated type describing the scaling mode.

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = backBufferModeDesc;


	swapChainDesc.SampleDesc.Count = 1; //The number of multisamples per pixel
	swapChainDesc.SampleDesc.Quality = 0; //The image quality level. The higher the quality, the lower the performance
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //surface usage and CPU access options for the back buffer
	swapChainDesc.BufferCount = 1; //A value that describes the number of buffers in the swap chain

	swapChainDesc.OutputWindow = myWindowHandle;
	swapChainDesc.Windowed = mySetupData.myIsWindowed; //Fullscreen flag



	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //andling the contents of the presentation buffer after presenting a surface
	swapChainDesc.Flags = 0; //DXGI_SWAP_CHAIN_FLAG enumerated type

	IDXGIDevice* dxgiDevice = 0;
	HRESULT hr = myDevice->QueryInterface(__uuidof(IDXGIDevice),
		(void**)&dxgiDevice);

	if (FAILED(hr))
	{
		assert(0 && "Could not init base chain");
		return false;
	}

	IDXGIAdapter* dxgiAdapter = 0;
	hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
		(void**)&dxgiAdapter);
	if (FAILED(hr))
	{
		assert(0 && "Could not init swap chain adapter");
		return false;
	}

	IDXGIFactory* dxFactory = 0;
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&dxFactory);

	if (FAILED(hr))
	{
		assert(0 && "Could not init swap chain factory");
		return false;
	}

	hr = dxFactory->CreateSwapChain(myDevice,
		&swapChainDesc,
		&mySwapChain);

	if (FAILED(hr))
	{
		assert(0 && "Could not create device swap chain");
		return false;
	}


	return true;

}

bool RevEngineMain::InitRTV()
{

	ID3D11Texture2D* swapChainTexture = NULL;

	HRESULT hr = mySwapChain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&swapChainTexture));
	if (FAILED(hr))
	{
		return false;
	}

	hr = myDevice->CreateRenderTargetView(swapChainTexture,
		0,
		&myRenderTargetView);


	if (FAILED(hr))
	{
		return false;
	}


	return true;
}

bool RevEngineMain::InitDSV()
{
	HRESULT result = E_FAIL;

	ID3D11Texture2D* dsvTex = NULL;

	D3D11_TEXTURE2D_DESC depthBufferInfo;
	ZeroMemory(&depthBufferInfo, sizeof(D3D11_TEXTURE2D_DESC));
	depthBufferInfo.Width = (UINT)mySetupData.myWidth;
	depthBufferInfo.Height = (UINT)mySetupData.myHeight;
	depthBufferInfo.MipLevels = 1;
	depthBufferInfo.ArraySize = 1;
	depthBufferInfo.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthBufferInfo.SampleDesc.Count = 1;
	depthBufferInfo.SampleDesc.Quality = 0;

	depthBufferInfo.Usage = D3D11_USAGE_DEFAULT;
	depthBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	depthBufferInfo.CPUAccessFlags = 0;
	depthBufferInfo.MiscFlags = 0;


	result = myDevice->CreateTexture2D(&depthBufferInfo,
		NULL,
		&dsvTex);
	if (result)
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilViewInfo;
	ZeroMemory(&stencilViewInfo,
		sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));


	stencilViewInfo.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	stencilViewInfo.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	result = myDevice->CreateDepthStencilView(dsvTex, &stencilViewInfo, &myDSV);

	if (FAILED(result))
	{
		dsvTex->Release();
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC resDesc;
	ZeroMemory(&resDesc, sizeof(resDesc));
	resDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	resDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resDesc.Texture2D.MipLevels = 1;
	result = myDevice->CreateShaderResourceView(dsvTex, &resDesc, &myDSVRSV);

	if (FAILED(result))
	{
		if (myDSV)
		{
			myDSV->Release();
			myDSV = NULL;
		}
		dsvTex->Release();
		return false;
	}


	return true;
}

bool RevEngineMain::InitViewPort()
{
	D3D11_VIEWPORT  port;
	ZeroMemory(&port, sizeof(D3D11_VIEWPORT));
	port.Width = mySetupData.myWidth;
	port.Height = mySetupData.myHeight;
	port.TopLeftX = 0;
	port.TopLeftY = 0;
	port.MaxDepth = 1.0f;
	port.MinDepth = 0.0f;
	myDeviceContext->RSSetViewports(1, &port);
	return true;
}

RevEngineMain::RevEngineMain(HWND& aWindowHandle,
	RevSetupData& aSetupData)
	:myWindowHandle(aWindowHandle),
	mySetupData(aSetupData)
{
	myDeviceContext = NULL;
	myDevice = NULL;
	mySwapChain = NULL;
	myRenderTargetView = NULL;
	myDSV = NULL;
	myDSVRSV = NULL;
}

RevEngineMain::~RevEngineMain(void)
{
	if (myDevice) myDevice->Release();
	if (myDeviceContext) myDeviceContext->Release();
	if (myRenderTargetView) myRenderTargetView->Release();
	if (mySwapChain) mySwapChain->Release();
	if (myDSV) myDSV->Release();
	if (myDSVRSV) myDSVRSV->Release();
}