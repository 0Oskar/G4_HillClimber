#pragma once
#include"Room.h"
#include"CheckpointHandler.h"

class PyramidRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

	DirectX::BoundingOrientedBox m_pyramidOOB;
	CheckpointHandler m_checkpointHandler;
	const XMVECTOR platformPush = XMVectorSet(0, 0, -10, 0 );
	std::vector<DirectX::BoundingBox*> platformBB;
	int completedRooms;
public:
	PyramidRoom();
	~PyramidRoom();
	void update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged);
	void onEntrance();
	void init(DirectX::BoundingOrientedBox* pyramidBB);
	void portals();
	std::vector<DirectX::BoundingBox*> getBBForHook();
};