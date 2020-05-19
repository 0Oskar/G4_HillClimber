#pragma once
#include "Room.h"

class TristansRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	std::vector<DirectX::BoundingBox> SpikesBB;


public:

	TristansRoom();
	~TristansRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();

};