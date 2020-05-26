#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	this->m_visible = true;
	this->m_collidable = false;
	this->m_isStatic = false;
	this->m_drawBB = false;
	this->m_useDeceleration = true;
	this->m_modelIndex = -1;
	this->m_wvpCBufferIndex = -1;

	this->m_movementComp = nullptr;
	this->m_physicsComp = nullptr;
	this->m_modelptr = nullptr;
	this->m_removeMe = false;
}

GameObject::GameObject(const GameObject& otherGameObject)
{
	this->m_visible = otherGameObject.m_visible;
	this->m_collidable = otherGameObject.m_collidable;
	this->m_isStatic = otherGameObject.m_isStatic;
	this->m_drawBB = otherGameObject.m_drawBB;
	this->m_useDeceleration = otherGameObject.m_useDeceleration;
	this->m_modelIndex = otherGameObject.m_modelIndex;
	this->m_wvpCBufferIndex = otherGameObject.m_wvpCBufferIndex;
	this->m_modelptr = otherGameObject.m_modelptr;

	// Movement Component
	if (this->m_movementComp)
	{
		delete this->m_movementComp;
		this->m_movementComp = nullptr;
	}
	if (otherGameObject.m_movementComp != nullptr)
		this->m_movementComp = new MovementComponent(*otherGameObject.m_movementComp);
	
	// Physics Component
	if (this->m_physicsComp)
	{
		delete this->m_physicsComp;
		this->m_physicsComp = nullptr;
	}
	if (otherGameObject.m_physicsComp != nullptr)
		this->m_physicsComp = new PhysicsComponent(*otherGameObject.m_physicsComp);
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

GameObject& GameObject::operator=(const GameObject& otherGameObject)
{
	if (this == &otherGameObject)
		return *this;

	this->m_visible = otherGameObject.m_visible;
	this->m_collidable = otherGameObject.m_collidable;
	this->m_isStatic = otherGameObject.m_isStatic;
	this->m_drawBB = otherGameObject.m_drawBB;
	this->m_useDeceleration = otherGameObject.m_useDeceleration;
	this->m_modelIndex = otherGameObject.m_modelIndex;
	this->m_wvpCBufferIndex = otherGameObject.m_wvpCBufferIndex;
	this->m_modelptr = otherGameObject.m_modelptr;

	// Movement Component
	if (this->m_movementComp)
	{
		delete this->m_movementComp;
		this->m_movementComp = nullptr;
	}
	if (otherGameObject.m_movementComp != nullptr)
		this->m_movementComp = new MovementComponent(*otherGameObject.m_movementComp);

	// Physics Component
	if (this->m_physicsComp)
	{
		delete this->m_physicsComp;
		this->m_physicsComp = nullptr;
	}
	if (otherGameObject.m_physicsComp != nullptr)
		this->m_physicsComp = new PhysicsComponent(*otherGameObject.m_physicsComp);

	return *this;
}

void GameObject::initializeStatic(bool collidable, int modelIndex, int wvpCBufferIndex, Model* mdl)
{
	this->m_collidable = collidable;
	this->m_isStatic = true;
	this->m_drawBB = collidable;
	this->m_useDeceleration = false;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
	this->m_physicsComp->initialize(this->m_movementComp);
	this->m_modelptr = mdl;

	this->m_texturePath = this->m_modelptr->m_originalTexture;
}

void GameObject::initializeDynamic(bool collidable, bool useDeceleration, int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, Model* mdl)
{
	this->m_collidable = collidable;
	this->m_isStatic = false;
	this->m_drawBB = collidable;
	this->m_useDeceleration = useDeceleration;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
	this->m_physicsComp->initialize(this->m_movementComp, mass, acceleration, deceleration);
	this->m_modelptr = mdl;

	this->m_texturePath = this->m_modelptr->m_originalTexture;
}

void GameObject::update(float dt)
{
	if (this->m_useDeceleration)
		this->m_physicsComp->updatePosition(dt);
	else
		this->m_physicsComp->updatePositionNoDecel(dt);
}

bool GameObject::visible() const
{
	return this->m_visible;
}

bool GameObject::collidable() const
{
	return this->m_collidable;
}

bool GameObject::getDrawBB() const
{
	return this->m_drawBB;
}

XMVECTOR GameObject::getRotation() const
{
	return this->m_movementComp->rotation;
}

DirectX::XMVECTOR GameObject::getPosition() const
{
	return this->m_movementComp->position;
}

DirectX::XMMATRIX GameObject::getWorldMatrix() const
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixScalingFromVector(this->m_movementComp->scale) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComp->localRotation) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComp->rotation) *
		DirectX::XMMatrixTranslationFromVector(this->m_movementComp->position));
}


DirectX::XMMATRIX GameObject::getTranslationMatrix() const
{
	return DirectX::XMMatrixTranslationFromVector(this->m_movementComp->position);
}

int GameObject::getModelIndex() const
{
	return this->m_modelIndex;
}

int GameObject::getWvpCBufferIndex() const
{
	return this->m_wvpCBufferIndex;
}

void GameObject::setWvpCBufferIndex(int index)
{
	this->m_wvpCBufferIndex = index;
}

MovementComponent* GameObject::getMoveCompPtr()
{
	return this->m_movementComp;
}

PhysicsComponent* GameObject::getphysicsCompPtr()
{
	return this->m_physicsComp;
}

DirectX::BoundingBox GameObject::getAABB()
{
	return *(this->m_physicsComp->getAABBPtr());
}

DirectX::BoundingBox* GameObject::getAABBPtr()
{
	return this->m_physicsComp->getAABBPtr();
}

void GameObject::setVisibility(bool visible)
{
	this->m_visible = visible;
}

void GameObject::setIfCollidable(bool collidable)
{
	this->m_collidable = collidable;
}

void GameObject::setRotation(DirectX::XMVECTOR newRotation)
{
	if (this->m_movementComp)
	{
		this->m_movementComp->rotation = newRotation;
		this->m_movementComp->updateDirVectors();
	}
}

std::wstring GameObject::getTexturePath()
{
	return this->m_texturePath;
}

void GameObject::setTexturePath(std::wstring texturePath)
{
	this->m_texturePath = texturePath;
}

void GameObject::setDrawBB(bool drawable)
{
	this->m_drawBB = drawable;
}

void GameObject::setScale(DirectX::XMVECTOR newScale)
{
	if (this->m_movementComp)
		this->m_movementComp->scale = newScale;
}

void GameObject::setPosition(DirectX::XMVECTOR newPosition)
{
	if (this->m_movementComp)
	{
		this->m_movementComp->position = newPosition;
		if (this->m_physicsComp)
			this->m_physicsComp->getAABBPtr()->Center = this->m_movementComp->getPositionF3();
	}
}

void GameObject::setBoundingBox(DirectX::XMFLOAT3 extends)
{
	this->m_physicsComp->setBoundingBox(this->m_movementComp->getPositionF3(), extends);
}

void GameObject::move(Direction dir, float dt)
{
	this->m_physicsComp->addForceDir(dir, dt);
}

void GameObject::move(DirectX::XMVECTOR dir, float dt)
{
	this->m_physicsComp->addForceDir(dir, dt);
}