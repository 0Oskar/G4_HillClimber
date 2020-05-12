#pragma once
#include"GameObject.h"
#include "Player.h"

class Portal : public GameObject
{
private:

	XMVECTOR m_teleportLocation;
	Player* player;
	int m_roomID;
	bool m_changeActiveRoom;
public:

	Portal();
	~Portal();
	void initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation, Player* player, int roomID);
	void update();
	void resetActiveRoomVariable();
	bool shouldChangeActiveRoom();
	int getRoomID();

};