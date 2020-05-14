#pragma once
#include"Room.h"
#include "time.h"
#include "Timer.h"

class EdvinsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onWin();
	void onFail();
	bool CorrectOrder(int arr1[], int arr2[]);
	//void spinButton(GameObject* button);

	std::vector<Lever*> wonPuzzleObject;

	//Cover
	GameObject* cover;
	Timer coverTimer;
	bool canMoveCover = false;
	bool moveCoverUp = false;
	bool moveCoverDown = false;
	DirectX::XMVECTOR coverPos = DirectX::XMVectorSet(-6, 9.5, 0, 1);

	//Buttons
	std::vector<Lever*> buttons;
	Timer timer;
	bool moveButtons = false;
	int guessPos;
	int guessOrder[5];
	int spinButtonIndex = -1;
	float buttonsPosY = -6;
	DirectX::XMVECTOR buttonRot = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(0), 0, 1);

	//Bricks
	std::vector<GameObject*> bricks;
	int hyroglajfArr[5];

	//Lever
	std::vector<DirectX::BoundingBox*> leverBB;
	Lever* lever;
	GameObject* leverHandle;
	Timer leverTimer;
	bool canPullLever = true;
	bool moveLever = false;
	bool tempLever = false;
	
	//Other
	int nrOfFails;
	bool wonThePuzzle = false;
	bool lifeTaken = false;
	Portal* roomPortal;
public:
	EdvinsRoom();
	~EdvinsRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};