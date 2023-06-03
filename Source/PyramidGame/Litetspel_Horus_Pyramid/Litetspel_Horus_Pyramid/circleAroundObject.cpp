#include "pch.h"
#include "circleAroundObject.h"

circleAroundObject::circleAroundObject()
{
}

circleAroundObject::circleAroundObject(GameObject* rotatingObject, XMVECTOR centerOfRotation, float theRadius)
{
	theObject = rotatingObject;
	center = centerOfRotation;
	radius = theRadius;
}

void circleAroundObject::rotateAround(std::string whichAxis, float dt)
{
	if (whichAxis == "Y")
	{

		XMVECTOR objectPosition = theObject->getPosition();
		XMVECTOR objectRotation = theObject->getRotation();

		float centerX = center.m128_f32[0];
		float centerY = center.m128_f32[1];
		float centerZ = center.m128_f32[2];

		float speed = radius / 7.f;
		float flyMax = 30 * speed;


		flySpeed += (dt * 1000);

		if (flySpeed >= flyMax)
		{
			angle--;
			objectRotation.m128_f32[1] += XMConvertToRadians(1);

			if (angle <= 0.f)
			{
				angle = 360.f;
			}

			flySpeed = flySpeedStartValue;
		}

		float newPosX = (centerX + cos(XMConvertToRadians(angle)) * radius);
		float newPosZ = (centerZ + sin(XMConvertToRadians(angle)) * radius);

		objectPosition.m128_f32[0] = newPosX;
		objectPosition.m128_f32[2] = newPosZ;

		theObject->setPosition(XMVectorSet(objectPosition.m128_f32[0], centerY, objectPosition.m128_f32[2], 0.0f));
		theObject->setRotation(XMVectorSet(objectRotation.m128_f32[0], objectRotation.m128_f32[1], objectRotation.m128_f32[2], 0.0f));
	}
}

