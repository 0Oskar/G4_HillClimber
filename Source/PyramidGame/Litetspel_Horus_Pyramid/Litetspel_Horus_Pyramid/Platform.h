#pragma once
#include"GameObject.h"
#include"Timer.h"

class Platform : public GameObject
{
private:
	const float m_destroyTime = 5.0f;
	const float m_removeTime = 2.0f;
	int m_nrOfTextures = 5; 
	std::wstring m_textures[5] = {
		L"Textures/platformTextureCracks1.png",
		L"Textures/platformTextureCracks2.png",
		L"Textures/platformTextureCracks3.png",
		L"Textures/platformTextureCracks4.png",
		L"Textures/platformTextureCracks5.png"
	};

	Timer m_destructionTimer;
	bool m_shouldDestroy;
	bool m_removeMe;
	int currentTime;


	DirectX::BoundingBox* m_playerBoundingBox;
	DirectX::BoundingOrientedBox* m_pyramidBoundingBox;
	DirectX::BoundingBox platformTriggerBox;

	bool m_hasColided;
	bool canCheckColision;

	void colidesWithPlayer();
public:
	Platform();
	void init(bool colidable, int modelIndex, int wvpCBufferIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl);
	void update(float dt);
	void onPlayerColide();
	void onDestroy();
	void destroy();
	void setPlayerBoundingBox(DirectX::BoundingBox* boundingBox);

};