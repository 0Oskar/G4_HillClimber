#pragma once
#include"Room.h"
#include "followingEnemy.h"

class KevinsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	std::vector<DirectX::BoundingBox> pussels;
	std::vector<DirectX::BoundingBox*> trapBB;
	std::vector<DirectX::BoundingBox*> leverBB;
	std::vector<DirectX::BoundingBox*> bridgeBB;
    
	followingEnemy *scorpion;
	followingEnemy *scorpion2;

	////////////////
	std::vector<GameObject*> expandingBridge;
	std::vector<GameObject*> scorpionObject;
	std::vector<GameObject*> dartTrap;
	std::vector<GameObject*> dartTrapWall;
	std::vector<Lever*> lever;
	std::vector<Lever*> wonPuzzleObject;
	

	//Trigger things
	std::vector<DirectX::BoundingBox> triggerBB;
	std::vector<DirectX::BoundingBox> deathTrapBB;


	DirectX::BoundingOrientedBox* scorpionBB;
	
	

	bool dartFly1 = false;
	bool dartFly2 = false;
	float dartPosition1 = 40.f;
	float dartPosition2 = 40.f;

	int lives = 3;

	float bridgeStop = 10;

	bool trapActive1 = true;
	bool trapActive2 = true;
	bool wonThePuzzle = false;
	bool lostThePuzzle = false;
	bool lifeTaken = false;

	XMVECTOR scorpionBBPosition;
	Portal* portalPtr;

	

public:
	KevinsRoom();
	~KevinsRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void looseLife();
	void checkNrOfLives();
	void onEntrance();
	void init();
	void portals();
};