#pragma once
#include "GameObject.h"

class Bell : public GameObject
{
private:

	const float bellOffset = 30;

	XMVECTOR startPos;
	XMVECTOR endPos;

	Timer animationTimer;
	//Timer moveTimer;
	bool isAnimating = false;

public:

	Bell();
	~Bell();

	void initialize(int mdlIndex, int wvpCBufferIndex, Model* mdl);

	XMVECTOR getStartPos();
	XMVECTOR getEndPos();

	bool GetAnimating();

	float getBellOffset();

	void setStartPos(XMVECTOR pos);
	void setEndPos(XMVECTOR pos);

	void moveBellForward();
	void moveBellForward2();

	void moveBellBackward();
	void moveBellBackward2();

	void moveBellLeft();
	void moveBellLeft2();

	void moveBellRight();
	void moveBellRight2();

	void raiseBell();
	void lowerBell();

	void update(float dt);

};