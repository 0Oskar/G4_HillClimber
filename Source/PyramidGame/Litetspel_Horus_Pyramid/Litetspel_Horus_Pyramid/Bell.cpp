#include "pch.h"
#include "Bell.h"

Bell::Bell()
{

}

Bell::~Bell()
{

}

void Bell::initialize(int wvpCBufferIndex, Model* mdl)
{
	initializeStatic(false, wvpCBufferIndex, mdl);
}

XMVECTOR Bell::getStartPos()
{
	return startPos;
}

XMVECTOR Bell::getEndPos()
{
	return endPos;
}

bool Bell::GetAnimating()
{
	return isAnimating;
}

void Bell::setStartPos(XMVECTOR pos)
{
	startPos = pos;
}

void Bell::setEndPos(XMVECTOR pos)
{
	endPos = pos;
}

void Bell::moveBellForward()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, -BELL_OFFSET, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE FORWARD");
	/*std::wstring test(std::to_wstring(XMVectorGetX(getPosition())) + L", " + std::to_wstring(XMVectorGetY(getPosition())) + L", " + std::to_wstring(XMVectorGetZ(getPosition())));
	OutputDebugString(test.c_str());
	OutputDebugString(L"\n");*/
}

void Bell::moveBellForward2()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, -BELL_OFFSET * 2, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE FORWARD");
}

void Bell::moveBellBackward()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, BELL_OFFSET, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Backward");
}

void Bell::moveBellBackward2()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, BELL_OFFSET * 2, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Backward");
}

void Bell::moveBellLeft()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(BELL_OFFSET, 0, 0, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Left");

}

void Bell::moveBellLeft2()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(BELL_OFFSET * 2, 0, 0, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Left");

}

void Bell::moveBellRight()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(-BELL_OFFSET, 0, 0, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Right");

}

void Bell::moveBellRight2()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(-BELL_OFFSET * 2, 0, 0, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Right");

}

void Bell::raiseBell()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, BELL_OFFSET, 0, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Up");

}

void Bell::lowerBell()
{
	isAnimating = true;
	startPos = getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, -BELL_OFFSET, 0, 0);
	endPos = startPos + movePos;
	//OutputDebugString(L"MOVE Down");
}

void Bell::update(float dt)
{
	if (!XMVector3Equal(startPos, endPos))
	{
		XMVECTOR positionDelta = endPos - startPos;
		XMVECTOR currentPos = getPosition();
		setPosition(startPos + (positionDelta * (float)animationTimer.timeElapsed()));
		//if (XMVector3Equal(XMVector3Cross(startPos - endPos, startPos - getPosition()), XMVectorZero()))
		if (animationTimer.timeElapsed() > 1)
		{
			isAnimating = false;
			startPos = endPos;
			setPosition(endPos);
			OutputDebugString(L"Animation Ended\n");
		}
	}
}
