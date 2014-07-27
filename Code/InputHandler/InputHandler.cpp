#include "stdafx.h"
#include "inputhandler.h"

DWORD WINAPI InputThreadInit( LPVOID lpParam )
{
	InputHandler& inputHnd = (*(InputHandler*)lpParam);
	while(inputHnd.GetIfToContinueThread())
	{
		inputHnd.Update();
		Sleep(1);
	}

	inputHnd.SetThreadIsKilled();
	return(0);
}

InputHandler::InputHandler(void)
{
	myIfContinueThread = true;
	myThreadIsKilled = false;
	myShouldUpdate = false;
	myHaveUpdatedOnceFlag = false;
}

InputHandler::~InputHandler(void)
{
	Release();
}

void InputHandler::Init( HWND hWnd )
{
 	DirectInput8Create(GetModuleHandle(NULL),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&myInputHandler,NULL);
	myMouse.Init( myInputHandler, hWnd );
	myKeyboard.Init(myInputHandler, hWnd);
	UpdateData();
	myInputThread = CreateThread(NULL,0,InputThreadInit,&(*this),0,0);
	CloseHandle( myInputThread );
}

void InputHandler::SetupResolution( const float& aX, const float& aY )
{
	myMouse.Setup( aX, aY  );
}

void InputHandler::Release()
{
	myIfContinueThread = false;
	while( myThreadIsKilled == false ){ Sleep( 1 );}

}

void InputHandler::Update()
{
	myIsUpdating = true;
	UpdateData();

	if(myShouldUpdate)
		SaveDataToVariables();

	myIsUpdating = false;
}

void InputHandler::UpdateData()
{
	myMouse.Update();
	myKeyboard.Update();
}
void InputHandler::SaveDataToVariables()
{
	myMouse.SaveToVariables();
	myKeyboard.SaveToVariables();
}


void InputHandler::SwapMessages()
{
	myInternalMessages.IfBusy = true;
	myExternalMessages.IfBusy = true;

	for( int i = 0; i < myInternalMessages.Queue->size(); ++i)
	{
		myExternalMessages.Queue->push_back( (*myInternalMessages.Queue)[i] );
	}
	myInternalMessages.Queue->clear();

	myInternalMessages.IfBusy = false;
	myExternalMessages.IfBusy = false;
}

//getting the queue over threads
void InputHandler::LockQueue()
{
	myExternalMessages.IfBusy = false;
	myExternalMessages.Queue->clear();
}

std::vector< Message >* InputHandler::GetQueue()
{
	if(!myShouldUpdate)
		return 0;

	while( myIsUpdating )
		Sleep(1);
	if( myExternalMessages.IfBusy == false )
	{
		myExternalMessages.IfBusy = true;
		return(myExternalMessages.Queue);
	}
	return 0;
}

bool InputHandler::SendMessages()
{
	LockQueue();
	myShouldUpdate = true;
	return true;
}

bool InputHandler::BeReadyToSendMessages()
{	
	while(myIsUpdating)
	{
		Sleep(1);
	}

	myShouldUpdate = false;
	myKeyboard.RequestKeyboardMessage(&myExternalMessages);
	myMouse.RequestMouseInput(&myExternalMessages);

	return true;
}



void InputHandler::SetToUpdate()
{
	myShouldUpdate = true;
}

bool InputHandler::GetShouldNotUpdateFlag()
{
	return myShouldUpdate;
}