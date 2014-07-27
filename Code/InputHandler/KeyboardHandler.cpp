#include "stdafx.h"
#include "KeyboardHandler.h"
#include "Message.h"

KeyboardHandler::KeyboardHandler(void)
{
	myCurrentActiveIndex = 0;
}

KeyboardHandler::~KeyboardHandler(void)
{
	myKeyboardDevice->Release();
}

bool KeyboardHandler::Init(LPDIRECTINPUT8& aInputDevice, HWND& aHWD)
{

	HRESULT result = aInputDevice->CreateDevice(	GUID_SysKeyboard,
		&myKeyboardDevice,
		NULL);

	if(FAILED(result))
	{
		return false;
	}

	result = myKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);

	if(FAILED(result))
	{
		return false;
	}
	
	result = myKeyboardDevice->SetCooperativeLevel(aHWD,
		DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if(FAILED(result))
	{
		return false;
	}
	Reset();
	memset(myCurrentPressedKeys, 0, 256 * sizeof(unsigned char));
	memset(myLastFrameCurrentPicked, 0, 256 * sizeof(unsigned char));
	memset(myKeyState, 0, sizeof(unsigned char) * 256 );
	return true;
}

void KeyboardHandler::Update()
{
	myKeyboardDevice->Acquire();
	myKeyboardDevice->GetDeviceState(256, (LPVOID)myKeyState);
}

void KeyboardHandler::SaveToVariables()
{
	for(int i = 0; i < 256; i++)
	{
		if(myKeyState[i] && !myCurrentFramePressedKeys[i])
		{
			myCurrentFramePressedKeys[i] = 1;
		}
	}
	for(int i = 0; i < 256; i++)
	{
		if(myCurrentFramePressedKeys[i] && !myLastFrameCurrentPicked[i])
		{
			myTimePressedCharacter[i]++;
		}
		myLastFrameCurrentPicked[i] = myCurrentFramePressedKeys[i];
	}
}


void KeyboardHandler::RequestKeyboardMessage(MessageQueue* aQue)
{
	for(int i = 0; i < 256; i++)
	{
		if(myCurrentFramePressedKeys[i])
		{
			Message message(INPUT_BUTTON_HOLD_IN);
			message.AddValue<int>("key-hold", i);
			aQue->Queue->push_back(message);
		}
		if(myCurrentFramePressedKeys[i] && !myCurrentPressedKeys[i])
		{
			myCurrentPressedKeys[i] = 1;
			for(int j = 0; j <myTimePressedCharacter[i]; j++)
			{
				Message message(INPUT_BUTTON_CLICKED);
				message.AddValue<int>("key-clicked", i);
				aQue->Queue->push_back(message);
			}
		}
		else
		if(!myCurrentFramePressedKeys[i] && myCurrentPressedKeys[i])
		{
			myCurrentPressedKeys[i] = 0;

			Message message(INPUT_BUTTON_RELEASED);
			message.AddValue<int>("key-released", i);
			aQue->Queue->push_back(message);
		}

	}
	Reset();
}
void KeyboardHandler::Reset()
{
	memset(myCurrentFramePressedKeys, 0, 256 * sizeof(unsigned char));
	memset(myTimePressedCharacter, 0, 256 * sizeof(int));
}

void KeyboardHandler::Refresh()
{
	myKeyboardDevice->Acquire();
}
