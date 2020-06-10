#pragma once

#include "pch.h"
#include "GameObject.h"

class circleAroundObject
{

private:

	float radius;
	GameObject* theObject;
	XMVECTOR center;

	float angle = 360.0f;

	//Change both of these if you're changing speed (Will change speed of all rotating objects)
	float flySpeed = 3.0f;
	float flySpeedStartValue = 3.0f;

public:

	circleAroundObject();
	circleAroundObject(GameObject * rotatingObject, XMVECTOR centerOfRotation, float theRadius);

	void rotateAround(std::string whichAxis, float dt);

};