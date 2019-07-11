#pragma once

#include "DXUtil.h"
#include "../Library/Keyboard.h"
#include "../Library/Mouse.h"

#pragma comment (lib, "Library/DirectXTK.lib")

using namespace DirectX;

class InputClass
{
public:
	InputClass();
	~InputClass();

	bool InitializeInput(HWND hwnd);
	bool IsKeyDown(Keyboard::Keys key) 
	{ 
		return keyboard->GetState().IsKeyDown(key);
	}

	bool IsKeyUp(Keyboard::Keys key)
	{
		return keyboard->GetState().IsKeyUp(key);
	}

	void ResetKeyboardState() { keyboard->Reset(); }
	XMFLOAT2 GetMouseDragState();
	Mouse::State GetMouseState();

private:

	//DirectX::Keyboard
	Keyboard* keyboard;
	Mouse* mouse;
};