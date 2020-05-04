#pragma once

#include "GameObject.h"

const int NR_OF_CHAIN_LINKS = 30;

class Chain
{
private:
	bool m_visible;
	bool m_shooting;
	std::vector<GameObject*>* m_chainGObjects;
	GameObject* m_hookGObject;
	GameObject* m_gaunletGObject;

	float m_constant;
	float m_length;
	float m_friction;

public:
	Chain();
	~Chain();

	void initialize(GameObject* hookGObject, GameObject* gaunletGObject, std::vector<GameObject*>* chainGObjects);

	bool isVisible() const;

	void setVisibility(bool visible);
	void setShooting(bool shooting);

	void update(float dt);
};