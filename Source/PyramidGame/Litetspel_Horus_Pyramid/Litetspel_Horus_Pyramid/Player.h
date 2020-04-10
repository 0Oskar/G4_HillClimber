#pragma once

#include "MovementComponent.h"

class Player
{
private:
	// Physics component
	DirectX::BoundingBox aabb;

	// Comonents
	MovementComponent* m_movementComp;

public:
	Player();
	~Player();

	// Initialization
	void initialize();

	// Getters
	DirectX::XMVECTOR getPosition() const;
	DirectX::XMMATRIX getWorldMatrix() const;
	MovementComponent* getMoveCompPtr();

	// Setters
	void setPosition(DirectX::XMVECTOR newPosition);
};