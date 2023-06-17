#pragma once

#include "pch.h"

enum class Direction { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class MovementComponent
{
public:
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR localRotation;
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
		scale = DirectX::XMVectorSet(1.f, 1.f, 1.f, 0.f);
		rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		localRotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);
		position = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);

		forward = DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f);
		left = DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f);
		right = DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f);
		backward = DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f);
		up = DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f);
		down = DirectX::XMVectorSet(0.f, -1.f, 0.f, 0.f);

		
		viewMatrix = new DirectX::XMMATRIX(DirectX::XMMatrixIdentity());
	}
	MovementComponent(const MovementComponent& otherMoveComponent)
	{
		scale = otherMoveComponent.scale;
		rotation = otherMoveComponent.rotation;
		localRotation = otherMoveComponent.localRotation;
		position = otherMoveComponent.position;

		forward = otherMoveComponent.forward;
		left = otherMoveComponent.left;
		right = otherMoveComponent.right;
		backward = otherMoveComponent.backward;
		up = otherMoveComponent.up;
		down = otherMoveComponent.down;

		// View Matrix
		if (viewMatrix) {
			delete viewMatrix;
		}

		viewMatrix = nullptr;
		if (otherMoveComponent.viewMatrix != nullptr)
			viewMatrix = new DirectX::XMMATRIX(*otherMoveComponent.viewMatrix);
	}
	~MovementComponent()
	{
		if (viewMatrix)
		{
			delete viewMatrix;
			viewMatrix = nullptr;
		}
	}

	MovementComponent& operator=(const MovementComponent& otherMoveComponent)
	{
		if (this == &otherMoveComponent)
			return *this;

		scale = otherMoveComponent.scale;
		rotation = otherMoveComponent.rotation;
		localRotation = otherMoveComponent.localRotation;
		position = otherMoveComponent.position;

		forward = otherMoveComponent.forward;
		left = otherMoveComponent.left;
		right = otherMoveComponent.right;
		backward = otherMoveComponent.backward;
		up = otherMoveComponent.up;
		down = otherMoveComponent.down;

		// View Matrix
		if (viewMatrix)
		{
			delete viewMatrix;
			viewMatrix = nullptr;
		}
		if (otherMoveComponent.viewMatrix != nullptr)
			viewMatrix = new DirectX::XMMATRIX(*otherMoveComponent.viewMatrix);

		return *this;
	}

	DirectX::XMFLOAT3 getPositionF3() const
	{
		DirectX::XMFLOAT3 positionF3;
		DirectX::XMStoreFloat3(&positionF3, position);
		return positionF3;
	}

	void move(DirectX::XMVECTOR moveVector)
	{
		position = DirectX::XMVectorAdd(moveVector, position);
	}

	void updateViewMatrix()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(rotation);
		// Look at
		DirectX::XMVECTOR lookAt = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		// Add position to lookAt
		lookAt = DirectX::XMVectorAdd(position, lookAt);
		// Up
		DirectX::XMVECTOR up = XMVectorSet(0.f, 1.f, 0.f, 0.f);

		// Update View Matrix with new Rotation
		*viewMatrix = DirectX::XMMatrixLookAtLH(position, lookAt, up);

		// Update Direction Vectors
		updateDirVectors();
	}

	void updateDirVectors()
	{
		DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(DirectX::XMVectorGetY(rotation));
		forward = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
		left = XMVector3TransformCoord(DirectX::XMVectorSet(-1.f, 0.f, 0.f, 0.f), rotationMatrix);
		right = XMVector3TransformCoord(DirectX::XMVectorSet(1.f, 0.f, 0.f, 0.f), rotationMatrix);
		backward = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, -1.f, 0.f), rotationMatrix);
	}
};