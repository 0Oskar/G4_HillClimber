#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	this->m_collidable = false;
	this->m_isStatic = false;
	this->m_modelIndex = -1;
	this->m_wvpCBufferIndex = -1;

	this->m_movementComp = nullptr;
	this->m_physicsComp = nullptr;
}

GameObject::GameObject(const GameObject& otherGameObject)
{
	this->m_collidable = otherGameObject.m_collidable;
	this->m_isStatic = otherGameObject.m_isStatic;
	this->m_modelIndex = otherGameObject.m_modelIndex;
	this->m_wvpCBufferIndex = otherGameObject.m_wvpCBufferIndex;

	if (otherGameObject.m_movementComp != nullptr)
		this->m_movementComp = new MovementComponent(*otherGameObject.m_movementComp);
	else
		this->m_movementComp = nullptr;
	
	if (otherGameObject.m_physicsComp != nullptr)
		this->m_physicsComp = new PhysicsComponent(*otherGameObject.m_physicsComp);
	else
		this->m_physicsComp = nullptr;

}

GameObject::~GameObject()
{
	if (this->m_movementComp)
	{
		delete this->m_movementComp;
		this->m_movementComp = nullptr;
	}
	if (this->m_physicsComp)
	{
		delete this->m_physicsComp;
		this->m_physicsComp = nullptr;
	}
}

void GameObject::initializeStatic(bool collidable, int modelIndex, int wvpCBufferIndex)
{
	this->m_isStatic = true;
	this->m_collidable = collidable;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
	this->m_physicsComp->initialize(this->m_movementComp);
}

void GameObject::initializeDynamic(bool collidable, int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration)
{
	this->m_isStatic = false;
	this->m_collidable = collidable;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
	this->m_physicsComp->initialize(this->m_movementComp, mass, acceleration, deceleration);
}

void GameObject::update(float dt)
{
	this->m_physicsComp->updatePosition(dt);
}

DirectX::XMVECTOR GameObject::getPosition() const
{
	return this->m_movementComp->position;
}

DirectX::XMMATRIX GameObject::getWorldMatrix() const
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixScalingFromVector(this->m_movementComp->scale) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComp->rotation) *
		DirectX::XMMatrixTranslationFromVector(this->m_movementComp->position));
}

int GameObject::getModelIndex() const
{
	return this->m_modelIndex;
}

int GameObject::getWvpCBufferIndex() const
{
	return this->m_wvpCBufferIndex;
}

MovementComponent* GameObject::getMoveCompPtr()
{
	return this->m_movementComp;
}

void GameObject::setScale(DirectX::XMVECTOR newScale)
{
	this->m_movementComp->scale = newScale;
}

void GameObject::setPosition(DirectX::XMVECTOR newPosition)
{
	if (this->m_movementComp)
		this->m_movementComp->position = newPosition;
}

void GameObject::move(Direction dir, float dt)
{
	this->m_physicsComp->addForceDir(dir, dt);
}