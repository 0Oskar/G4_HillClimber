#include "pch.h"
#include "followingEnemy.h"

followingEnemy::followingEnemy()
{
	m_hasColided = false;
	reachedEdge = true;
	scorpionSpawnPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

void followingEnemy::init(bool colidable, int wvpCBufferIndex, Model* mdl, Player *player)
{
	initializeDynamic(colidable, false, wvpCBufferIndex, 16, DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), mdl);
	thePlayer = player;


	XMFLOAT3 scorpionPos;
	XMStoreFloat3(&scorpionPos, getMoveCompPtr()->position);

	XMFLOAT4 scorpionRot;
	XMStoreFloat4(&scorpionRot, getMoveCompPtr()->rotation);

	scorpionBB = BoundingOrientedBox(scorpionPos, XMFLOAT3(1.2f, 2.f, 1.2f), scorpionRot);	
}

void followingEnemy::update(float dt)
{	
	followPlayer(dt);
	scorpionBB.Center = XMFLOAT3(getMoveCompPtr()->position.m128_f32[0], getMoveCompPtr()->position.m128_f32[1], getMoveCompPtr()->position.m128_f32[2]);
	XMVECTOR aRotation = XMQuaternionRotationRollPitchYawFromVector(getMoveCompPtr()->rotation);
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, aRotation);
	scorpionBB.Orientation = rot;
}
	

void followingEnemy::onPlayerColide()
{

}

void followingEnemy::setReachedEdge(bool aValue)
{
	reachedEdge = aValue;
}

BoundingOrientedBox* followingEnemy::getBB()
{
	return &scorpionBB;
}

bool followingEnemy::getReachedEdge()
{
	return reachedEdge;
}

void followingEnemy::followPlayer(float dt)
{
	XMVECTOR walkDirection;
	XMVECTOR playerPosition;
	XMVECTOR previousPosition;
	XMVECTOR scorpionPos = getMoveCompPtr()->position;

	if (getSpawnPosOnce == false)
	{
		scorpionSpawnPos = getMoveCompPtr()->position;
		getSpawnPosOnce = true;
	}
	

	XMVECTOR upDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR playerRotation = thePlayer->getMoveCompPtr()->rotation;
	float playerRotationY = XMVectorGetY(playerRotation);

	playerPosition = thePlayer->getMoveCompPtr()->position;
	playerPosition = XMVectorSetY(playerPosition, 5.f);
	previousPosition = playerPosition;


	XMVECTOR walkBackDirection;
	walkBackDirection = scorpionSpawnPos - scorpionPos;
	walkBackDirection = XMVector3Normalize(walkBackDirection);

	walkDirection = playerPosition -scorpionPos;
	walkDirection = XMVector3Normalize(walkDirection);

	if ( reachedEdge == false)
	{
		getMoveCompPtr()->position += (walkDirection * dt * 9);


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
		
		getMoveCompPtr()->rotation = XMVectorSet(0.0f, currentRotationY, 0.0f, 0.0f);
	}

	else
	{
		if (XMVectorGetX(walkBackDirection) <= 0.f && XMVectorGetY(walkBackDirection) <= 0.f && XMVectorGetZ(walkBackDirection) <= 0.f)
		{
			getMoveCompPtr()->rotation = XMVectorSet(0.0f, XMConvertToRadians(180.0f), 0.0f, 0.0f);
		}

		else 
		{
			getMoveCompPtr()->position += (walkBackDirection * dt * 9);

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

			getMoveCompPtr()->rotation = XMVectorSet(0.0f, currentRotationY, 0.0f, 0.0f);
		}
	}
}
	

	

	


