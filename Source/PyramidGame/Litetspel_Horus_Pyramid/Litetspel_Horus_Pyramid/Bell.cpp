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
	XMVECTOR movePos = XMVectorSet(0, 0, -bellOffset, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellForward2()
{
	XMVECTOR movePos = XMVectorSet(0, 0, -bellOffset * 2, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellBackward()
{
	XMVECTOR movePos = XMVectorSet(0, 0, bellOffset, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellBackward2()
{
	XMVECTOR movePos = XMVectorSet(0, 0, bellOffset * 2, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellLeft()
{
	XMVECTOR movePos = XMVectorSet(bellOffset, 0, 0, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellLeft2()
{
	XMVECTOR movePos = XMVectorSet(bellOffset * 2, 0, 0, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellRight()
{
	XMVECTOR movePos = XMVectorSet(-bellOffset, 0, 0, 0);
	endPos = startPos + movePos;
}

void Bell::moveBellRight2()
{
	XMVECTOR movePos = XMVectorSet(-bellOffset * 2, 0, 0, 0);
	endPos = startPos + movePos;
}
