#pragma once
#include"Room.h"

class TemplateRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	Lever* m_objectTest;
public:
	TemplateRoom();
	~TemplateRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};