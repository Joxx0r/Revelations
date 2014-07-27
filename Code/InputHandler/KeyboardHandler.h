#ifndef _KEYBOARD_HANDLER_HEADER
#define _KEYBOARD_HANDLER_HEADER

#include <windows.h>
#include "Message.h"
#include "InputStruct.h"
#include <dinput.h>

struct IDirectInputDevice8;
class KeyboardHandler
{
public:
	KeyboardHandler(void);
	~KeyboardHandler(void);
	bool Init(LPDIRECTINPUT8& aInputDevice, HWND& aHWD);

	void Update();
	void SaveToVariables();

	void Reset();
	void RequestKeyboardMessage(MessageQueue* aQue);
	void Refresh();
private:
	LPDIRECTINPUTDEVICE8	myKeyboardDevice; //Pointer to the keyboard device
	
	unsigned char			myCurrentFramePressedKeys[256];
	unsigned char			myLastFrameCurrentPicked[256];
	unsigned char			myCurrentPressedKeys[256];

	int						myTimePressedCharacter[256];

	unsigned char			myKeyState[256];
	

	int						myCurrentActiveIndex;
	int						myCurrentActiveIndex2;
	
};

#endif