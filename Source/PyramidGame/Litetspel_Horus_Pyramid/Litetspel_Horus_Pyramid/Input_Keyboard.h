#pragma once


class KeyboardEvent
{
private:
	Event m_eventType;
	unsigned char m_key;
public:

	KeyboardEvent();
	KeyboardEvent(const Event type, const unsigned char key);

	bool isPressed() const;
	bool isReleased() const;
	bool isValid() const;
	Event getEvent() const;
	unsigned char getKey() const;
};

class Keyboard
{
private:
	bool m_keyStatus[256];
	std::queue<KeyboardEvent> m_keyBuffer;

public:
	Keyboard();

	void onKeyPressed(const unsigned char key);
	void onRelease(const unsigned char key);
	bool isKeyPressed(const unsigned char key) const;
	bool empty() const;
	KeyboardEvent readKey();
	std::queue<KeyboardEvent> getKeyBufferCopy();
};
