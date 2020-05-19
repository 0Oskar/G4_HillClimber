#pragma once

#include "GameObject.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "Input.h"
#include "HookHand.h"
#include "StatusTextHandler.h"

class Player : public GameObject
{
private:
	std::vector<DirectX::BoundingBox*> m_collidableAABBoxes;
	std::vector<DirectX::BoundingOrientedBox*> m_collidableOrientedBoxes;
	DirectX::BoundingOrientedBox m_pyramidOBB;
	HookHand m_hookHand;
	XMVECTOR m_spawnPosition;

	bool inUse;

	float m_lastOnGroundYPos;
	float m_failThreshold;
	bool m_lastFly;

	bool m_QAmode;

public:
	Player();
	~Player();

	// Initialization
	void initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, GameObject* gObj, GameObject* hookGun, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine, std::vector<DirectX::BoundingBox*> platformBB);

	// Collidable Bounding Boxes
	void addAABB(DirectX::BoundingBox* aabb);
	void addAABBFromVector(std::vector<DirectX::BoundingBox>* aabb);
	void clearAABB();
	void addOrientedBBFromVector(std::vector<DirectX::BoundingOrientedBox>* obb);
	void addPyramidOBB(DirectX::BoundingOrientedBox* obb);
	bool getinUse();

	void resetVelocity();
	void flyDown(float speed);

	// Setters
	void setSpawnPosition(XMVECTOR position);

	// Update
	void respawn();
	void updateHookHandBB(std::vector<DirectX::BoundingBox*> platformBB);
	void update(Keyboard* keyboard, Mouse* mouse, float dt);
};