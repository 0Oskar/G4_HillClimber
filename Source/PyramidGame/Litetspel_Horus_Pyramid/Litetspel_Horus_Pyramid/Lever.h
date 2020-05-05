#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "AudioComponent.h"

class Lever : public GameObject
{
private:

	std::wstring m_theTexture;
	std::wstring m_sound;

	DirectX::BoundingBox* m_playerBoundingBox;
	DirectX::BoundingBox leverTriggerBox;

	bool activated;
	bool canUseLever;
	//Model defaultLever;

	AudioComponent* m_audioComponent;

public:
	Lever();

	void init(bool colidable, int modelIndex, int wvpCBufferIndex, Model* mdl);
	void onPlayerColide();
	void activateLever();
	bool getLeverValue();
	bool getCanUseLever();
	void setPlayerBoundingBox(DirectX::BoundingBox* boundingBox);
	void collidesWithPlayer();
	void initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent);

};