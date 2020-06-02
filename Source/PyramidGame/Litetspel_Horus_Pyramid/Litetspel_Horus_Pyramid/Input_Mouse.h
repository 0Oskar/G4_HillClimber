#pragma once

struct MousePos
{
	int x;
	int y;
};

class MouseEvent
{
private:
	Event eventType;
	MousePos mousePos;
public:
	MouseEvent();
	MouseEvent(const Event evnt, const int x, const int y);
	int getPosX() const;
	int getPosY() const;
	Event getEvent() const;
	MousePos getPos() const;
	bool isValid() const;
};


class Mouse
{

private:
	bool m_LDown;
	bool m_RDown;
	bool m_MDown;
	MousePos m_mousePos;
	std::queue<MouseEvent> m_events;

public:
	Mouse();
	MouseEvent readEvent();
	bool empty() const;

	void onMove(const MousePos pos);
	void onRawMove(const MousePos pos);
	void onLeftPress(const MousePos pos);
	void onRightPress(const MousePos pos);
	void onMiddlePress(const MousePos pos);
	void onLeftRelease(const MousePos pos);
	void onRightRelease(const MousePos pos);
	void onMiddleRelease(const MousePos pos);
	void onWheelUp(const MousePos pos);
	void onWheelDown(const MousePos pos);

	bool isLDown() const;
	bool isRDown() const;
	bool isMDown() const;
	int getPosx() const;
	int getPosY() const;
	MousePos getPos() const;
};
