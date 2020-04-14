#pragma once
#include "Input_Mouse.h"
#include "Input_Keyboard.h"

class Input
{
private:
	Keyboard m_Keyboard;
	Mouse m_Mouse;

public:
	Input();
	LRESULT handleMessages(HWND hwind, UINT uMsg, WPARAM& wParam, LPARAM& lParam);
	void readBuffers();

	MouseEvent getMouseEvent();
	Keyboard* getKeyboard();
};