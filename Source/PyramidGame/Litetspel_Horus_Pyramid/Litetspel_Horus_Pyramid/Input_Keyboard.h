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
	unsigned char getKey() const;
};

class Keyboard
{
private:
	bool keyStatus[256];
	std::queue<KeyboardEvent> keyBuffer;

public:
	Keyboard();

	void onKeyPressed(unsigned char key);
	void onRelease(unsigned char key);
	bool isKeyPressed(unsigned char key);
	bool empty();
	KeyboardEvent readKey();
};
