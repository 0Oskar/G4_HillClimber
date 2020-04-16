#pragma once

#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "Input.h"

class GameObject
{
protected:
	bool m_collidable;
	bool m_isStatic;
	int m_modelIndex;
	int m_wvpCBufferIndex;
	
	// Componenets
	MovementComponent* m_movementComp;
	PhysicsComponent* m_physicsComp;

public:
	GameObject();
	GameObject(const GameObject& otherGameObject);
	virtual ~GameObject();

	// Initialization
	void initializeStatic(bool collidable, int modelIndex, int wvpCBufferIndex);
	void initializeDynamic(bool collidable, int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration);

	// Update
	virtual void update(float dt);

	// Getters
	DirectX::XMVECTOR getPosition() const;
	DirectX::XMMATRIX getWorldMatrix() const;
	int getModelIndex() const;
	int getWvpCBufferIndex() const;
	MovementComponent* getMoveCompPtr();

	// Setters
	void setScale(DirectX::XMVECTOR newScale);
	void setPosition(DirectX::XMVECTOR newPosition);
	void move(Direction dir, float dt);
};