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
	void setActiveStatus(const bool activeStatus);
	void initialize(const int modelIndex, const int wvpCBufferIndex, Model* mdl, const XMVECTOR teleportLocation, Player* player, const int roomID, const bool oneTimeUse = true);
	void update();
	void resetActiveRoomVariable();
	bool shouldChangeActiveRoom() const;
	int getRoomID()const ;

};