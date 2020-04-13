#pragma once
#include"Event.h"

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
	MouseEvent(Event evnt, int x, int y);
	int getPosX();
	int getPosY();
	Event getEvent();
	MousePos getPos();
	bool isValid();


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
	bool empty();

	void onMove(MousePos pos);
	void onRawMove(MousePos pos);
	void onLeftPress(MousePos pos);
	void onRightPress(MousePos pos);
	void onMiddlePress(MousePos pos);
	void onLeftRelease(MousePos pos);
	void onRightRelease(MousePos pos);
	void onMiddleRelease(MousePos pos);
	void onWheelUp(MousePos pos);
	void onWheelDown(MousePos pos);

	bool isLDown();
	bool isRDown();
	bool isMDown();
	int getPosx();
	int getPosY();
	MousePos getPos();


};
