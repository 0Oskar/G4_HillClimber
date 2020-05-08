#include "pch.h"
#include "CheckpointHandler.h"

CheckpointHandler::CheckpointHandler()
{
	this->m_currentCheckpoint.first = -1;
	this->m_currentCheckpoint.second = {0.f, 0.f, 0.f, 0.f};
}

CheckpointHandler::~CheckpointHandler() {}

void CheckpointHandler::addCheckpointGameObject(int index, XMVECTOR gameObjectPosition)
{
	this->m_checkpointIndexPos.push_back(std::make_pair(index, gameObjectPosition));
}

int CheckpointHandler::getCurrentCheckpointIndex() const
{
	return this->m_currentCheckpoint.first;
}

XMVECTOR CheckpointHandler::getCurrentCheckpointpos() const
{
	return this->m_currentCheckpoint.second;
}