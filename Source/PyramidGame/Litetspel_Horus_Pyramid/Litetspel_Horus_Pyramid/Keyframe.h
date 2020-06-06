#pragma once

class Keyframe
{

public:

	Keyframe();

	struct JointTransform
	{
		float position[3];
		float rotation[3];
	};

	JointTransform* transforms;
	float timeStamp;

};

