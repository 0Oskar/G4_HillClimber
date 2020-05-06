#pragma once
#include"Room.h"

class TemplateRoom : public Room
{
private:
	void createBoundingBoxes();
	void createSceneObjects();
	void onCompleted();

public:
	TemplateRoom();
	~TemplateRoom();
	void onEntrance();
	void update(float dt, Camera* camera);
	void init();
};