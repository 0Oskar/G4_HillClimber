#pragma once
#include"GameObject.h"
#include "Player.h"

class Portal : public GameObject
{
private:

	XMVECTOR m_teleportLocation;
	Player* player;

public:

	Portal();
	~Portal();
	void initialize(int modelIndex, int wvpCBufferIndex, Model* mdl, XMVECTOR teleportLocation, Player* player);
	void update();

};