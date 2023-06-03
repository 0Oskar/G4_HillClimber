#include "pch.h"
#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
	m_current.first = -1;
	m_current.second = {0.f, 0.f, 0.f, 0.f};
}

CheckpointHandler::~CheckpointHandler() {}

void CheckpointHandler::setCurrent(int index, XMVECTOR position)
{
	m_current.first = index;
	m_current.second = position;
}

void CheckpointHandler::addCheckpointGameObject(int index, XMVECTOR gameObjectPosition)
{
	m_checkpointIndexPos.push_back(std::make_pair(index, gameObjectPosition));
}

uint32_t CheckpointHandler::size() const
{
	return (uint32_t)m_checkpointIndexPos.size();
}

std::pair<int, XMVECTOR> CheckpointHandler::getIndexPosAt(int index) const
{
	return m_checkpointIndexPos[index];
}

int CheckpointHandler::getCurrentIndex() const
{
	return m_current.first;
}

XMVECTOR CheckpointHandler::getCurrentpos() const
{
	return m_current.second;
}

std::pair<int, XMVECTOR> CheckpointHandler::getCurrent() const
{
	return m_current;
}
