#pragma once

#include "pch.h"
#include "MovementComponent.h"

const float GRAVITY = 0.01f; // 9.82f
const float MAX_GRAVITY = 5.f;

class PhysicsComponent
{
private:
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_acceleration;
	DirectX::XMFLOAT3 m_deceleration;
	float m_mass;

	// Jump
	bool m_isJumping;

	DirectX::BoundingBox* m_aabb;

	// Movement Component
	MovementComponent* m_moveComp;

public:
	PhysicsComponent()
	{
		this->m_velocity = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		this->m_acceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		this->m_deceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		this->m_mass = 0.f;
		this->m_isJumping = false;
		this->m_aabb = nullptr;
		this->m_moveComp = nullptr;
	}
	PhysicsComponent(const PhysicsComponent& otherPhysicsComponent)
	{
		this->m_velocity = otherPhysicsComponent.m_velocity;
		this->m_acceleration = otherPhysicsComponent.m_acceleration;
		this->m_deceleration = otherPhysicsComponent.m_deceleration;
		this->m_mass = otherPhysicsComponent.m_mass;
		this->m_isJumping = otherPhysicsComponent.m_isJumping;

		// AABB
		if (this->m_aabb)
		{
			delete this->m_aabb;
			this->m_aabb = nullptr;
		}
		if (otherPhysicsComponent.m_aabb != nullptr)
			this->m_aabb = new DirectX::BoundingBox(*otherPhysicsComponent.m_aabb);
		
		// Movement Component
		this->m_moveComp = otherPhysicsComponent.m_moveComp;
	}
	~PhysicsComponent()
	{
		if (this->m_aabb)
		{
			delete this->m_aabb;
			this->m_aabb = nullptr;
		}
	}

	PhysicsComponent& operator=(const PhysicsComponent& otherPhysicsComponent)
	{
		if (this == &otherPhysicsComponent)
			return *this;

		this->m_velocity = otherPhysicsComponent.m_velocity;
		this->m_acceleration = otherPhysicsComponent.m_acceleration;
		this->m_deceleration = otherPhysicsComponent.m_deceleration;
		this->m_mass = otherPhysicsComponent.m_mass;
		this->m_isJumping = otherPhysicsComponent.m_isJumping;

		// AABB
		if (this->m_aabb)
		{
			delete this->m_aabb;
			this->m_aabb = nullptr;
		}
		if (otherPhysicsComponent.m_aabb != nullptr)
			this->m_aabb = new DirectX::BoundingBox(*otherPhysicsComponent.m_aabb);

		// Movement Component
		this->m_moveComp = otherPhysicsComponent.m_moveComp;

		return *this;
	}

