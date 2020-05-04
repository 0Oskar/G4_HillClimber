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
	DirectX::BoundingOrientedBox m_pyramidOBB;
	HookHand m_hookHand;
	bool lastFly;

	std::string myString;

public:
	Player();
	~Player();

	// Initialization
	void initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, GameObject* gObj, GameObject* hookGun, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine);

	// Collidable Bounding Boxes
	void addAABB(DirectX::BoundingBox* aabb);
	void addPyramidOBB(DirectX::BoundingOrientedBox* obb);

	// Update
	void update(Keyboard* keyboard, Mouse* mouse, float dt);
};