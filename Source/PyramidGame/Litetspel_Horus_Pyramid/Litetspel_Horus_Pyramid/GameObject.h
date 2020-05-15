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
	bool m_drawBB;
	bool m_useDeceleration;
	int m_modelIndex;
	int m_wvpCBufferIndex;
	
	// Componenets
	MovementComponent* m_movementComp;
	PhysicsComponent* m_physicsComp;
	Model* m_modelptr;
	std::wstring m_texturePath;

public:
	GameObject();
	GameObject(const GameObject& otherGameObject);
	virtual ~GameObject();
	GameObject& operator=(const GameObject& otherGameObject);

	// Initialization
	void initializeStatic(bool collidable, int modelIndex, int wvpCBufferIndex, Model* mdl);
	void initializeDynamic(bool collidable, bool useDeceleration, int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, Model* mdl);

	// Update
	virtual void update(float dt);

	// Getters
	bool visible() const;
	bool collidable() const;
	bool getDrawBB() const;
	DirectX::XMVECTOR getPosition() const;
	DirectX::XMMATRIX getWorldMatrix() const;

	DirectX::XMMATRIX getTranslationMatrix() const;
	int getModelIndex() const;
	int getWvpCBufferIndex() const;
	void setWvpCBufferIndex(int index);
	MovementComponent* getMoveCompPtr();
	PhysicsComponent* getphysicsCompPtr();
	DirectX::BoundingBox getAABB();
	DirectX::BoundingBox* getAABBPtr();
	std::wstring getTexturePath();

	void setTexturePath(std::wstring texturePath);


	// Setters
	void setDrawBB(bool drawable);
	void setVisibility(bool visible);
	void setIfCollidable(bool collidable);
	void setRotation(DirectX::XMVECTOR newRotation);
	void setScale(DirectX::XMVECTOR newScale);
	void setPosition(DirectX::XMVECTOR newPosition);
	void setBoundingBox(DirectX::XMFLOAT3 extends);
	void move(Direction dir, float dt);
	void move(DirectX::XMVECTOR dir, float dt);

	bool m_removeMe;
	
};