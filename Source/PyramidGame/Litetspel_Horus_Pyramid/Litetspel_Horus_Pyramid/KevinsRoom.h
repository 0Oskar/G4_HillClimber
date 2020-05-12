#pragma once
#include"Room.h"

class KevinsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	std::vector<DirectX::BoundingBox> pussels;
	std::vector<DirectX::BoundingBox*> trapBB;
	std::vector<DirectX::BoundingBox*> leverBB;


	std::vector<GameObject*> dartTrap;
	std::vector<Lever*> lever;
	std::vector<Lever*> wonPuzzleObject;

	//Trigger things
	std::vector<DirectX::BoundingBox> triggerBB;
	std::vector<DirectX::BoundingBox> deathTrapBB;

	bool dartFly1 = false;
	bool dartFly2 = false;
	float dartPosition1 = 40.f;
	float dartPosition2 = 40.f;

	bool trapActive1 = true;
	bool trapActive2 = true;
	bool wonThePuzzle = false;
	bool lifeTaken = false;

public:
	KevinsRoom();
	~KevinsRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};