#pragma once
#include "Room.h"

class TristansRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();
	Portal* portal;
	std::vector<DirectX::BoundingBox> SpikesBB;

	std::vector<DirectX::BoundingBox*> LeverBB;
	std::vector<Lever*> leverGrip;
	Timer leverTimer[4];
	bool canPullLever0 = true;
	bool canPullLever1 = true;
	bool canPullLever2 = true;
	bool canPullLever3 = true;

	bool moveLever0 = false;
	bool moveLever1 = false;
	bool moveLever2 = false;
	bool moveLever3 = false;

	bool tempLever0 = false;
	bool tempLever1 = false;
	bool tempLever2 = false;
	bool tempLever3 = false;

public:

	TristansRoom();
	~TristansRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();

};