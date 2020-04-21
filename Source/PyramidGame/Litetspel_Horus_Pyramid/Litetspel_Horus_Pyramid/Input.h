#pragma once
#include "Input_Mouse.h"
#include "Input_Keyboard.h"

class Input
{
private:
	Keyboard m_Keyboard;
	Mouse m_Mouse;
	bool m_cursorEnabled;

public:
	Input();
	LRESULT handleMessages(HWND hwnd, UINT& uMsg, WPARAM& wParam, LPARAM& lParam);
	void readBuffers();

	MouseEvent getMouseEvent();
	Keyboard* getKeyboard();
	Mouse* getMouse();

};