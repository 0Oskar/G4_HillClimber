#pragma once

class CheckpointHandler
{
private:
	std::pair<int, XMVECTOR> m_currentCheckpoint;
	std::vector<std::pair<int, XMVECTOR>> m_checkpointIndexPos;

public:
	CheckpointHandler();
	~CheckpointHandler();

	void addCheckpointGameObject(int index, XMVECTOR gameObjectPosition);

	int getCurrentCheckpointIndex() const;
	XMVECTOR getCurrentCheckpointpos() const;
};