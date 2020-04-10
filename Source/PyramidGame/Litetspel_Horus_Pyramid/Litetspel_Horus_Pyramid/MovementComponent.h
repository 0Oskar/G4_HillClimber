#pragma once

#include "pch.h"

class MovementComponent
{
private:
	// Movement
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_rotation;
	float m_speed;

	// Direction Vectors
	DirectX::XMVECTOR m_forward;
	DirectX::XMVECTOR m_left;
	DirectX::XMVECTOR m_right;
	DirectX::XMVECTOR m_backward;

public:
	MovementComponent()
	{
		this->m_position = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->m_rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->m_speed = 0.f;

		this->m_forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
		this->m_left = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		this->m_right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
		this->m_backward = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
	}
	~MovementComponent() {}



};