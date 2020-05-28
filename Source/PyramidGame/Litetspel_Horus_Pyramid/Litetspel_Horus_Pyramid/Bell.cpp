#include "pch.h"
#include "Bell.h"

Bell::Bell()
{

}

Bell::~Bell()
{

}

void Bell::initialize(int mdlIndex, int wvpCBufferIndex, Model* mdl)
{
	this->initializeStatic(false, mdlIndex, wvpCBufferIndex, mdl);
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
	return this->isAnimating;
}

float Bell::getBellOffset()
{
	return bellOffset;
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
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, bellOffset, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE FORWARD");
}

void Bell::moveBellForward2()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, bellOffset * 2, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE FORWARD");
}

void Bell::moveBellBackward()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, -bellOffset, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Backward");
}

void Bell::moveBellBackward2()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, 0, -bellOffset * 2, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Backward");
}

void Bell::moveBellLeft()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(-bellOffset, 0, 0, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Left");

}

void Bell::moveBellLeft2()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(-bellOffset * 2, 0, 0, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Left");

}

void Bell::moveBellRight()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(bellOffset, 0, 0, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Right");

}

void Bell::moveBellRight2()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(bellOffset * 2, 0, 0, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Right");

}

void Bell::raiseBell()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, -bellOffset, 0, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Up");

}

void Bell::lowerBell()
{
	this->isAnimating = true;
	startPos = this->getPosition();
	animationTimer.restart();
	XMVECTOR movePos = XMVectorSet(0, bellOffset, 0, 0);
	endPos = startPos + movePos;
	OutputDebugString(L"MOVE Down");
}

void Bell::update(float dt)
{
	if (!XMVector3Equal(startPos, endPos))
	{
		XMVECTOR positionDelta = this->startPos - this->endPos;
		XMVECTOR currentPos = this->getPosition();
		this->setPosition(startPos + (positionDelta * animationTimer.timeElapsed()));
		//if (XMVector3Equal(XMVector3Cross(this->startPos - this->endPos, startPos - this->getPosition()), XMVectorZero()))
		if (animationTimer.timeElapsed() > 1)
		{
			this->isAnimating = false;
			startPos = endPos;
			OutputDebugString(L"Animation Ended");
		}
	}
}
