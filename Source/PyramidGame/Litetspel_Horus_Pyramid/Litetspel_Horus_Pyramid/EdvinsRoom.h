#pragma once
#include"Room.h"

class EdvinsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	std::vector<DirectX::BoundingBox*> leverBB;


	std::vector<GameObject*> dartTrap;
	std::vector<Lever*> lever;
	std::vector<Lever*> wonPuzzleObject;


	bool wonThePuzzle = false;
	bool lifeTaken = false;
	int nrOfFails;

public:
	EdvinsRoom();
	~EdvinsRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};