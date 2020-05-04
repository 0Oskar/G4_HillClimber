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
	const DirectX::XMVECTOR gunPosOffset = DirectX::XMVectorSet(1.5f, -0.7f, 2.3f, 0.f);
	const DirectX::XMVECTOR hookPosOffsetConst = DirectX::XMVectorSet(0.f, 0.5f, 1.4f, 0.0f);
	const DirectX::XMVECTOR hookRotOffsetConst = DirectX::XMVectorSet(0.3f, 160.0f, 0.f, 0.f);
	const DirectX::XMVECTOR gunRotOffset = DirectX::XMVectorSet(0.f, 160.f, 0.f, 0.f);

	DirectX::XMVECTOR hookPosOffset = DirectX::XMVectorSet(0.f, 0.f, 5.f, 0.f);


	float hookSpeedForward = 100.0f;
	float hookSpeedRetract = 100.0f;

	hookState m_hookState;
	GameObject* m_hookGameObject;
	GameObject* m_gunGameObject;
	MovementComponent* m_playerMovement;
	std::vector<DirectX::BoundingBox*>* m_boundingBoxes;
	std::vector<DirectX::BoundingBox*>* m_platformsBB;


	Timer m_hookTimer;
	DirectX::XMVECTOR m_origin;
	DirectX::XMVECTOR m_shootDirection;
	DirectX::XMVECTOR m_platformCenter;
	bool colide();
	bool canFire();
	bool canRecall();
	void updateHandModel();
	DirectX::XMVECTOR invertX(DirectX::XMVECTOR VectorToInvertX);
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	std::shared_ptr<DirectX::SoundEffect> m_fireSound;
	std::shared_ptr<DirectX::SoundEffect> m_impactSound;
	std::shared_ptr<DirectX::SoundEffect> m_ejectSound;
	std::unique_ptr<DirectX::SoundEffectInstance> m_effect;

public:
	HookHand();
	void init(GameObject* gObject, MovementComponent* plyMoveComp, std::vector<DirectX::BoundingBox*>* bb, GameObject* hookGun, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB);
	void update(float dt);
	bool shouldFly();
	void fire();
	void retract();

	DirectX::XMVECTOR m_toHeadDir;


};