	// Initialization
	void initialize(MovementComponent* moveComp, float mass = 1.f, DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(0.f, 0.f, 0.f))
	{
		this->m_moveComp = moveComp;
		this->m_mass = mass;
		this->m_acceleration = acceleration;
		this->m_deceleration = deceleration;
		this->m_aabb = new DirectX::BoundingBox();
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
	DirectX::BoundingBox* getAABBPtr()
	{
		return this->m_aabb;
	}

	// Setters
	void setBoundingBox(DirectX::XMFLOAT3 center, DirectX::XMFLOAT3 extends)
	{
		this->m_aabb->Center = center;
		this->m_aabb->Extents = extends;
	}
	void setVelocity(DirectX::XMFLOAT3 newVelocity)
	{
		this->m_velocity = newVelocity;
	}
	void setAcceleration(DirectX::XMFLOAT3 newAcceleration)
	{
		this->m_acceleration = newAcceleration;
	}

	void addForce(DirectX::XMFLOAT3 force, float dt)
	{
		this->m_velocity = DirectX::XMFLOAT3(this->m_velocity.x + (force.x * dt), this->m_velocity.y + (force.y * dt), this->m_velocity.z + (force.z * dt));
	}

	void addForceDir(Direction dir, float dt, float multiplier = 1.f)
	{
		DirectX::XMVECTOR finalForce = DirectX::XMVectorZero();

		switch (dir)
		{
		case Direction::FORWARD:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->forward, this->m_acceleration.x * multiplier * dt);
			break;
		case Direction::BACKWARD:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->backward, this->m_acceleration.x * multiplier * dt);
			break;
		case Direction::LEFT:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->left, this->m_acceleration.x * multiplier * dt);
			break;
		case Direction::RIGHT:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->right, this->m_acceleration.x * multiplier * dt);
			break;
		case Direction::UP:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->up, this->m_acceleration.y * multiplier * dt);
			break;
		case Direction::DOWN:
			finalForce = DirectX::XMVectorScale(this->m_moveComp->down, this->m_acceleration.y * multiplier * dt);
			break;
		default:
			assert(!"Error, no valid direction found!");
			break;
		}

		DirectX::XMFLOAT3 finalForceF3 = DirectX::XMFLOAT3(0.f, 0.f, 0.f);
		DirectX::XMStoreFloat3(&finalForceF3, finalForce);

		this->m_velocity = DirectX::XMFLOAT3(this->m_velocity.x + finalForceF3.x, this->m_velocity.y + finalForceF3.y, this->m_velocity.z + finalForceF3.z);
	}

	void addGravity(float dt)
	{
		float force = this->m_mass * -GRAVITY * dt;
		if (this->m_velocity.y > -MAX_GRAVITY * dt)
			this->m_velocity.y += force;
	}

	void jump(float accelerationMultipler, float dt)
	{
		if (!this->m_isJumping)
		{
			this->m_isJumping = true;
			this->m_velocity.y += this->m_acceleration.y * accelerationMultipler * dt;
		}
	}

	// Update
	void handleCollision(std::vector<DirectX::BoundingBox*> boundingBoxes, float dt)
	{
		DirectX::BoundingBox AABBNextFrame = *(this->m_aabb);
		AABBNextFrame.Center = DirectX::XMFLOAT3(this->m_aabb->Center.x + this->m_velocity.x * dt, this->m_aabb->Center.y + this->m_velocity.y * dt, this->m_aabb->Center.z + this->m_velocity.z * dt);

		DirectX::BoundingBox xAABB = *(this->m_aabb);
		xAABB.Center = DirectX::XMFLOAT3(this->m_aabb->Center.x + this->m_velocity.x * dt, this->m_aabb->Center.y, this->m_aabb->Center.z);
		
		DirectX::BoundingBox yAABB = *(this->m_aabb);
		yAABB.Center = DirectX::XMFLOAT3(this->m_aabb->Center.x, this->m_aabb->Center.y + this->m_velocity.y * dt, this->m_aabb->Center.z);
		
		DirectX::BoundingBox zAABB = *(this->m_aabb);
		zAABB.Center = DirectX::XMFLOAT3(this->m_aabb->Center.x, this->m_aabb->Center.y, this->m_aabb->Center.z + this->m_velocity.z * dt);
		
		for (size_t i = 0; i < boundingBoxes.size(); i++)
		{
			if (AABBNextFrame.Intersects(*boundingBoxes[i]))
			{
				if (xAABB.Intersects(*boundingBoxes[i]))
				{
					//OutputDebugString(L"X Collision!\n");
					if (this->m_velocity.x > 0)
						this->m_moveComp->position = DirectX::XMVectorSetX(this->m_moveComp->position, boundingBoxes[i]->Center.x - boundingBoxes[i]->Extents.x - AABBNextFrame.Extents.x - 0.0001f);
					else
						this->m_moveComp->position = DirectX::XMVectorSetX(this->m_moveComp->position, boundingBoxes[i]->Center.x + boundingBoxes[i]->Extents.x + AABBNextFrame.Extents.x + 0.0001f);
					
					this->m_velocity.x = 0;
				}

				if (yAABB.Intersects(*boundingBoxes[i]))
				{
					//OutputDebugString(L"Y Collision!\n");
					if (this->m_velocity.y > 0)
						this->m_moveComp->position = DirectX::XMVectorSetY(this->m_moveComp->position, boundingBoxes[i]->Center.y - boundingBoxes[i]->Extents.y - AABBNextFrame.Extents.y - 0.0001f);
					else
					{
						this->m_moveComp->position = DirectX::XMVectorSetY(this->m_moveComp->position, boundingBoxes[i]->Center.y + boundingBoxes[i]->Extents.y + AABBNextFrame.Extents.y + 0.0001f);
						this->m_isJumping = false;
					}
					
					this->m_velocity.y = 0;
				}

				if (zAABB.Intersects(*boundingBoxes[i]))
				{
					//OutputDebugString(L"Z Collision!\n");
					if (this->m_velocity.z > 0)
						this->m_moveComp->position = DirectX::XMVectorSetZ(this->m_moveComp->position, boundingBoxes[i]->Center.z - boundingBoxes[i]->Extents.z - AABBNextFrame.Extents.z - 0.0001f);
					else
						this->m_moveComp->position = DirectX::XMVectorSetZ(this->m_moveComp->position, boundingBoxes[i]->Center.z + boundingBoxes[i]->Extents.z + AABBNextFrame.Extents.z + 0.0001f);
					
					this->m_velocity.z = 0;
				}
			}
		}
	}
	void updatePosition(float dt)
	{
		this->m_moveComp->position = DirectX::XMVectorAdd(
			this->m_moveComp->position, 
			DirectX::XMVectorSet(
				this->m_velocity.x * dt, 
				this->m_velocity.y * dt, 
				this->m_velocity.z * dt, 1.f)
		);
		this->m_aabb->Center = this->m_moveComp->getPositionF3();

		this->m_moveComp->updateViewMatrix();

		this->m_velocity.x *= this->m_deceleration.x;
		this->m_velocity.z *= this->m_deceleration.z;
	}
};