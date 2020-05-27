#pragma once
#include "GameObject.h"

class Bell : public GameObject
{
private:

	const float bellOffset = 30;

	XMVECTOR startPos;
	XMVECTOR endPos;



public:

	Bell();
	~Bell();

	void initialize(int mdlIndex, int wvpCBufferIndex, Model* mdl);

	XMVECTOR getStartPos();
	XMVECTOR getEndPos();

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

};