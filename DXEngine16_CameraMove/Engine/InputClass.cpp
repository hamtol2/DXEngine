#include "InputClass.h"

InputClass::InputClass()
{
	keyboard = new Keyboard();
	mouse = new Mouse();
}

InputClass::~InputClass()
{
	Memory::SafeDelete(keyboard);
	Memory::SafeDelete(mouse);
}

bool InputClass::InitializeInput(HWND hwnd)
{
	mouse->SetWindow(hwnd);
	mouse->SetMode(Mouse::Mode::MODE_RELATIVE);

	return true;
}

XMFLOAT2 InputClass::GetMouseDragState()
{
	Mouse::State state = mouse->GetState();
	return XMFLOAT2(state.x, state.y);
}

Mouse::State InputClass::GetMouseState()
{
	return mouse->GetState();
}
