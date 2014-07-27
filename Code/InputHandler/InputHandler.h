#ifndef DXINPUTHANDLER_HEADER
#define DXINPUTHANDLER_HEADER

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <windows.h>
#include <vector>

#include "InputStruct.h"
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "Message.h"


class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void Init( HWND hWnd );
	void SetupResolution( const float& aX, const float& aY );
	void Release();

	void Update();
	void SwapMessages();

	std::vector< Message >* GetQueue();
	void LockQueue();

	
	void SetThreadIsKilled(){ myThreadIsKilled = true; }
	bool GetIfToContinueThread() { return myIfContinueThread; }
	void SetNotToUpdate(){ myShouldUpdate = false; }
	void SetToUpdate();
	bool SendMessages();
	bool BeReadyToSendMessages();
	bool GetShouldNotUpdateFlag();
private:

	void UpdateData();
	void SaveDataToVariables();

	LPDIRECTINPUT8 myInputHandler;
	MouseHandler myMouse;
	KeyboardHandler	myKeyboard;


	MessageQueue myInternalMessages;
	MessageQueue myExternalMessages;
	

	bool myIsUpdating;
	bool myShouldUpdate;

	bool myIfContinueThread;
	bool myThreadIsKilled;
	HANDLE myInputThread;
	bool myHaveUpdatedOnceFlag;
};

#endif