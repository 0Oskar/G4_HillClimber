#pragma once

#include "GameObject.h"

const int NR_OF_CHAIN_LINKS = 100;

class Chain
{
private:
	bool m_visible;
	bool m_shooting;
	bool m_retracting;
	Timer m_timer;
	std::vector<GameObject*>* m_chainGObjects;
	std::vector<bool> m_chainRetracted;
	int m_nrOfUnretractedLinks;
	GameObject* m_hookGObject;
	GameObject* m_gaunletGObject;

	float m_constant;
	float m_length;
	float m_friction;

	void simulateLinks(GameObject* link1, GameObject* link2, bool notFirst, float dt);
	void linkRetractionUpdate();

public:
	Chain();
	~Chain();

	void initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects);

	bool isVisible() const;

	void setVisibility(bool visible);
	void setShooting(bool shooting);
	void setRetracting(bool retracting);

	void update(float dt);
};