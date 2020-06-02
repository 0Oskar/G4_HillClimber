#pragma once
#include"GameObject.h"
#include"Timer.h"
#include"AudioComponent.h"

class Platform : public GameObject
{
private:
	const float m_destroyTime = 2.5f;
	const float m_removeTime = 2.0f;
	const int m_nrOfTextures = 5; 
	const std::wstring m_textures[5] = {
		L"Textures/platformTextureCracks1.png",
		L"Textures/platformTextureCracks2.png",
		L"Textures/platformTextureCracks3.png",
		L"Textures/platformTextureCracks4.png",
		L"Textures/platformTextureCracks5.png"
	};
	const int m_nrOfCracKSounds = 3;
	const std::wstring m_crackSounds[3] = {
		L"Sounds/Cracks1.wav",
		L"Sounds/Cracks2.wav",
		L"Sounds/Cracks3.wav"
	};

	Timer m_destructionTimer;
	bool m_shouldDestroy;
	int m_currentTime;


	DirectX::BoundingBox* m_playerBoundingBox;
	DirectX::BoundingOrientedBox* m_pyramidBoundingBox;
	DirectX::BoundingBox m_platformTriggerBox;

	bool m_hasColided;
	bool m_canCheckColision;

	AudioComponent* m_audioComponent;

	void colidesWithPlayer();
public:
	Platform();
	~Platform();
	void init(const bool colidable, const int modelIndex, const int wvpCBufferIndex, DirectX::BoundingOrientedBox* pyramid, Model* mdl);
	void update(const float dt);
	void onPlayerColide();
	void onDestroy();
	void destroy();
	void setPlayerBoundingBox(DirectX::BoundingBox* boundingBox);
	void initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent);
	void pushToLocation(const XMVECTOR position);
};