#pragma once
#include"Room.h"

// Viktor's Room

class FindGemsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();


	//Lever
	std::vector<DirectX::BoundingBox*> leverBB;
	Lever* lever;

	// Gems
	std::vector<Lever*> gems;

public:
	FindGemsRoom();
	~FindGemsRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};