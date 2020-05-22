#pragma once
#include"Room.h"
#include <string>


class finalRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	std::vector<GameObject*> swingingAxes;

	float rotationSwap = 0.0f;

	int doOnceAtStart = 0;

	float swingingSpeed = 80.0f;

	std::vector<DirectX::BoundingOrientedBox> axeBB;
	DirectX::BoundingOrientedBox axeTestBB;
	Lever* lever;

	int timesHit = 0;
	std::string timesHitString;

	bool respawn = false;

	XMFLOAT3 axePos;

	XMVECTOR aRotation;
	XMFLOAT4 rot;

	bool flipDirection = false;

	bool wonGame = false;

	Lever* m_objectTest;
public:
	finalRoom();
	~finalRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};