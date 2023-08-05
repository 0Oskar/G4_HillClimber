#pragma once

#include "GameObject.h"

#define NR_OF_CHAIN_LINKS 128

class Chain
{
private:
	bool m_visible;
	bool m_shooting;
	bool m_retracting;
	Timer m_timer;
	std::vector<GameObject*>* m_chainGObjects;
	std::vector<bool> m_retractedLinks;
	int m_nrOfUnretractedLinks;
	GameObject* m_hookGObject;
	GameObject* m_gaunletGObject;
	XMVECTOR m_gauntletSpawnPosition;

	float m_constant;
	float m_linkLength;
	float m_friction;

	void simulateLinks(GameObject* link1, GameObject* link2, bool notFirst, float dt);
	void linkRetractionUpdate();
	void calculateGauntletLinkSpawnPosition();

public:
	Chain();
	~Chain();

	void initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects);

	bool isVisible() const;

	XMVECTOR getLinkPosition(uint32_t index);

	void setVisibility(bool visible);
	void setShooting(bool shooting);
	void setRetracting(bool retracting);

	void update(float dt);
};