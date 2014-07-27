#include "stdafx.h"
#include "MouseHandler.h"
#include "InputStruct.h"
#include <sstream>


#define MOUSESENSITIVITY 5.0f	


MouseHandler::MouseHandler(void)
{
}

MouseHandler::~MouseHandler(void)
{
	myMouseDevice->Release();
}

//dinput stuff
void MouseHandler::Init(LPDIRECTINPUT8& aHandler, HWND hWnd)
{
	HRESULT result = aHandler->CreateDevice(GUID_SysMouse,&myMouseDevice,NULL);
	if(FAILED(result))
	{
		assert( 0 && "Failed to initialse mouse input device in input handler");
	}
	result = myMouseDevice->SetDataFormat(&c_dfDIMouse);
	if(FAILED(result))
	{
		assert( 0 && "Failed to initialse mouse device format in input handler");
	}

	result = myMouseDevice->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if(FAILED(result))
	{
		assert( 0 && "Failed to initialse cooperative level input handler");
	}

	myCurrentMouse.Set(0,0);

	for( int i = 0; i < MAX_MOUSEBUTTONS; ++i )
	{
		myButtonHadBeenPressed[i] = false;
		myButtonHadBeenPressedOneMsg[i] = false;
	}
}

void MouseHandler::Setup( const float& aX, const float& aY )
{
	myResolutionHalf.Set( aX / 2 , aY / 2);
	mySavedMouseMovement.Set(0,0);
	mySavedMouseMovementFloat.Set(0,0);
}

void MouseHandler::Update()
{
	myMouseDevice->Acquire();
	myMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE),(LPVOID)&myMouseState);
	myCurrentMouse.x += static_cast<float>(myMouseState.lX) * (1 / MOUSESENSITIVITY);
	myCurrentMouse.y += static_cast<float>(myMouseState.lY) * (1 / MOUSESENSITIVITY);
}
void MouseHandler::SaveToVariables()
{
	mySavedMouseMovement += myCurrentMouse;
	mySavedMouseMovementFloat.x += myCurrentMouse.x;
	mySavedMouseMovementFloat.y += myCurrentMouse.y;
	mySavedMouseMovementFloat.x = max(-myResolutionHalf.x, mySavedMouseMovementFloat.x);
	mySavedMouseMovementFloat.x = min(myResolutionHalf.x, mySavedMouseMovementFloat.x);
	
	mySavedMouseMovementFloat.y = max(-myResolutionHalf.y, mySavedMouseMovementFloat.y);
	mySavedMouseMovementFloat.y = min(myResolutionHalf.y, mySavedMouseMovementFloat.y);
}
void MouseHandler::RequestMouseInput(MessageQueue* msgs)
{
	CheckMousePosition( msgs );
	CheckMouseButtons( MAX_MOUSEBUTTONS, msgs );

	Message msg(INPUT_MOUSE_ALL);
	CheckMouseButtonsOneMsg( msg, MAX_MOUSEBUTTONS );
	CheckMousePositionOneMsg( msg );
	msgs->Queue->push_back(msg);

	myCurrentMouse.Set(0.0f,0.0f);
}

//messages
void MouseHandler::CheckMouseButtons(int aMouseBtnAmount, MessageQueue* msgs)
{
	//buttons
	for(int btn = 0; btn < aMouseBtnAmount; ++btn)
	{
		Message msg(NUM_OF_MESSAGE_TYPES);
		if( CheckMouseButton(btn, msg) )
		{
			msgs->Queue->push_back( msg );
		}
		else
		{
			msg.DeleteAllRemainingData();
		}
	}
}

void MouseHandler::CheckMousePosition(MessageQueue* msgs)
{	
	Message msg(INPUT_MOUSE_POSITION);
	CheckMousePositionOneMsg(msg);
	msgs->Queue->push_back(msg);
}

void MouseHandler::CheckMousePositionOneMsg(Message& msg)
{	
	CU::Vector2f abso;
	abso.x = myResolutionHalf.x +  mySavedMouseMovementFloat.x;
	abso.y = myResolutionHalf.y + mySavedMouseMovementFloat.y;
	msg.AddValue("PositionAbsolute",abso);
	msg.AddValue("PositionRelative",myCurrentMouse);
}

void MouseHandler::CheckMouseButtonsOneMsg(Message& msg, int aMouseBtnAmount)
{
	//buttons
	for(int btn = 0; btn < aMouseBtnAmount; ++btn)
	{
		CheckMouseButtonOneMsg(btn, msg);
	}
}

bool MouseHandler::CheckMouseButton(int aMouseID, Message& msg)
{
	assert(aMouseID < 5 && "You cant check mouse buttons above mouse button id 5");
	msg.AddValue( "MouseID" , aMouseID );

	if( myMouseState.rgbButtons[aMouseID] )
	{
		if( myButtonHadBeenPressed[aMouseID] == true )
		{
			msg.SetMessageListType( INPUT_MOUSE_DOWN );
			return true;
		}
		else
		{
			msg.SetMessageListType( INPUT_MOUSE_CLICK );
			myButtonHadBeenPressed[aMouseID] = true;
			return true;
		}
	}
	else
	{
		if( myButtonHadBeenPressed[aMouseID] == true )
		{
			msg.SetMessageListType( INPUT_MOUSE_RELEASE );
			myButtonHadBeenPressed[aMouseID] = false;
			return true;
		}
	}
	return false;
}

void MouseHandler::CheckMouseButtonOneMsg(int aMouseID, Message& msg)
{
	std::bitset< AMOUNT_OF_MOUSE > mouseData;
	for(int i = 0; i < 3; ++i)
		mouseData[i] = false;
	
	if( myMouseState.rgbButtons[aMouseID] )
	{
		if( myButtonHadBeenPressedOneMsg[aMouseID] == true )
		{
			mouseData[ MOUSE_DOWN ] = true;
		}
		else
		{
			myButtonHadBeenPressedOneMsg[aMouseID] = true;
			mouseData[ MOUSE_CLICK ] = true;
		}
	}
	else
	{
		if( myButtonHadBeenPressedOneMsg[aMouseID] == true )
		{
			myButtonHadBeenPressedOneMsg[aMouseID] = false;
			mouseData[ MOUSE_RELEASE ] = true;
		}
	}
	std::stringstream str;
	str << "MouseID";
	str << aMouseID;
	std::string a = str.str();
	msg.AddValue( a, mouseData );
}

void MouseHandler::Refresh()
{
	myMouseDevice->Acquire();
}