#pragma once

#include "pch.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "Player.h"

class followingEnemy : public GameObject
{

private:


	bool m_hasColided;
	bool reachedEdge;
	Player *thePlayer;

	float delayForMove = 1.0f;

	XMVECTOR scorpionSpawnPos;

	bool doOnce = false;
	bool getSpawnPosOnce = false;

	bool startCountDown = false;
	float countDown = 0.5f;
	float walkingBackCountDown = 5;
	float currentRotationY = 0.0f;

	DirectX::BoundingOrientedBox scorpionBB;


public:

	followingEnemy();

	void init(bool colidable, int modelIndex, int wvpCBufferIndex, Model* mdl, Player *player);
	void update(float dt);
	void onPlayerColide();
	void setReachedEdge(bool aValue);

	BoundingOrientedBox getBB();
	bool getReachedEdge();

	void followPlayer(float dt);
	

};