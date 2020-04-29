#pragma once

#include "pch.h"

enum class Direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class MovementComponent
{
public:
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR position;
	
	// Direction Vectors
	DirectX::XMVECTOR forward;
	DirectX::XMVECTOR left;
	DirectX::XMVECTOR right;
	DirectX::XMVECTOR backward;
	DirectX::XMVECTOR up;
	DirectX::XMVECTOR down;

	// View Matrix
	DirectX::XMMATRIX* viewMatrix;

	MovementComponent()
	{
		this->scale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
		this->rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		this->position = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

		this->forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
		this->left = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		this->right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
		this->backward = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
		this->up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
		this->down = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);

		this->viewMatrix = new DirectX::XMMATRIX(DirectX::XMMatrixIdentity());
	}
	MovementComponent(const MovementComponent& otherMoveComponent)
	{
		this->scale = otherMoveComponent.scale;
		this->rotation = otherMoveComponent.rotation;
		this->position = otherMoveComponent.position;

		this->forward = otherMoveComponent.forward;
		this->left = otherMoveComponent.left;
		this->right = otherMoveComponent.right;
		this->backward = otherMoveComponent.backward;
		this->up = otherMoveComponent.up;
		this->down = otherMoveComponent.down;

		// View Matrix
		if (this->viewMatrix)
		{
			delete this->viewMatrix;
			this->viewMatrix = nullptr;
		}
		if (otherMoveComponent.viewMatrix != nullptr)
			this->viewMatrix = new DirectX::XMMATRIX(*otherMoveComponent.viewMatrix);
	}
	~MovementComponent()
	{
		if (this->viewMatrix)
		{
			delete this->viewMatrix;
			this->viewMatrix = nullptr;
		}
	}

	MovementComponent& operator=(const MovementComponent& otherMoveComponent)
	{
		if (this == &otherMoveComponent)
			return *this;

		this->scale = otherMoveComponent.scale;
		this->rotation = otherMoveComponent.rotation;
		this->position = otherMoveComponent.position;

		this->forward = otherMoveComponent.forward;
		this->left = otherMoveComponent.left;
		this->right = otherMoveComponent.right;
		this->backward = otherMoveComponent.backward;
		this->up = otherMoveComponent.up;
		this->down = otherMoveComponent.down;

		// View Matrix
		if (this->viewMatrix)
		{
			delete this->viewMatrix;
			this->viewMatrix = nullptr;
		}
		if (otherMoveComponent.viewMatrix != nullptr)
			this->viewMatrix = new DirectX::XMMATRIX(*otherMoveComponent.viewMatrix);

		return *this;
	}

	DirectX::XMFLOAT3 getPositionF3() const
	{
		DirectX::XMFLOAT3 positionF3;
		DirectX::XMStoreFloat3(&positionF3, this->position);
		return positionF3;
	}

	void move(DirectX::XMVECTOR moveVector)
	{
		this->position = DirectX::XMVectorAdd(moveVector, this->position);
	}

	void updateViewMatrix()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->rotation);
		// Look at
		DirectX::XMVECTOR lookAt = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		// Add position to lookAt
		lookAt = DirectX::XMVectorAdd(this->position, lookAt);
		// Up
		DirectX::XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		// Update View Matrix with new Rotation

		*viewMatrix = DirectX::XMMatrixLookAtLH(this->position, lookAt, up);

		// Update Direction Vectors
		this->updateDirVectors();
	}

	void updateDirVectors()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMVectorGetY(this->rotation));
		this->forward = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		this->left = XMVector3TransformCoord(DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f), rotationMatrix);
		this->right = XMVector3TransformCoord(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), rotationMatrix);
		this->backward = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f), rotationMatrix);
	}
};