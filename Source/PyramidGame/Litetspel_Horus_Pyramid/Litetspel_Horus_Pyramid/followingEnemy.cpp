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
	walkDirection = XMVector3NormalizeEst(walkDirection);

	this->getMoveCompPtr()->position += (walkDirection * dt * 5);
	walkDirection = XMVectorSetX(walkDirection, 0.0f);

	//XMMATRIX lookAt = XMMatrixLookAtLH(this->getMoveCompPtr()->position, this->thePlayer->getMoveCompPtr()->position, upDir);

	//XMVECTOR scorpionRotation = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f), lookAt);

	//this->setRotation(scorpionRotation);

	///////////////

	//XMVECTOR toCam = this->thePlayer->getMoveCompPtr()->position - scorpionPos;
	//XMVECTOR fwdVector = XMVector3NormalizeEst(toCam);
	//
	//
	//XMVECTOR upVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMVECTOR sideVector = XMVector3Cross(upVector, fwdVector);
	//upVector = XMVector3Cross(sideVector, fwdVector);

	//XMVECTOR normalizedUp = XMVector3NormalizeEst(upVector);

	//XMVECTOR normalizedSide = XMVector3NormalizeEst(sideVector);
	//
	//XMMATRIX orientation(XMVectorGetX(sideVector), XMVectorGetY(sideVector), XMVectorGetZ(sideVector), 0.0f,
	//	XMVectorGetX(upVector), XMVectorGetY(upVector), XMVectorGetZ(upVector), 0.0f,
	//	XMVectorGetX(fwdVector), XMVectorGetY(fwdVector), XMVectorGetZ(fwdVector), 0.0f,
	//	XMVectorGetX(scorpionPos), XMVectorGetY(scorpionPos), XMVectorGetZ(scorpionPos), 0.0f);
	//
	//XMVECTOR scorpionRotation = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f), orientation);

	//this->setRotation(scorpionRotation);

	///////////////

	XMVECTOR newPlayerPosition = this->thePlayer->getMoveCompPtr()->position;
	
	float playerPositionX = XMVectorGetX(playerPosition);
	float playerPositionY = XMVectorGetY(playerPosition);
	float playerPositionZ = XMVectorGetZ(playerPosition);

	XMVECTOR scorpionPosition = this->getMoveCompPtr()->position;

	float scorpionPositionX = XMVectorGetX(scorpionPosition);
	float scorpionPositionY = XMVectorGetY(scorpionPosition);
	float scorpionPositionZ = XMVectorGetX(scorpionPosition);

	XMVECTOR scorpionRotation = this->getMoveCompPtr()->rotation;

	float scorpionRotationY = XMVectorGetY(scorpionRotation);
	float scorpionRotationX = XMVectorGetX(scorpionRotation);

	////////
	//XMVECTOR vectorBetweenThem = playerPosition - scorpionPosition;


	//if (XMVectorGetX(vectorBetweenThem) * scorpionRotationY > (XMVectorGetY(vectorBetweenThem) * scorpionRotationX))
	//{
	//	scorpionRotationY += XMConvertToRadians(100 * dt);
	//	scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);

	//	this->setRotation(scorpionRotation);
	//}

	//else 
	//{
	//	scorpionRotationY -= XMConvertToRadians(100 * dt);
	//	scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);;

	//	this->setRotation(scorpionRotation);
	//}
	////////////
	
	////////////////
	//float targetRotation = (float)atan2((double)(playerPositionY - scorpionPositionY ), (double)(playerPositionX - scorpionPositionX));

	//float rotationDifference = scorpionRotationY -targetRotation;

	//

	//if (rotationDifference > XM_PI)
	//{
	//	rotationDifference -= (float)XM_PI * 2;
	//}

	//if (rotationDifference < -XM_PI)
	//{
	//	
	//	rotationDifference += (float)XM_PI * 2;	
	//}

	//int rotationsDegree = (int)(XMConvertToDegrees(rotationDifference));
	//
	//if (rotationsDegree < 0)
	//{
	//	scorpionRotationY += XMConvertToRadians(100 * dt);
	//	scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);

	//	
	//	this->setRotation(scorpionRotation);
	//}

	//else if (rotationsDegree > 0)
	//{
	//	scorpionRotationY -= XMConvertToRadians(100 * dt);
	//	scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);;

	//	
	//	this->setRotation(scorpionRotation);
	//}

	//Bad billboarding try
	////////////////////////////////////////////////////////////////

	//XMVECTOR right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	//XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	//XMVECTOR lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	//XMVECTOR enemyToPlayer = newPlayerPosition - scorpionPosition;

	//XMVECTOR enemyToPlayerProj = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//enemyToPlayerProj = XMVectorSetY(enemyToPlayerProj, 0);

	////Normalize
	//enemyToPlayerProj = XMVector3NormalizeEst(enemyToPlayerProj);
	//enemyToPlayerProj = XMVector4Cross(up, lookAt, enemyToPlayerProj);

	//
	//this->setRotation(enemyToPlayerProj);

	////////////////////////////////////////////////////////////////

	//Närmst till lösning

	float targetRotation = (float)atan2((double)(scorpionPositionX - playerPositionX), (double)(scorpionPositionZ - playerPositionZ));


	targetRotation *= (180 / XM_PI);


	float finalRotation = XMConvertToRadians(targetRotation);
    
	this->setRotation(XMVectorSet(0.0f, finalRotation, 0.0f, 0.0f));

	//float rotationDifference = scorpionRotationY - targetRotation;



	//if (rotationDifference > XM_PI)
	//{
	//	rotationDifference -= (float)XM_PI * 2;
	//}

	//if (rotationDifference < -XM_PI)
	//{

	//	rotationDifference += (float)XM_PI * 2;
	//}

	//int rotationsDegree = (int)(XMConvertToDegrees(rotationDifference));

	//if (rotationsDegree < 0)
	//{
	//	scorpionRotationY += XMConvertToRadians(100 * dt);
	//	scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);


	//	this->setRotation(scorpionRotation);
	//}

	//else if (rotationsDegree > 0)
	//{
	//	scorpionRotationY -= XMConvertToRadians(100 * dt);
	//	scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);;


	//	this->setRotation(scorpionRotation);
	//}

	///////////////////////////////////////////////////////////////////////

	/*float rotationDifference = scorpionRotationY -targetRotation;

	

	if (rotationDifference > XM_PI)
	{
		rotationDifference -= (float)XM_PI * 2;
	}

	if (rotationDifference < -XM_PI)
	{
		
		rotationDifference += (float)XM_PI * 2;	
	}

	int rotationsDegree = (int)(XMConvertToDegrees(rotationDifference));
	
	if (rotationsDegree < 0)
	{
		scorpionRotationY += XMConvertToRadians(100 * dt);
		scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);

		
		this->setRotation(scorpionRotation);
	}

	else if (rotationsDegree > 0)
	{
		scorpionRotationY -= XMConvertToRadians(100 * dt);
		scorpionRotation = XMVectorSet(0.0f, scorpionRotationY, 0.0f, 0.0f);;

		
		this->setRotation(scorpionRotation);
	}*/
	
	/////////////

	
		  
	//playerRotation = this->thePlayer->getMoveCompPtr()->rotation;
	//SÅ SKORPIONEN TITTAR PÅ SPELAREN????
	//playerRotation = XMMatrixLookAtLH(this->getMoveCompPtr()->position, this->thePlayer->getMoveCompPtr()->position, upDir);

	

 
	//playerRotation = XMVectorSet(0.0f, XMVectorGetY(walkDirection), 0.0f, 0.0f);
	//this->setRotation(playerRotation);

	
	
	

	
}

