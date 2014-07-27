#ifndef _APP_H
#define _APP_H

#include "LabStateApp.h"
#include "TI_TimeManager.h"
#include "InputHandler.h"

class RevEngineMain;

class App
{
public:
	App( RevEngineMain* aEngine );
	~App(void);
	bool Update( );
	bool Render( );
	bool UpdateInputThread( );
private:
	RevEngineMain*			myEngine;
	LabStateApp				myApp;
	TI_Time::TimeManager	myTimeManager;
	InputHandler			m_inputHandler;

};

#endif