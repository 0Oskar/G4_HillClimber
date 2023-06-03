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
		m_windowDimensions = { 100.f, 100.f };
		m_transparency = 0.f;
		m_outlineColor = Colors::Black;
		m_textColor = Colors::Red;
		m_timeToShow = 2.0;
		m_showtext = false;
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
		m_string = text;
		m_timer.restart();
		m_transparency = 1.f;
		m_outlineColor = Colors::Black;
		m_textColor = Colors::Red;
		m_timeToShow = timeToShow;
		m_showtext = true;
	}

	void setWindowDimensions(int windowWidth, int windowHeight)
	{
		m_windowDimensions = { (float)windowWidth, (float)windowHeight };
	}

	void update(float dt)
	{
		if (m_showtext)
		{
			if (m_timer.timeElapsed() > m_timeToShow && m_transparency > 0.f)
			{
				m_transparency -= dt * 2;
				m_outlineColor.f[3] = m_transparency;
				m_textColor.f[3] = m_transparency;
			}
			else if (m_transparency <= 0.f)
			{
				m_showtext = false;
				m_timer.stop();
			}
		}
	}

	void render(SpriteFont* sf, SpriteBatch* sb)
	{
		if (m_showtext)
		{
			float scale = 0.75f;
			XMVECTOR fontDimensions = sf->MeasureString(m_string.c_str(), false);
			XMFLOAT2 position = {
				(m_windowDimensions.x / 2.f) - ((XMVectorGetX(fontDimensions) * scale) / 2.f),
				(m_windowDimensions.y / 6.f) - ((XMVectorGetY(fontDimensions) * scale) / 2.f)
			};

			// Outline
			sf->DrawString(sb, m_string.c_str(), { position.x + 1.f, position.y + 1.f }, m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);
			sf->DrawString(sb, m_string.c_str(), { position.x - 1.f, position.y + 1.f }, m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);
			sf->DrawString(sb, m_string.c_str(), { position.x - 1.f, position.y - 1.f }, m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);
			sf->DrawString(sb, m_string.c_str(), { position.x + 1.f, position.y - 1.f }, m_outlineColor, 0.f, DirectX::XMFLOAT2(0.f, 0.f), scale);

			// Text
			sf->DrawString(sb, m_string.c_str(), position, m_textColor, 0.f, {0.f, 0.f}, scale);
		}
	}
};