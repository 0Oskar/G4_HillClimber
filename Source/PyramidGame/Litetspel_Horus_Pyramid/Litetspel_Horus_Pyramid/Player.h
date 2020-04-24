#pragma once

#include "GameObject.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "Input.h"
#include"HookHand.h"

class Player : public GameObject
{
private:
	std::vector<DirectX::BoundingBox*> m_collidableAABBoxes;
	HookHand m_hookHand;
	bool lastFly;

public:
	Player();
	~Player();

	// Initialization
	void initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, GameObject* gObj, GameObject* hookGun, std::shared_ptr<DirectX::AudioEngine> audioEngine);

	// Collidable Bounding Boxes
	void addAABB(DirectX::BoundingBox* aabb);

	// Update
	void update(Keyboard* keyboard, Mouse* mouse, float dt);
};