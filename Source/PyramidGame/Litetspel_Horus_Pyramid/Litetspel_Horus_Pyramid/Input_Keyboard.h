#pragma once


class KeyboardEvent
{
private:
	Event eventType;
	unsigned char key;
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
	bool keyStatus[256];
	std::queue<KeyboardEvent> keyBuffer;

public:
	Keyboard();

	void onKeyPressed(const unsigned char key);
	void onRelease(const unsigned char key);
	bool isKeyPressed(const unsigned char key) const;
	bool empty() const;
	KeyboardEvent readKey();
};
