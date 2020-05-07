#pragma once
#include"Room.h"

class PyramidRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	DirectX::BoundingOrientedBox m_pyramidOOB;
public:
	PyramidRoom();
	~PyramidRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init(DirectX::BoundingOrientedBox* pyramidBB);
	void portals();
};