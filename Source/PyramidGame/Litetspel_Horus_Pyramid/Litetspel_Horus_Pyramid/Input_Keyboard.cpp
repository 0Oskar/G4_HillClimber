#include "pch.h"
#include "Input_Keyboard.h"


KeyboardEvent::KeyboardEvent()
{
	m_eventType = Event::Invalid;
	m_key = 0u;
}

KeyboardEvent::KeyboardEvent(const Event type, const unsigned char key)
{
	m_eventType = type;
	m_key = key;
}

bool KeyboardEvent::isPressed() const
{
	return m_eventType == Event::Pressed;
}
bool KeyboardEvent::isReleased() const
{
	return m_eventType == Event::Released;
}
bool KeyboardEvent::isValid() const
{
	return m_eventType != Event::Invalid;
}
Event KeyboardEvent::getEvent() const
{
	return m_eventType;
}
unsigned char KeyboardEvent::getKey() const
{
	return m_key;
}


Keyboard::Keyboard()
{
	for (int i = 0; i < 256; i++)
	{
		m_keyStatus[i] = false;
	}
}

void Keyboard::onKeyPressed(const unsigned char key)
{
	m_keyStatus[key] = true;
	m_keyBuffer.push(KeyboardEvent(Event::Pressed, key));
}
void Keyboard::onRelease(const unsigned char key)
{
	m_keyStatus[key] = false;
	m_keyBuffer.push(KeyboardEvent(Event::Released, key));
}
bool Keyboard::isKeyPressed(const unsigned char key) const
{
	return m_keyStatus[key];
}
bool Keyboard::empty() const
{
	return m_keyBuffer.empty();
}
std::queue<KeyboardEvent> Keyboard::getKeyBufferCopy()
{
	return m_keyBuffer;
}
KeyboardEvent Keyboard::readKey()
{
	if (m_keyBuffer.empty())
	{
		return KeyboardEvent();
	}
	else
	{
		KeyboardEvent inputEvent = m_keyBuffer.front();
		m_keyBuffer.pop();
		return inputEvent;
	}
}