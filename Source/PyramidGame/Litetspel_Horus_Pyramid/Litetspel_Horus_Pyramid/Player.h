#pragma once

#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "Input.h"

class Player
{
private:
	// Physics component
	DirectX::BoundingBox aabb;

	// Components
	MovementComponent* m_movementComp;
	PhysicsComponent* m_physicsComp;

public:
	Player();
	~Player();

	// Initialization
	void initialize(float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration);

	// Update
	void update(Keyboard* keyboard, MouseEvent mouseEvent, float dt);

	// Getters
	DirectX::XMVECTOR getPosition() const;
	DirectX::XMMATRIX getWorldMatrix() const;
	MovementComponent* getMoveCompPtr();

	// Setters
	void setPosition(DirectX::XMVECTOR newPosition);
	void move(Direction dir, float dt);
};