#pragma once

#include "pch.h"

#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "Shaders.h"
#include "Timer.h"

struct PS_FADE_BUFFER
{
	XMFLOAT3 color;
	float alpha;
};

class Transition
{
private:
	// Device
	ID3D11DeviceContext* m_dContext;
	
	// Vertices
	VertexBuffer<VertexPosTex> m_vertexBuffer;

	// Constant Buffers
	ConstBuffer<PS_FADE_BUFFER> m_fadeCBuffer;
	
	// Shaders
	Shaders m_shaders;

	// Timer
	Timer m_timer;
	bool m_fadeIn;
	bool m_animationDone;

	Transition()
	{
		m_dContext = nullptr;
		m_fadeIn = false;
		m_animationDone = true;
		m_fadeCBuffer.m_data.color = { 0.f, 0.f, 0.f };
		m_fadeCBuffer.m_data.alpha = 0.f;
	}

public:
	static Transition& get()
	{
		static Transition handlerInstance;
		return handlerInstance;
	}
	Transition(Transition const&) = delete;
	void operator=(Transition const&) = delete;
	~Transition() {}

	void initialize(ID3D11Device* device, ID3D11DeviceContext* dContext)
	{
		m_dContext = dContext;
		VertexPosTex vertices[]
		{
			{
				-1.f,  1.f, 0.f,
				0.f, 0.f
			},
			{
				-1.f, -1.f, 0.f,
				0.f, 1.f
			},
			{
				1.f, -1.f, 0.f,
				1.f, 1.f
			},
			{
				-1.f,  1.f, 0.f,
				0.f, 0.f
			},
			{
				1.f, -1.f, 0.f,
				1.f, 1.f
			},
			{
				1.f,  1.f, 0.f,
				1.f, 0.f
			}
		};

		m_vertexBuffer.initialize(device, vertices, 6);

		m_fadeCBuffer.init(device, dContext);

		ShaderFiles shaderFiles;
		shaderFiles.vs = L"Shader Files\\TransitionVS.hlsl";
		shaderFiles.ps = L"Shader Files\\TransitionPS.hlsl";
		m_shaders.initialize(device, dContext, shaderFiles);
	}

	void fadeIn(XMFLOAT3 color)
	{
		m_fadeCBuffer.m_data.color = color;
		m_fadeIn = true;
		m_animationDone = false;
	}
	void fadeOut()
	{
		m_fadeIn = false;
		m_animationDone = false;
	}

	void update(float dt)
	{
		if (m_fadeIn)
		{
			if (m_fadeCBuffer.m_data.alpha < 1.f)
				m_fadeCBuffer.m_data.alpha += dt * 2;
			else
			{
				m_fadeCBuffer.m_data.alpha = 1.f;
				m_animationDone = true;
			}
		}
		else
		{
			if (m_fadeCBuffer.m_data.alpha > 0.f)
				m_fadeCBuffer.m_data.alpha -= dt * 2;
			else
			{
				m_fadeCBuffer.m_data.alpha = 0.f;
				m_animationDone = true;
			}
		}
	}

	bool isAnimationDone() const
	{
		return m_animationDone;
	}

	void render()
	{
		m_fadeCBuffer.upd();
		m_dContext->PSSetConstantBuffers(0, 1, m_fadeCBuffer.GetAdressOf());

		m_shaders.setShaders();

		UINT vertexOffset = 0;
		m_dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), m_vertexBuffer.getStridePointer(), &vertexOffset);

		m_dContext->Draw(m_vertexBuffer.getSize(), 0);
	}
};