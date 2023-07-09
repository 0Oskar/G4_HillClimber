#pragma once

#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "Input.h"
#include "Model.h"

class GameObject
{
protected:
	bool m_visible;
	bool m_collidable;
	bool m_isStatic;
	bool m_shouldDrawBB;
	bool m_useDeceleration;
	int m_wvpCBufferIndex;
	
	// Componenets
	MovementComponent* m_movementComp;
	PhysicsComponent* m_physicsComp;
	Model* m_modelPtr;
	std::wstring m_texturePath;

public:
	GameObject();
	GameObject(const GameObject& otherGameObject);
	virtual ~GameObject();
	GameObject& operator=(const GameObject& otherGameObject);

	// Initialization
	void initializeStatic(bool collidable, int wvpCBufferIndex, Model* mdl);
	void initializeDynamic(bool collidable, bool useDeceleration, int wvpCBufferIndex, float mass, XMFLOAT3 acceleration, XMFLOAT3 deceleration, Model* mdl);

	// Update
	virtual void update(float dt);

	// Getters
	bool visible() const;
	bool collidable() const;
	bool shouldDrawBB() const;
	XMVECTOR getScale() const;
	XMVECTOR getLocalRotation() const;
	XMVECTOR getRotation() const;
	XMVECTOR getPosition() const;
	XMMATRIX getWorldMatrix() const;

	XMMATRIX getTranslationMatrix() const;
	int getWvpCBufferIndex() const;
	void setWvpCBufferIndex(int index);
	MovementComponent* getMoveCompPtr();
	PhysicsComponent* getphysicsCompPtr();
	BoundingBox getAABB();
	BoundingBox* getAABBPtr();
	Model* getModelPtr() const;
	std::wstring getTexturePath() const;

	void setTexturePath(std::wstring texturePath);


	// Setters
	void setDrawBB(bool drawable);
	void setVisibility(bool visible);
	void setIfCollidable(bool collidable);
	void setRotation(XMVECTOR newRotation);
	void setScale(XMVECTOR newScale);
	void setPosition(XMVECTOR newPosition);
	void setBoundingBox(XMFLOAT3 extends);
	void move(Direction dir, float dt);
	void move(XMVECTOR dir, float dt);

	bool m_removeMe;
	
};