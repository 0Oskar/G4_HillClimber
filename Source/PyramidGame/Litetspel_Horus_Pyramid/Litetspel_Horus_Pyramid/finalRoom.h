#pragma once
#include"Room.h"

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

	bool flipDirection = false;

	Lever* m_objectTest;
public:
	finalRoom();
	~finalRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};