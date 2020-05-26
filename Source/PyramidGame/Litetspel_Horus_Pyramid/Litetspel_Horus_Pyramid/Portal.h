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
	bool m_oneTimeUse;
	bool m_active;
public:

	Portal();
	~Portal();
	void setActiveStatus(bool activeStatus);
	void initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation, Player* player, int roomID, bool oneTimeUse = true);
	void update();
	void resetActiveRoomVariable();
	bool shouldChangeActiveRoom();
	int getRoomID();

};