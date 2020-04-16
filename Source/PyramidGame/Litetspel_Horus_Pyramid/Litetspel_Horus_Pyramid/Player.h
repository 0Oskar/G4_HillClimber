#pragma once

#include "GameObject.h"
#include "MovementComponent.h"
#include "PhysicsComponent.h"
#include "Input.h"

class Player : public GameObject
{
private:
	std::vector<DirectX::BoundingBox*> m_collidableAABBoxes;

public:
	Player();
	~Player();

	// Initialization
	void initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration);

	// Collidable Bounding Boxes
	void addAABB(DirectX::BoundingBox* aabb);

	// Update
	void update(Keyboard* keyboard, MouseEvent mouseEvent, float dt);
};