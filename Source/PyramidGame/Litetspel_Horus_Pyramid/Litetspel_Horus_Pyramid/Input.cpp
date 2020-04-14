#include "pch.h"
#include"Input.h"

Input::Input()
{


}

LRESULT Input::handleMessages(HWND hwnd, UINT uMsg, WPARAM& wParam, LPARAM& lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:
	{
		bool wasPressed = lParam & 0x40000000;
		unsigned char key = static_cast<unsigned char>(wParam);

		if (!wasPressed)
		{
			m_Keyboard.onKeyPressed(key);
		}
		return 0;
	}
	case WM_KEYUP:
	{
		unsigned char key = static_cast<unsigned char>(wParam);
		m_Keyboard.onRelease(key);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		m_Mouse.onLeftPress(mPos);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		m_Mouse.onLeftRelease(mPos);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		m_Mouse.onRightPress(mPos);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		m_Mouse.onRightRelease(mPos);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		m_Mouse.onMiddlePress(mPos);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		m_Mouse.onMiddleRelease(mPos);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		this->m_Mouse.onMove(mPos);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		MousePos mPos;
		mPos.x = LOWORD(lParam);
		mPos.y = HIWORD(lParam);

		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			m_Mouse.onWheelUp(mPos);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			m_Mouse.onWheelDown(mPos);
		}
		return 0;
	}
	case WM_INPUT:
	{
		UINT size;
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &size, sizeof(RAWINPUTHEADER));
		if (size > 0)
		{
			std::unique_ptr<BYTE[]> data = std::make_unique<BYTE[]>(size);
			if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, data.get(), &size, sizeof(RAWINPUTHEADER)) == size)
			{
				RAWINPUT* rInput = reinterpret_cast<RAWINPUT*>(data.get());
				if (rInput->header.dwType == RIM_TYPEMOUSE)
				{
					MousePos mPos;
					mPos.x = rInput->data.mouse.lLastX;
					mPos.y = rInput->data.mouse.lLastY;

					m_Mouse.onRawMove(mPos);
				}
			}
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	}

	return 0;
}

void Input::readBuffers()
{
	if (!this->m_Keyboard.empty())
	{
		std::string keyInfo = "KeyEvent";
		KeyboardEvent readEvent = this->m_Keyboard.readKey();
		keyInfo += readEvent.getKey();
		//OutputDebugStringA(keyInfo.c_str());
	}

	if (!this->m_Mouse.empty())
	{
		std::string mouseInfo = "MousePos: X:";
		MouseEvent mouseEvnt = this->m_Mouse.readEvent();
		mouseInfo += std::to_string(mouseEvnt.getPosX()) + " Y: " + std::to_string(mouseEvnt.getPosY()) + "\n";
		//OutputDebugStringA(mouseInfo.c_str());
	}
}

MouseEvent Input::getMouseEvent()
{
	return this->m_Mouse.readEvent();
}

Keyboard* Input::getKeyboard()
{
	return &this->m_Keyboard;
}