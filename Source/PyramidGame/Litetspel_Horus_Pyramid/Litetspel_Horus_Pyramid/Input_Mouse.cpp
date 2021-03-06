#include "pch.h"
#include "Input_Mouse.h"

MouseEvent::MouseEvent()
{
	this->eventType = Event::Invalid;
	this->mousePos.x = 0;
	this->mousePos.y = 0;
}
MouseEvent::MouseEvent(Event evnt, int x, int y)
{
	this->eventType = evnt;
	this->mousePos.x = x;
	this->mousePos.y = y;
}
int MouseEvent::getPosX() const
{
	return this->mousePos.x;
}
int MouseEvent::getPosY()const
{
	return this->mousePos.y;
}
Event MouseEvent::getEvent() const
{
	return this->eventType;
}
MousePos MouseEvent::getPos() const
{

	return this->mousePos;
}
bool MouseEvent::isValid() const
{
	return this->eventType != Event::Invalid;
}
Mouse::Mouse()
{
	this->m_LDown = false;
	this->m_MDown = false;
	this->m_RDown = false;
	this->m_mousePos.x = 0;
	this->m_mousePos.y = 0;
}
MouseEvent Mouse::readEvent()
{
	MouseEvent evnt;
	if (this->m_events.empty())
	{

	}
	else
	{
		evnt = this->m_events.front();
		this->m_events.pop();
	}

	return evnt;
}

bool Mouse::empty() const
{
	return this->m_events.empty();
}

void Mouse::onMove(MousePos pos)
{
	this->m_mousePos = pos;
	this->m_events.push(MouseEvent(Event::MouseMove, (int)(pos.x*0.25), (int)(pos.y* 0.25)));
}
void Mouse::onRawMove(MousePos pos)
{
	this->m_events.push(MouseEvent(Event::MouseRAW_MOVE, pos.x, pos.y));
}
void Mouse::onLeftPress(MousePos pos)
{
	this->m_LDown = true;
	this->m_events.push(MouseEvent(Event::MouseLPressed, pos.x, pos.y));
}
void Mouse::onRightPress(MousePos pos)
{
	this->m_RDown = true;
	this->m_events.push(MouseEvent(Event::MouseRPressed, pos.x, pos.y));
}
void Mouse::onMiddlePress(MousePos pos)
{
	this->m_MDown = true;
	this->m_events.push(MouseEvent(Event::MouseMPressed, pos.x, pos.y));
}
void Mouse::onLeftRelease(MousePos pos)
{
	this->m_LDown = false;
	this->m_events.push(MouseEvent(Event::MouseLReleased, pos.x, pos.y));
}
void Mouse::onRightRelease(MousePos pos)
{
	this->m_RDown = false;
	this->m_events.push(MouseEvent(Event::MouseRReleased, pos.x, pos.y));
}
void Mouse::onMiddleRelease(MousePos pos)
{
	this->m_MDown = false;
	this->m_events.push(MouseEvent(Event::MouseMReleased, pos.x, pos.y));
}
void Mouse::onWheelUp(MousePos pos)
{
	this->m_events.push(MouseEvent(Event::MouseWheelUp, pos.x, pos.y));
}
void Mouse::onWheelDown(MousePos pos)
{
	this->m_events.push(MouseEvent(Event::MouseWheelDown, pos.x, pos.y));
}

bool Mouse::isLDown() const
{
	return this->m_LDown;
}
bool Mouse::isRDown() const
{
	return this->m_RDown;
}
bool Mouse::isMDown() const
{
	return this->m_MDown;
}
int Mouse::getPosx() const
{
	return this->m_mousePos.x;
}
int Mouse::getPosY() const
{
	return this->m_mousePos.y;
}
MousePos Mouse::getPos() const
{
	return this->m_mousePos;
}