#pragma once
#include"Room.h"
#include"CheckpointHandler.h"
#include"circleAroundObject.h"

class PyramidRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();
	void addPalmTrees();
	void addCactais();


	std::vector<GameObject*> rotateObjects;

	std::vector<bool> doOnce;

	bool assignOnce = false;

	circleAroundObject rotating[6];

	//Rotating objects around specific point variables
	float angle = 360.0f;

	//Change both of these if you're changing speed (Will change speed of all rotating objects)
	float flySpeed = 3.0f;
	float flySpeedStartValue = 3.0f;

	std::vector<XMVECTOR> palmTreePos;
	////////////////////////////////

	GameObject* clouds;
	DirectX::BoundingOrientedBox m_pyramidOOB;
	CheckpointHandler m_checkpointHandler;
	const XMVECTOR platformPush = XMVectorSet(0, 0, -10, 0 );
	std::vector<DirectX::BoundingBox*> platformBB;
	int completedRooms;
public:
	PyramidRoom();
	~PyramidRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init(DirectX::BoundingOrientedBox* pyramidBB);
	void portals();
	std::vector<DirectX::BoundingBox*> getBBForHook();
};