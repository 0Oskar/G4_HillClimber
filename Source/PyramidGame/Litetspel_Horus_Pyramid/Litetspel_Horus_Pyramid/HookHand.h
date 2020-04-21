#pragma once
#include"Model.h"
#include"GameObject.h"
#include"Timer.h"

class HookHand
{
	enum class hookState
	{
		idle,
		shooting,
		recalling,
		waiting,
	};
private:
	const static int hookDelayTime = 1;
	float hookSpeedForward = 100.0f;
	float hookSpeedRetract = 100.0f;

	hookState m_hookState;
	GameObject* m_hookGameObject;
	MovementComponent* m_playerMovement;

	Timer m_hookTimer;
	DirectX::XMVECTOR m_origin;
	DirectX::XMVECTOR m_shootDirection;
public:
	HookHand();
	void init(GameObject* gObject, MovementComponent* plyMoveComp);
	bool canFire();
	bool canRecall();
	void fire();
	void retract();
	void update(float dt);
};