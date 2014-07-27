#include "StdAfx.h"
#include "App.h"
#include "RevEngineMain.h"

App::App( RevEngineMain* aEngine )
:myEngine( aEngine )
{	
	m_inputHandler.SetupResolution( RevEngineMain::GetWindowWidth(), RevEngineMain::GetWindowHeight() );
	m_inputHandler.Init( RevEngineMain::GetInstance()->GetHWND() );
	m_inputHandler.SetToUpdate();
	myApp.Init();
	myTimeManager.InitDefaults( );
	myApp.Activate();
}

App::~App(void)
{
	
}

bool App::Update( )
{
	myTimeManager.Update();
	UpdateInputThread();
	GameRoot::DistributeMessages();

	const float deltaTime =  static_cast<float>(  myTimeManager.GetElapsedTime().GetTime() );
	myApp.Update(deltaTime );
	myEngine->Update( deltaTime );
	Render();
	if( GetAsyncKeyState( VK_ESCAPE ) )
	{
		return false;
	}
	return true;
}

bool App::Render( )
{
	myApp.Render( );
	myEngine->Render( );
	myEngine->OnNextFrame( );
	return true;
}

#pragma optimize("" ,off )

bool App::UpdateInputThread( )
{
	std::vector< Message >* msgs =  m_inputHandler.GetQueue();
	m_inputHandler.BeReadyToSendMessages();
	if( msgs ) 
	{
		for(int i = 0; i < msgs->size(); ++i)
		{
			GameRoot::AddMessage( (*msgs)[i] );
		}

		m_inputHandler.SwapMessages();
		m_inputHandler.SendMessages();
	}
	return true;
}

#pragma optimize("" ,on )
