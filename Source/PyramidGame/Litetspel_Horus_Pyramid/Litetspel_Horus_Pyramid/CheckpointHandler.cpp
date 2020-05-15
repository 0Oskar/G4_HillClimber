#include "pch.h"
#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
	this->m_current.first = -1;
	this->m_current.second = {0.f, 0.f, 0.f, 0.f};
}

CheckpointHandler::~CheckpointHandler() {}

void CheckpointHandler::setCurrent(int index, XMVECTOR position)
{
	this->m_current.first = index;
	this->m_current.second = position;
}

void CheckpointHandler::addCheckpointGameObject(int index, XMVECTOR gameObjectPosition)
{
	this->m_checkpointIndexPos.push_back(std::make_pair(index, gameObjectPosition));
}

int CheckpointHandler::size() const
{
	return (int)this->m_checkpointIndexPos.size();
}

std::pair<int, XMVECTOR> CheckpointHandler::getIndexPosAt(int index) const
{
	return this->m_checkpointIndexPos[index];
}

int CheckpointHandler::getCurrentIndex() const
{
	return this->m_current.first;
}

XMVECTOR CheckpointHandler::getCurrentpos() const
{
	return this->m_current.second;
}