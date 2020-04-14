#pragma once

#include "pch.h"

class MovementComponent
{
public:
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR position;
	float speed;

	// Direction Vectors
	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR left;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR backward;

	MovementComponent()
	{
		this->scale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
		this->rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->position = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->speed = 0.f;

		this->forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
		this->left = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		this->right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
		this->backward = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
	}
	~MovementComponent() {}

	void move(DirectX::XMVECTOR moveVector)
	{
		this->position = DirectX::XMVectorAdd(moveVector, this->position);
	}

	void updateDirVectors()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->rotation);
		this->forward = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		this->left = XMVector3TransformCoord(DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f), rotationMatrix);
		this->right = XMVector3TransformCoord(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), rotationMatrix);
		this->backward = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f), rotationMatrix);
	}
};