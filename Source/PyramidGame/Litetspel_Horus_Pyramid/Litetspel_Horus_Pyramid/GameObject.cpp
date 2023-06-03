#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
	m_visible = true;
	m_collidable = false;
	m_isStatic = false;
	m_drawBB = false;
	m_useDeceleration = true;
	m_wvpCBufferIndex = -1;

	m_movementComp = nullptr;
	m_physicsComp = nullptr;
	m_modelPtr = nullptr;
	m_removeMe = false;
}

GameObject::GameObject(const GameObject& otherGameObject)
{
	m_visible = otherGameObject.m_visible;
	m_collidable = otherGameObject.m_collidable;
	m_isStatic = otherGameObject.m_isStatic;
	m_drawBB = otherGameObject.m_drawBB;
	m_useDeceleration = otherGameObject.m_useDeceleration;
	m_wvpCBufferIndex = otherGameObject.m_wvpCBufferIndex;
	m_modelPtr = otherGameObject.m_modelPtr;

	// Movement Component
	if (m_movementComp)
	{
		delete m_movementComp;
		m_movementComp = nullptr;
	}
	if (otherGameObject.m_movementComp != nullptr)
		m_movementComp = new MovementComponent(*otherGameObject.m_movementComp);
	
	// Physics Component
	if (m_physicsComp)
	{
		delete m_physicsComp;
		m_physicsComp = nullptr;
	}
	if (otherGameObject.m_physicsComp != nullptr)
		m_physicsComp = new PhysicsComponent(*otherGameObject.m_physicsComp);
}

GameObject::~GameObject()
{
	if (m_movementComp)
	{
		delete m_movementComp;
		m_movementComp = nullptr;
	}
	if (m_physicsComp)
	{
		delete m_physicsComp;
		m_physicsComp = nullptr;
	}
}

GameObject& GameObject::operator=(const GameObject& otherGameObject)
{
	if (this == &otherGameObject)
		return *this;

	m_visible = otherGameObject.m_visible;
	m_collidable = otherGameObject.m_collidable;
	m_isStatic = otherGameObject.m_isStatic;
	m_drawBB = otherGameObject.m_drawBB;
	m_useDeceleration = otherGameObject.m_useDeceleration;
	m_wvpCBufferIndex = otherGameObject.m_wvpCBufferIndex;
	m_modelPtr = otherGameObject.m_modelPtr;

	// Movement Component
	if (m_movementComp)
	{
		delete m_movementComp;
		m_movementComp = nullptr;
	}
	if (otherGameObject.m_movementComp != nullptr)
		m_movementComp = new MovementComponent(*otherGameObject.m_movementComp);

	// Physics Component
	if (m_physicsComp)
	{
		delete m_physicsComp;
		m_physicsComp = nullptr;
	}
	if (otherGameObject.m_physicsComp != nullptr)
		m_physicsComp = new PhysicsComponent(*otherGameObject.m_physicsComp);

	return *this;
}

void GameObject::initializeStatic(bool collidable, int wvpCBufferIndex, Model* mdl)
{
	m_collidable = collidable;
	m_isStatic = true;
	m_drawBB = collidable;
	m_useDeceleration = false;
	m_wvpCBufferIndex = wvpCBufferIndex;

	m_movementComp = new MovementComponent();
	m_physicsComp = new PhysicsComponent();
	m_physicsComp->initialize(m_movementComp);
	m_modelPtr = mdl;

	m_texturePath = m_modelPtr->m_texturePath;
}

void GameObject::initializeDynamic(bool collidable, bool useDeceleration, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, Model* mdl)
{
	m_collidable = collidable;
	m_isStatic = false;
	m_drawBB = collidable;
	m_useDeceleration = useDeceleration;
	m_wvpCBufferIndex = wvpCBufferIndex;

	m_movementComp = new MovementComponent();
	m_physicsComp = new PhysicsComponent();
	m_physicsComp->initialize(m_movementComp, mass, acceleration, deceleration);
	m_modelPtr = mdl;

	m_texturePath = m_modelPtr->m_texturePath;
}

void GameObject::update(float dt)
{
	if (m_useDeceleration)
		m_physicsComp->updatePosition(dt);
	else
		m_physicsComp->updatePositionNoDecel(dt);
}

bool GameObject::visible() const
{
	return m_visible;
}

bool GameObject::collidable() const
{
	return m_collidable;
}

bool GameObject::getDrawBB() const
{
	return m_drawBB;
}

XMVECTOR GameObject::getRotation() const
{
	return m_movementComp->rotation;
}

DirectX::XMVECTOR GameObject::getPosition() const
{
	return m_movementComp->position;
}

DirectX::XMMATRIX GameObject::getWorldMatrix() const
{
	return DirectX::XMMATRIX(
		DirectX::XMMatrixScalingFromVector(m_movementComp->scale) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(m_movementComp->localRotation) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(m_movementComp->rotation) *
		DirectX::XMMatrixTranslationFromVector(m_movementComp->position));
}


DirectX::XMMATRIX GameObject::getTranslationMatrix() const
{
	return DirectX::XMMatrixTranslationFromVector(m_movementComp->position);
}

int GameObject::getWvpCBufferIndex() const
{
	return m_wvpCBufferIndex;
}

void GameObject::setWvpCBufferIndex(int index)
{
	m_wvpCBufferIndex = index;
}

MovementComponent* GameObject::getMoveCompPtr()
{
	return m_movementComp;
}

PhysicsComponent* GameObject::getphysicsCompPtr()
{
	return m_physicsComp;
}

DirectX::BoundingBox GameObject::getAABB()
{
	return *(m_physicsComp->getAABBPtr());
}

DirectX::BoundingBox* GameObject::getAABBPtr()
{
	return m_physicsComp->getAABBPtr();
}

Model* GameObject::getModelPtr() const
{
	return m_modelPtr;
}

void GameObject::setVisibility(bool visible)
{
	m_visible = visible;
}

void GameObject::setIfCollidable(bool collidable)
{
	m_collidable = collidable;
}

void GameObject::setRotation(DirectX::XMVECTOR newRotation)
{
	if (m_movementComp)
	{
		m_movementComp->rotation = newRotation;
		m_movementComp->updateDirVectors();
	}
}

std::wstring GameObject::getTexturePath() const
{
	return m_texturePath;
}

void GameObject::setTexturePath(std::wstring texturePath)
{
	m_texturePath = texturePath;
}

void GameObject::setDrawBB(bool drawable)
{
	m_drawBB = drawable;
}

void GameObject::setScale(DirectX::XMVECTOR newScale)
{
	if (m_movementComp)
		m_movementComp->scale = newScale;
}

void GameObject::setPosition(DirectX::XMVECTOR newPosition)
{
	if (m_movementComp)
	{
		m_movementComp->position = newPosition;
		if (m_physicsComp)
			m_physicsComp->getAABBPtr()->Center = m_movementComp->getPositionF3();
	}
}

void GameObject::setBoundingBox(DirectX::XMFLOAT3 extends)
{
	m_physicsComp->setBoundingBox(m_movementComp->getPositionF3(), extends);
}

void GameObject::move(Direction dir, float dt)
{
	m_physicsComp->addForceDir(dir, dt);
}

void GameObject::move(DirectX::XMVECTOR dir, float dt)
{
	m_physicsComp->addForceDir(dir, dt);
}