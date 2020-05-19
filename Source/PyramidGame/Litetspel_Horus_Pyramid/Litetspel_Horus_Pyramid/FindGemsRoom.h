#pragma once
#include"Room.h"

// Viktor's Room

class FindGemsRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();


	//Lever
	std::vector<DirectX::BoundingBox*> m_LeverBB;
	Lever* m_Lever;

	// Gems
	std::vector<Lever*> m_Gems;
	unsigned int m_SpawnIndex;
	bool m_HasPedistal;
	bool m_CanBePickedUp;
	std::vector<XMVECTOR> m_PreFixedSpawnpoints;
	

	

public:
	FindGemsRoom();
	~FindGemsRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init();
	void portals();
};