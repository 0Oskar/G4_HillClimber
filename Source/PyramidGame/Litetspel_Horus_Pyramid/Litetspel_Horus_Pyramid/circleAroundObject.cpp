#include "pch.h"
#include "circleAroundObject.h"

circleAroundObject::circleAroundObject()
{
}

circleAroundObject::circleAroundObject(GameObject* rotatingObject, XMVECTOR centerOfRotation, float theRadius)
{
	this->theObject = rotatingObject;
	this->center = centerOfRotation;
	this->radius = theRadius;
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


		this->flySpeed += (dt * 1000);

		if (this->flySpeed >= flyMax)
		{
			this->angle--;
			objectRotation.m128_f32[1] += XMConvertToRadians(1);

			if (this->angle <= 0.f)
			{
				this->angle = 360.f;
			}

			this->flySpeed = flySpeedStartValue;
		}

		float newPosX = (centerX + cos(XMConvertToRadians(this->angle)) * radius);
		float newPosZ = (centerZ + sin(XMConvertToRadians(this->angle)) * radius);

		objectPosition.m128_f32[0] = newPosX;
		objectPosition.m128_f32[2] = newPosZ;

		theObject->setPosition(XMVectorSet(objectPosition.m128_f32[0], centerY, objectPosition.m128_f32[2], 0.0f));
		theObject->setRotation(XMVectorSet(objectRotation.m128_f32[0], objectRotation.m128_f32[1], objectRotation.m128_f32[2], 0.0f));
	}
}

