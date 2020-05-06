#pragma once
#include"GameObject.h"
#include "Player.h"
#include "Room.h"

class Portal : public GameObject
{
private:

	XMVECTOR m_teleportLocation;
	Player* player;
	Room* m_roomPtr;
	bool m_changeActiveRoom;
public:

	Portal();
	~Portal();
	void initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation, Player* player, Room* room = nullptr);
	void update();
	void resetActiveRoomVariable();
	bool shouldChangeActiveRoom();
	Room* getRoomPtr();

};