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
		this->m_dContext = nullptr;
		this->m_fadeIn = false;
		this->m_animationDone = true;
		this->m_fadeCBuffer.m_data.color = { 0.f, 0.f, 0.f };
		this->m_fadeCBuffer.m_data.alpha = 0.f;
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
		this->m_dContext = dContext;
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

		this->m_vertexBuffer.initialize(device, vertices, 6);

		this->m_fadeCBuffer.init(device, dContext);

		ShaderFiles shaderFiles;
		shaderFiles.vs = L"Shader Files\\TransitionVS.hlsl";
		shaderFiles.ps = L"Shader Files\\TransitionPS.hlsl";
		this->m_shaders.initialize(device, dContext, shaderFiles);
	}

	void fadeIn(XMFLOAT3 color)
	{
		this->m_fadeCBuffer.m_data.color = color;
		this->m_fadeIn = true;
		this->m_animationDone = false;
	}
	void fadeOut()
	{
		this->m_fadeIn = false;
		this->m_animationDone = false;
	}

	void update(float dt)
	{
		if (this->m_fadeIn)
		{
			if (this->m_fadeCBuffer.m_data.alpha < 1.f)
				this->m_fadeCBuffer.m_data.alpha += dt * 2;
			else
			{
				this->m_fadeCBuffer.m_data.alpha = 1.f;
				this->m_animationDone = true;
			}
		}
		else
		{
			if (this->m_fadeCBuffer.m_data.alpha > 0.f)
				this->m_fadeCBuffer.m_data.alpha -= dt * 2;
			else
			{
				this->m_fadeCBuffer.m_data.alpha = 0.f;
				this->m_animationDone = true;
			}
		}
	}

	bool isAnimationDone() const
	{
		return this->m_animationDone;
	}

	void render()
	{
		this->m_fadeCBuffer.upd();
		this->m_dContext->PSSetConstantBuffers(0, 1, this->m_fadeCBuffer.GetAdressOf());

		this->m_shaders.setShaders();

		UINT vertexOffset = 0;
		this->m_dContext->IASetVertexBuffers(0, 1, this->m_vertexBuffer.GetAddressOf(), this->m_vertexBuffer.getStridePointer(), &vertexOffset);

		this->m_dContext->Draw(this->m_vertexBuffer.getSize(), 0);
	}
};