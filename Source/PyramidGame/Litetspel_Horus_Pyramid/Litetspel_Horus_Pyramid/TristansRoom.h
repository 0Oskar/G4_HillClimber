#pragma once
#include "Room.h"
#include "Bell.h"

class TristansRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();
	Portal* portal;
	std::vector<DirectX::BoundingBox> SpikesBB;

	Bell* bell1;
	Bell* bell2;
	Bell* bell3;

	const float bellOffset = 30;

	bool animationIsFinnished;

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

	bool moveLeverAgain0 = false;
	bool moveLeverAgain1 = false;
	bool moveLeverAgain2 = false;
	bool moveLeverAgain3 = false;

	bool tempLever0 = false;
	bool tempLever1 = false;
	bool tempLever2 = false;
	bool tempLever3 = false;

	//void moveBellForward(GameObject * bell, float dt);
	//void moveBellForward2(GameObject* bell, float dt);

	//void moveBellBackward(GameObject* bell, float dt);
	//void moveBellBackward2(GameObject* bell, float dt);

	//void moveBellLeft(GameObject* bell, float dt);
	//void moveBellLeft2(GameObject* bell, float dt);

	//void moveBellRight(GameObject* bell, float dt);
	//void moveBellRight2(GameObject* bell, float dt);

public:

	TristansRoom();
	~TristansRoom();
	void addBellToRoom(int mdlIndex, Model* mdl, DirectX::XMVECTOR position);
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();

};