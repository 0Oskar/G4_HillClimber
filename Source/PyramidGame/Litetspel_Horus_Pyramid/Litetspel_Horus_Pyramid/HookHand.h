#pragma once
#include"Model.h"
#include"Timer.h"
#include"Chain.h"
#include"AudioComponent.h"

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
	const static int maxDistance = 40;
	const static int wingAnimationSpeed = 10;

	// Hook Offsets
	const DirectX::XMVECTOR hookPosOffsetConst = DirectX::XMVectorSet(0.f, 0.3f, 0.6f, 0.0f);
	const DirectX::XMVECTOR hookRotOffsetConst = DirectX::XMVectorSet(pMath::convertDegreesToRadians(0), 0.f, 0.f, 0.f);
	DirectX::XMVECTOR hookPosOffset = DirectX::XMVectorSet(-5.f, 0.f, 4.f, 0.f);
	
	// Hand Offsets
	const DirectX::XMVECTOR gunPosOffset = DirectX::XMVectorSet(1.0f, -0.8f, 2.0f, 0.f);
	//const DirectX::XMVECTOR hookRotOffsetConst = DirectX::XMVectorSet(0.3f, 160.0f, 0.f, 0.f);
	const DirectX::XMVECTOR gunRotOffset = DirectX::XMVectorSet(pMath::convertDegreesToRadians(5), 0.f, 0.f, 0.f);

	// Wing
	// - Animation
	bool m_wingsOpen;
	// - Offsets
	const DirectX::XMVECTOR leftWingPosOffset = XMVectorSet(-0.3f, -0.08f, 0.24f, 0.f);
	const DirectX::XMVECTOR leftWingRotOffset = XMVectorSet(0.f, XMConvertToRadians(60.f), 0.f, 0.f);
	
	const DirectX::XMVECTOR rightWingPosOffset = XMVectorSet(0.3f, -0.08f, 0.24f, 0.f);
	const DirectX::XMVECTOR rightWingRotOffset = XMVectorSet(0.f, XMConvertToRadians(-60.f), 0.f, 0.f);

	//Audio
	const std::wstring m_fireSoundString = L"Sounds/Explo1.wav";
	const std::wstring m_chainExtendingSound = L"Sounds/HookChainAudio.wav";
	bool m_shouldPlayChainSound = false;

	float hookSpeedForward = 40.0f;
	float hookSpeedRetract = 40.0f;
	AudioComponent* m_audioComponent;

	hookState m_hookState;
	GameObject* m_hookGameObject;
	GameObject* m_gunGameObject;
	GameObject* m_gemGameObject;
	GameObject* m_leftWingGameObject;
	GameObject* m_rightWingGameObject;
	Chain m_chain;
	PhysicsComponent* m_hookPhysicsComp;
	MovementComponent* m_playerMovement;
	PhysicsComponent* m_playerPhysics;
	std::vector<DirectX::BoundingBox*>* m_boundingBoxes;
	std::vector<DirectX::BoundingBox*>* m_platformsBB;

	Timer m_hookTimer;
	DirectX::XMVECTOR m_origin;
	DirectX::XMVECTOR m_shootDirection;
	DirectX::XMVECTOR m_platformCenter;
	bool colide();
	bool canFire();
	bool canRecall();
	void updateHandModel(float dt);
	DirectX::XMVECTOR invertX(DirectX::XMVECTOR VectorToInvertX);

public:
	HookHand();
	~HookHand();

	void init(GameObject* gObject, MovementComponent* plyMoveComp, PhysicsComponent* physicsComponent, std::vector<DirectX::BoundingBox*>* bb, GameObject* hookGun, GameObject* hookGem, GameObject* hookHandLeftWing, GameObject* hookHandRightWing, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB);
	void setBB(std::vector<DirectX::BoundingBox*> platformBB);
	
	void update(float dt);
	bool shouldFly();
	void fire();
	void retract();

	DirectX::XMVECTOR m_toHeadDir;
};