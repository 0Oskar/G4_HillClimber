#include "pch.h"
#include "followingEnemy.h"

followingEnemy::followingEnemy()
{
	this->m_hasColided = false;
	this->reachedEdge = false;
}

void followingEnemy::init(bool colidable, int modelIndex, int wvpCBufferIndex, Model* mdl, Player *player)
{
	this->initializeDynamic(colidable, false, modelIndex, wvpCBufferIndex, 16, DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), mdl);
	this->thePlayer = player;
	
}

void followingEnemy::update(float dt)
{
	if (this->reachedEdge == true)
	{
		this->delayForMove -= dt;

		//if (this->delayForMove <= 0)
	//	{
			//this->reachedEdge = false;
		//	this->delayForMove = 1.0f;
	//  }
	}

	else
	{
		followPlayer(dt);
	}
}
	

void followingEnemy::onPlayerColide()
{

}

void followingEnemy::setReachedEdge(bool aValue)
{
	this->reachedEdge = aValue;
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

	XMVECTOR upDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMVECTOR playerRotation = this->thePlayer->getMoveCompPtr()->rotation;
	float playerRotationY = XMVectorGetY(playerRotation);

	playerPosition = this->thePlayer->getMoveCompPtr()->position;
	playerPosition = XMVectorSetY(playerPosition, 2.9f);
	previousPosition = playerPosition;

	walkDirection = playerPosition -scorpionPos;
	walkDirection = XMVector3Normalize(walkDirection);

	if (this-> reachedEdge == false)
	{
		this->getMoveCompPtr()->position += (walkDirection * dt * 13);
		//walkDirection = XMVectorSetX(walkDirection, 0.0f);
	}
	

	XMVECTOR newPlayerPosition = this->thePlayer->getMoveCompPtr()->position;
	XMVECTOR scorpionPosition = this->getMoveCompPtr()->position;


	float targetRotation = (float)atan2((double)(walkDirection.m128_f32[0]), (double)(walkDirection.m128_f32[2])) + XM_PI;

	float rotationDifference = targetRotation - currentRotationY;

	if (rotationDifference > XM_PI)
	{
		rotationDifference -= (float)XM_PI * 2;
	}

	if (rotationDifference < -XM_PI)
	{
		rotationDifference += (float)XM_PI * 2;
	}
	
	currentRotationY += (rotationDifference) * dt * 5;

	this->getMoveCompPtr()->rotation = XMVectorSet(0.0f, currentRotationY, 0.0f, 0.0f);


	}
	

	

	


