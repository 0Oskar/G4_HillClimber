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

	GameObject* diamond;

	Bell* bell1;
	Bell* bell2;
	Bell* bell3;

	Timer moveTimer;
	Timer RaiseBellTimer;

	int randNr;

	XMVECTOR leverStartRot;
	XMVECTOR leverEndRot;

	std::vector<DirectX::BoundingBox*> LeverBB;
	std::vector<Lever*> leverGrip;
	Timer leverTimer[4];
	

	bool moveLever0 = false;
	bool moveLever1 = false;
	bool moveLever2 = false;
	bool moveLever3 = false;

	bool moveLeverAgain0 = false;
	bool moveLeverAgain1 = false;
	bool moveLeverAgain2 = false;
	bool moveLeverAgain3 = false;

	bool bellsShallRase = false;
	bool bellsShallLower = false;
	bool isAnimationGoing = false;

	bool guessedRight;

public:

	TristansRoom();
	~TristansRoom();
	void addBellToRoom(Model* mdl, DirectX::XMVECTOR position);
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();


	void moveBellsUp();
	void moveBellsDown(float startTime);
	void Bellmove1(float startTime);
	void Bellmove2(float startTime);
	void Bellmove3(float startTime);

	void onWin();
	void onFail();
};