#include "pch.h"
#include "followingEnemy.h"

followingEnemy::followingEnemy()
{
	this->m_hasColided = false;
	this->reachedEdge = true;
	this->scorpionSpawnPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

void followingEnemy::init(bool colidable, int modelIndex, int wvpCBufferIndex, Model* mdl, Player *player)
{
	this->initializeDynamic(colidable, false, modelIndex, wvpCBufferIndex, 16, DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), mdl);
	this->thePlayer = player;


	XMFLOAT3 scorpionPos;
	XMStoreFloat3(&scorpionPos, this->getMoveCompPtr()->position);

	XMFLOAT4 scorpionRot;
	XMStoreFloat4(&scorpionRot, this->getMoveCompPtr()->rotation);

	this->scorpionBB = BoundingOrientedBox(scorpionPos, XMFLOAT3(1.2f, 2.f, 1.2f), scorpionRot);	
}

void followingEnemy::update(float dt)
{	
	followPlayer(dt);
	this->scorpionBB.Center = XMFLOAT3(this->getMoveCompPtr()->position.m128_f32[0], this->getMoveCompPtr()->position.m128_f32[1], this->getMoveCompPtr()->position.m128_f32[2]);
	XMVECTOR aRotation = XMQuaternionRotationRollPitchYawFromVector(this->getMoveCompPtr()->rotation);
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, aRotation);
	this->scorpionBB.Orientation = rot;
}
	

void followingEnemy::onPlayerColide()
{

}

void followingEnemy::setReachedEdge(bool aValue)
{
	this->reachedEdge = aValue;
}

BoundingOrientedBox* followingEnemy::getBB()
{
	return &scorpionBB;
}

bool followingEnemy::getReachedEdge()
{
	return this->reachedEdge;
}

void followingEnemy::followPlayer(float dt)
{
	XMVECTOR walkDirection;
	XMVECTOR playerPosition;
	XMVECTOR previousPosition;
	XMVECTOR scorpionPos = this->getMoveCompPtr()->position;

	if (this->getSpawnPosOnce == false)
	{
		this->scorpionSpawnPos = this->getMoveCompPtr()->position;
		this->getSpawnPosOnce = true;
	}
	

	XMVECTOR upDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR playerRotation = this->thePlayer->getMoveCompPtr()->rotation;
	float playerRotationY = XMVectorGetY(playerRotation);

	playerPosition = this->thePlayer->getMoveCompPtr()->position;
	playerPosition = XMVectorSetY(playerPosition, 5.f);
	previousPosition = playerPosition;


	XMVECTOR walkBackDirection;
	walkBackDirection = this->scorpionSpawnPos - scorpionPos;
	walkBackDirection = XMVector3Normalize(walkBackDirection);

	walkDirection = playerPosition -scorpionPos;
	walkDirection = XMVector3Normalize(walkDirection);

	if (this-> reachedEdge == false)
	{
		this->getMoveCompPtr()->position += (walkDirection * dt * 12);


		float targetRotation = (float)atan2((double)(walkDirection.m128_f32[0]), (double)(walkDirection.m128_f32[2])) + XM_PI;
		float rotationDifference = targetRotation - currentRotationY;

		if (rotationDifference < XM_PI )
		{
			rotationDifference -= (float)XM_PI * 2;	
		}

		if (rotationDifference > -XM_PI )
		{
			rotationDifference += (float)XM_PI * 2;			
		}

		currentRotationY += (rotationDifference)*dt * 5;
		
		this->getMoveCompPtr()->rotation = XMVectorSet(0.0f, currentRotationY, 0.0f, 0.0f);
	}

	else
	{
		if (XMVectorGetX(walkBackDirection) <= 0.f && XMVectorGetY(walkBackDirection) <= 0.f && XMVectorGetZ(walkBackDirection) <= 0.f)
		{
			this->getMoveCompPtr()->rotation = XMVectorSet(0.0f, XMConvertToRadians(180.0f), 0.0f, 0.0f);
		}

		else 
		{
			this->getMoveCompPtr()->position += (walkBackDirection * dt * 13);

			float targetRotation = (float)atan2((double)(walkBackDirection.m128_f32[0]), (double)(walkBackDirection.m128_f32[2])) + XM_PI;

			float rotationDifference = targetRotation - currentRotationY;

			if (rotationDifference < XM_PI)
			{
				rotationDifference -= (float)XM_PI * 2;
			}

			if (rotationDifference > -XM_PI)
			{
				rotationDifference += (float)XM_PI * 2;
			}

			currentRotationY += (rotationDifference)*dt * 5;

			this->getMoveCompPtr()->rotation = XMVectorSet(0.0f, currentRotationY, 0.0f, 0.0f);
		}
	}
}
	

	

	


