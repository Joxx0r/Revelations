#ifndef MOUSE_HANDLER
#define MOUSE_HANDLER

#include <dinput.h>
#include "CU_Vector.h"
#include "InputStruct.h"
#include "Message.h"

#include <bitset>

#define MAX_MOUSEBUTTONS 2

enum MouseMouseMouse
{
	MOUSE_CLICK = 0,
	MOUSE_DOWN,
	MOUSE_RELEASE,
	AMOUNT_OF_MOUSE
};

class MouseHandler
{
public:
	MouseHandler(void);
	~MouseHandler(void);

	void Init(LPDIRECTINPUT8& aHandler, HWND hWnd);
	void Setup( const float& aX, const float& aY  );

	void Update();
	void SaveToVariables();

	void RequestMouseInput(MessageQueue* msgs);

	void CheckMouseButtons(int aMouseBtnAmount, MessageQueue* msgs);
	void CheckMousePosition(MessageQueue* msgs);

	void CheckMouseButtonsOneMsg(Message& msg, int aMouseBtnAmount);
	void CheckMousePositionOneMsg(Message& msg);
	void Refresh();
private:
	std::bitset< MAX_MOUSEBUTTONS > myButtonHadBeenPressed;
	std::bitset< MAX_MOUSEBUTTONS > myButtonHadBeenPressedOneMsg;

	bool CheckMouseButton(int aMouseID, Message& msg);
	void CheckMouseButtonOneMsg(int aMouseID, Message& msg);

	LPDIRECTINPUTDEVICE8 myMouseDevice;
	DIMOUSESTATE myMouseState;

	CU::Vector2f myResolutionHalf;

	CU::Vector2f mySavedMouseMovement;
	CU::Vector2f mySavedMouseMovementFloat;
	CU::Vector2f myCurrentMouse;
};

#endif