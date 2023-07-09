#pragma once

#include "pch.h"
#include "MovementComponent.h"

const float GRAVITY = 0.982f;

class PhysicsComponent
{
private:
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_acceleration;
	DirectX::XMFLOAT3 m_deceleration;
	float m_mass;

	// Jump
	bool m_isJumping;
	bool m_isFalling;

	DirectX::BoundingBox* m_aabb;

	// Movement Component
	MovementComponent* m_moveComp;

public:
	PhysicsComponent()
	{
		m_velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_acceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_deceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		m_mass = 0.f;
		m_isJumping = false;
		m_isFalling = true;
		m_aabb = nullptr;
		m_moveComp = nullptr;
	}
	PhysicsComponent(const PhysicsComponent& otherPhysicsComponent)
	{
		m_velocity = otherPhysicsComponent.m_velocity;
		m_acceleration = otherPhysicsComponent.m_acceleration;
		m_deceleration = otherPhysicsComponent.m_deceleration;
		m_mass = otherPhysicsComponent.m_mass;
		m_isJumping = otherPhysicsComponent.m_isJumping;
		m_isFalling = otherPhysicsComponent.m_isFalling;

		// AABB
		if (m_aabb)
		{
			delete m_aabb;
			m_aabb = nullptr;
		}
		if (otherPhysicsComponent.m_aabb != nullptr)
			m_aabb = new DirectX::BoundingBox(*otherPhysicsComponent.m_aabb);
		
		// Movement Component
		m_moveComp = otherPhysicsComponent.m_moveComp;
	}
	~PhysicsComponent()
	{
		if (m_aabb)
		{
			delete m_aabb;
			m_aabb = nullptr;
		}
	}

	PhysicsComponent& operator=(const PhysicsComponent& otherPhysicsComponent)
	{
		if (this == &otherPhysicsComponent)
			return *this;

		m_velocity = otherPhysicsComponent.m_velocity;
		m_acceleration = otherPhysicsComponent.m_acceleration;
		m_deceleration = otherPhysicsComponent.m_deceleration;
		m_mass = otherPhysicsComponent.m_mass;
		m_isJumping = otherPhysicsComponent.m_isJumping;
		m_isFalling = otherPhysicsComponent.m_isFalling;

		// AABB
		if (m_aabb)
		{
			delete m_aabb;
			m_aabb = nullptr;
		}
		if (otherPhysicsComponent.m_aabb != nullptr)
			m_aabb = new DirectX::BoundingBox(*otherPhysicsComponent.m_aabb);

		// Movement Component
		m_moveComp = otherPhysicsComponent.m_moveComp;

		return *this;
	}

