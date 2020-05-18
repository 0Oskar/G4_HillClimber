#pragma once

#include "pch.h"

class StatusTextHandler
{
private:
	std::string m_string;
	Timer m_timer;
	XMFLOAT2 m_windowDimensions;
	XMVECTORF32 m_outlineColor;
	XMVECTORF32 m_textColor;
	float m_transparency;
	double m_timeToShow;
	bool m_showtext;

	StatusTextHandler()
	{
		this->m_windowDimensions = { 100.f, 100.f };
		this->m_transparency = 0.f;
		this->m_outlineColor = Colors::Black;
		this->m_textColor = Colors::Red;
		this->m_timeToShow = 2.0;
		this->m_showtext = false;
	};

public:
	static StatusTextHandler& get()
	{
		static StatusTextHandler handlerInstance;
		return handlerInstance;
	}
	StatusTextHandler(StatusTextHandler const&) = delete;
	void operator=(StatusTextHandler const&) = delete;
	~StatusTextHandler() {}

	void sendText(std::string text, double timeToShow = 2.0)
	{
		this->m_string = text;
		this->m_timer.restart();
		this->m_transparency = 1.f;
		this->m_outlineColor = Colors::Black;
		this->m_textColor = Colors::Red;
		this->m_timeToShow = timeToShow;
		this->m_showtext = true;
	}

	void setWindowDimensions(int windowWidth, int windowHeight)
	{
		this->m_windowDimensions = { (float)windowWidth, (float)windowHeight };
	}

	void update(float dt)
	{
		if (this->m_showtext)
		{
			if (this->m_timer.timeElapsed() > this->m_timeToShow && this->m_transparency > 0.f)
			{
				this->m_transparency -= dt * 2;
				this->m_outlineColor.f[3] = this->m_transparency;
				this->m_textColor.f[3] = this->m_transparency;
			}
			else if (this->m_transparency <= 0.f)
			{
				this->m_showtext = false;
				this->m_timer.stop();
			}
		}
	}

	void render(SpriteFont* sf, SpriteBatch* sb)
	{
		if (this->m_showtext)
		{
			float scale = 0.75f;
			XMVECTOR fontDimensions = sf->MeasureString(this->m_string.c_str(), false);
			XMFLOAT2 position = {
				(this->m_windowDimensions.x / 2.f) - ((XMVectorGetX(fontDimensions) * scale) / 2.f),
				(this->m_windowDimensions.y / 6.f) - ((XMVectorGetY(fontDimensions) * scale) / 2.f)
			};

			// Outline
			sf->DrawString(sb, this->m_string.c_str(), { position.x + 1.f, position.y + 1.f }, this->m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);
			sf->DrawString(sb, this->m_string.c_str(), { position.x - 1.f, position.y + 1.f }, this->m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);
			sf->DrawString(sb, this->m_string.c_str(), { position.x - 1.f, position.y - 1.f }, this->m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);
			sf->DrawString(sb, this->m_string.c_str(), { position.x + 1.f, position.y - 1.f }, this->m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);

			// Text
			sf->DrawString(sb, this->m_string.c_str(), position, this->m_textColor, 0.f, {0.f, 0.f}, scale);
		}
	}
};