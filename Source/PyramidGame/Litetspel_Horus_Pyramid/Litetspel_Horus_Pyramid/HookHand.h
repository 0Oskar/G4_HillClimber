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
		hit,
		flyYouFool,
	};
private:
	const static int hookDelayTime = 1;
	const static int maxDistance = 100;
	float hookSpeedForward = 100.0f;
	float hookSpeedRetract = 100.0f;

	hookState m_hookState;
	GameObject* m_hookGameObject;
	MovementComponent* m_playerMovement;
	std::vector<DirectX::BoundingBox*>* m_boundingBoxes;

	Timer m_hookTimer;
	DirectX::XMVECTOR m_origin;
	DirectX::XMVECTOR m_shootDirection;
	DirectX::XMVECTOR m_platformCenter;
	bool colide();
	bool canFire();
	bool canRecall();
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	std::shared_ptr<DirectX::SoundEffect> m_fireSound;
	std::shared_ptr<DirectX::SoundEffect> m_impactSound;
	std::shared_ptr<DirectX::SoundEffect> m_ejectSound;

public:
	HookHand();
	void init(GameObject* gObject, MovementComponent* plyMoveComp, std::vector<DirectX::BoundingBox*>* bb, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	void update(float dt);
	bool shouldFly();
	void fire();
	void retract();

	DirectX::XMVECTOR m_toHeadDir;


};