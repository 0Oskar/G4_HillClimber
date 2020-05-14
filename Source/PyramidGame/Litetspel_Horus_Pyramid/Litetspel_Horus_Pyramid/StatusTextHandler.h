#pragma once

#include "pch.h"
#include"Timer.h"

class StatusTextHandler
{
private:
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	std::string m_string;
	Timer timer;

	StatusTextHandler() {};

public:
	static StatusTextHandler& get()
	{
		static StatusTextHandler handlerInstance;
		return handlerInstance;
	}
	StatusTextHandler(StatusTextHandler const&) = delete;
	void operator=(StatusTextHandler const&) = delete;
	~StatusTextHandler() {}

	void update()
	{
		//test
	}

	void render()
	{

	}
};