	// Initialization
	void initialize(MovementComponent* moveComp, float mass = 1.f, DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f))
	{
		m_moveComp = moveComp;
		m_mass = mass;
		m_acceleration = acceleration;
		m_deceleration = deceleration;
		m_aabb = new DirectX::BoundingBox();
	}

	// Getters
	DirectX::XMFLOAT3 getVelocity() const
	{
		return m_velocity;
	}
	DirectX::XMFLOAT3 getAcceleration() const
	{
		return m_acceleration;
	}
	DirectX::BoundingBox* getAABBPtr()
	{
		return m_aabb;
	}
	bool getIsJumping() const
	{
		return m_isJumping;
	}
	bool getIsFalling() const
	{
		return m_isFalling;
	}

	// Setters
	void setBoundingBox(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extends)
	{
		m_aabb->Center = center;
		m_aabb->Extents = extends;
	}
	void setVelocity(DirectX::XMFLOAT3 newVelocity)
	{
		m_velocity = newVelocity;
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}
	void setAcceleration(DirectX::XMFLOAT3 newAcceleration)
	{
		m_acceleration = newAcceleration;
	}
	void setIsJumping(bool isJumping)
	{
		m_isJumping = isJumping;
	}
	void setIsFalling(bool isFalling)
	{
		m_isFalling = isFalling;
	}

	void addForce(DirectX::XMFLOAT3 force, float dt)
	{
		m_velocity = DirectX::XMFLOAT3(m_velocity.x + force.x, m_velocity.y + force.y, m_velocity.z + force.z);
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}
	void addForceDir(Direction dir, float dt, float multiplier = 1.f)
	{
		DirectX::XMVECTOR finalForce = DirectX::XMVectorZero();

		switch (dir)
		{
		case Direction::FORWARD:
			finalForce = DirectX::XMVectorScale(m_moveComp->forward, m_acceleration.x * multiplier);
			break;
		case Direction::BACKWARD:
			finalForce = DirectX::XMVectorScale(m_moveComp->backward, m_acceleration.x * multiplier);
			break;
		case Direction::LEFT:
			finalForce = DirectX::XMVectorScale(m_moveComp->left, m_acceleration.x * multiplier);
			break;
		case Direction::RIGHT:
			finalForce = DirectX::XMVectorScale(m_moveComp->right, m_acceleration.x * multiplier);
			break;
		case Direction::UP:
			finalForce = DirectX::XMVectorScale(m_moveComp->up, m_acceleration.y * multiplier);
			break;
		case Direction::DOWN:
			finalForce = DirectX::XMVectorScale(m_moveComp->down, m_acceleration.y * multiplier);
			break;
		default:
			assert(!"Error, no valid direction found!");
			break;
		}

		DirectX::XMFLOAT3 finalForceF3 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMStoreFloat3(&finalForceF3, finalForce);

		m_velocity = DirectX::XMFLOAT3(m_velocity.x + finalForceF3.x, m_velocity.y + finalForceF3.y, m_velocity.z + finalForceF3.z);
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}
	void addForceDir(DirectX::XMVECTOR dir, float dt, float multiplier = 1.f)
	{
		DirectX::XMVECTOR finalForce = DirectX::XMVectorZero();

		
		finalForce = DirectX::XMVectorScale(dir, m_acceleration.x * multiplier);

		DirectX::XMFLOAT3 finalForceF3 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMStoreFloat3(&finalForceF3, finalForce);

		m_velocity = DirectX::XMFLOAT3(m_velocity.x + finalForceF3.x, m_velocity.y + finalForceF3.y, m_velocity.z + finalForceF3.z);
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}
	void addGravity(float dt)
	{
		m_velocity.y += m_mass * -GRAVITY * dt;
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}

	void addYDecel(float dt)
	{
		m_velocity.y *= m_deceleration.y * dt;
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}

	void jump(float accelerationMultipler, float dt)
	{
		if (!m_isJumping)
		{
			m_isJumping = true;
			m_isFalling = false;
			m_velocity.y += m_acceleration.y * accelerationMultipler;
			assert(!isnan(m_velocity.y));
		}
	}

	// Update
	void handleCollision(std::vector<DirectX::BoundingBox*> boundingBoxes, BoundingOrientedBox pyramidOBB,float dt, std::vector<DirectX::BoundingOrientedBox*> orientedBoundingBoxes)
	{
		DirectX::BoundingBox AABBNextFrame = *(m_aabb);
		AABBNextFrame.Center = DirectX::XMFLOAT3(m_aabb->Center.x + m_velocity.x * dt, m_aabb->Center.y + m_velocity.y * dt, m_aabb->Center.z + m_velocity.z * dt);

		// Pyramid Intersection
		if (pyramidOBB.Extents.x != 0.f &&
			pyramidOBB.Extents.y != 0.f &&
			pyramidOBB.Extents.z != 0.f)
		{
			if (AABBNextFrame.Intersects(pyramidOBB))
			{
				m_velocity.z = -20.f;
				m_velocity.y = -21.f;
				m_isFalling = true;
			}
		}

		DirectX::BoundingBox xAABB = *(m_aabb);
		xAABB.Center = DirectX::XMFLOAT3(m_aabb->Center.x + m_velocity.x * dt, m_aabb->Center.y, m_aabb->Center.z);
		
		DirectX::BoundingBox yAABB = *(m_aabb);
		yAABB.Center = DirectX::XMFLOAT3(m_aabb->Center.x, m_aabb->Center.y + m_velocity.y * dt, m_aabb->Center.z);
		
		DirectX::BoundingBox zAABB = *(m_aabb);
		zAABB.Center = DirectX::XMFLOAT3(m_aabb->Center.x, m_aabb->Center.y, m_aabb->Center.z + m_velocity.z * dt);
		
		for (size_t i = 0; i < boundingBoxes.size(); i++)
		{
			if (AABBNextFrame.Intersects(*boundingBoxes[i]))
			{
				if (xAABB.Intersects(*boundingBoxes[i]))
				{
					if (m_velocity.x > 0)
						m_moveComp->position = DirectX::XMVectorSetX(m_moveComp->position, boundingBoxes[i]->Center.x - boundingBoxes[i]->Extents.x - AABBNextFrame.Extents.x - 0.0001f);
					else
						m_moveComp->position = DirectX::XMVectorSetX(m_moveComp->position, boundingBoxes[i]->Center.x + boundingBoxes[i]->Extents.x + AABBNextFrame.Extents.x + 0.0001f);
					
					m_velocity.x = 0;
				}

				if (yAABB.Intersects(*boundingBoxes[i]))
				{
					if (m_velocity.y > 0)
						m_moveComp->position = DirectX::XMVectorSetY(m_moveComp->position, boundingBoxes[i]->Center.y - boundingBoxes[i]->Extents.y - AABBNextFrame.Extents.y - 0.0001f);
					else
					{
						m_moveComp->position = DirectX::XMVectorSetY(m_moveComp->position, boundingBoxes[i]->Center.y + boundingBoxes[i]->Extents.y + AABBNextFrame.Extents.y + 0.0001f);
						m_isJumping = false;
						m_isFalling = false;
					}
					
					m_velocity.y = 0;
				}
				else
				{
					m_isFalling = true;
				}

				if (zAABB.Intersects(*boundingBoxes[i]))
				{
					if (m_velocity.z > 0)
						m_moveComp->position = DirectX::XMVectorSetZ(m_moveComp->position, boundingBoxes[i]->Center.z - boundingBoxes[i]->Extents.z - AABBNextFrame.Extents.z - 0.0001f);
					else
						m_moveComp->position = DirectX::XMVectorSetZ(m_moveComp->position, boundingBoxes[i]->Center.z + boundingBoxes[i]->Extents.z + AABBNextFrame.Extents.z + 0.0001f);
					
					m_velocity.z = 0;
				}
			}
		}
		assert(!isnan(m_velocity.x));
		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}
	
	void updatePosition(float dt, bool isCamera = false)
	{
		m_moveComp->position = DirectX::XMVectorAdd(
			m_moveComp->position, 
			DirectX::XMVectorSet(
				m_velocity.x * dt, 
				m_velocity.y * dt, 
				m_velocity.z * dt, 1.f)
		);
		assert(!DirectX::XMVector3IsNaN(m_moveComp->position));

		m_aabb->Center = m_moveComp->getPositionF3();

		if (isCamera)
			m_moveComp->updateViewMatrix();
		else
			m_moveComp->updateDirVectors();

		m_velocity.x *= m_deceleration.x * dt;
		m_velocity.z *= m_deceleration.z * dt;

		assert(!isnan(m_velocity.y));
		assert(!isnan(m_velocity.z));
	}
	void updatePositionNoDecel(float dt, bool isCamera = false)
	{
		m_moveComp->position = DirectX::XMVectorAdd(
			m_moveComp->position,
			DirectX::XMVectorSet(
				m_velocity.x * dt,
				m_velocity.y * dt,
				m_velocity.z * dt, 1.f)
		);
		assert(!DirectX::XMVector3IsNaN(m_moveComp->position));
		m_aabb->Center = m_moveComp->getPositionF3();

		if (isCamera)
			m_moveComp->updateViewMatrix();
		else
			m_moveComp->updateDirVectors();
	}
};