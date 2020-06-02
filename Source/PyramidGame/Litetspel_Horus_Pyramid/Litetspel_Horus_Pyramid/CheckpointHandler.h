#pragma once

class CheckpointHandler
{
private:
	std::pair<int, XMVECTOR> m_current;
	std::vector<std::pair<int, XMVECTOR>> m_checkpointIndexPos;

public:
	CheckpointHandler();
	~CheckpointHandler();

	// Setters
	void setCurrent(int index, XMVECTOR position);
	void addCheckpointGameObject(int index, XMVECTOR gameObjectPosition);

	// Getters
	int size() const;
	int getCurrentIndex() const;
	XMVECTOR getCurrentpos() const;
	std::pair<int, XMVECTOR> getCurrent() const;
	std::pair<int, XMVECTOR> getIndexPosAt(int index) const;
};