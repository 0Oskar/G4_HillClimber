#pragma once

#include "pch.h"
#include "MovementComponent.h"

class PhysicsComponent
{
private:
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_acceleration;
	DirectX::XMFLOAT3 m_deceleration;
	float m_mass;

	DirectX::BoundingBox aabb;

	// Movement Component
	MovementComponent* m_moveComp;

public:
	PhysicsComponent()
	{
		this->m_velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		this->m_acceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		this->m_deceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		this->m_mass = 0.f;
		this->m_moveComp = nullptr;
	}
	PhysicsComponent(const PhysicsComponent& otherPhysicsComponent)
	{
		this->m_velocity = otherPhysicsComponent.m_velocity;
		this->m_acceleration = otherPhysicsComponent.m_acceleration;
		this->m_deceleration = otherPhysicsComponent.m_deceleration;
		this->m_mass = otherPhysicsComponent.m_mass;
		
		if (otherPhysicsComponent.m_moveComp != nullptr)
			this->m_moveComp = new MovementComponent(*otherPhysicsComponent.m_moveComp);
		else
			this->m_moveComp = nullptr;
	}
	~PhysicsComponent() {}

	PhysicsComponent& operator=(const PhysicsComponent& otherPhysicsComponent)
	{
		this->m_velocity = otherPhysicsComponent.m_velocity;
		this->m_acceleration = otherPhysicsComponent.m_acceleration;
		this->m_deceleration = otherPhysicsComponent.m_deceleration;
		this->m_mass = otherPhysicsComponent.m_mass;
		*(this->m_moveComp) = *otherPhysicsComponent.m_moveComp;
		return *this;
	}

	// Initialization
	void initialize(MovementComponent* moveComp, float mass = 1.f, DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f))
	{
		this->m_moveComp = moveComp;
		this->m_mass = mass;
		this->m_acceleration = acceleration;
		this->m_deceleration = deceleration;
	}

	// Getters
	DirectX::XMFLOAT3 getVelocity() const
	{
		return this->m_velocity;
	}
	DirectX::XMFLOAT3 getAcceleration() const
	{
		return this->m_acceleration;
	}

	// Setters
	void setVelocity(DirectX::XMFLOAT3 newVelocity)
	{
		this->m_velocity = newVelocity;
	}
	void setAcceleration(DirectX::XMFLOAT3 newAcceleration)
	{
		this->m_acceleration = newAcceleration;
	}

	void addForce(DirectX::XMFLOAT3 force)
	{
		this->m_velocity = DirectX::XMFLOAT3(this->m_velocity.x + force.x, this->m_velocity.y + force.y, this->m_velocity.z + force.z);
	}

	void addForceDir(Direction dir, float dt)
	{
		DirectX::XMVECTOR finalForce = DirectX::XMVectorZero();

		switch (dir)
		{
		case Direction::FORWARD:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->forward, this->m_acceleration.x * dt);
			break;
		case Direction::BACKWARD:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->backward, this->m_acceleration.x * dt);
			break;
		case Direction::LEFT:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->left, this->m_acceleration.x * dt);
			break;
		case Direction::RIGHT:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->right, this->m_acceleration.x * dt);
			break;
		case Direction::UP:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->up, this->m_acceleration.y * dt);
			break;
		case Direction::DOWN:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->down, this->m_acceleration.y * dt);
			break;
		default:
			assert(!"Error, no valid direction found!");
			break;
		}

		DirectX::XMFLOAT3 finalForceF3 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMStoreFloat3(&finalForceF3, finalForce);

		this->m_velocity = DirectX::XMFLOAT3(this->m_velocity.x + finalForceF3.x, this->m_velocity.y + finalForceF3.y, this->m_velocity.z + finalForceF3.z);
	}

	// Update
	void updatePosition(float dt)
	{
		this->m_moveComp->position = DirectX::XMVectorAdd(
			this->m_moveComp->position, 
			DirectX::XMVectorSet(
				this->m_velocity.x * dt, 
				this->m_velocity.y * dt, 
				this->m_velocity.z * dt, 1.f)
		);

		this->m_moveComp->updateViewMatrix();

		this->m_velocity.x *= this->m_deceleration.x;
		this->m_velocity.y *= this->m_deceleration.y;
		this->m_velocity.z *= this->m_deceleration.z;
	}
};