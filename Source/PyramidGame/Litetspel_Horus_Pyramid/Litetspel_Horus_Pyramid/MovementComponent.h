#pragma once

#include "pch.h"

enum class Direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

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
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR down;

	MovementComponent()
	{
		this->scale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
		this->rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->position = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->speed = 1.f;

		this->forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
		this->left = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		this->right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
		this->backward = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
		this->up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
		this->down = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);
	}
	~MovementComponent() {}

	void move(DirectX::XMVECTOR moveVector)
	{
		this->position = DirectX::XMVectorAdd(moveVector, this->position);
	}

	void move(Direction dir, float dt)
	{
		switch (dir)
		{
		case Direction::FORWARD:
			this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->forward, this->speed * dt), this->position);
			break;
		case Direction::BACKWARD:
			this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->backward, this->speed * dt), this->position);
			break;
		case Direction::LEFT:
			this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->left, this->speed * dt), this->position);
			break;
		case Direction::RIGHT:
			this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->right, this->speed * dt), this->position);
			break;
		case Direction::UP:
			this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->up, this->speed * dt), this->position);
			break;
		case Direction::DOWN:
			this->position = DirectX::XMVectorAdd(DirectX::XMVectorScale(this->down, this->speed * dt), this->position);
			break;
		default:
			assert(!"Error, no valid direction found!");
			break;
		}
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