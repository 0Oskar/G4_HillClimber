#pragma once
#include"MovementComponent.h"
#include"ResourceHandler.h"

class AudioComponent
{
private:
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	std::unique_ptr<DirectX::SoundEffectInstance> m_effect;
	MovementComponent* playerMovementComp;
	DirectX::AudioListener listener;
	DirectX::AudioEmitter emitter;
	ResourceHandler* resourceHandler;
public:
	AudioComponent() 
	{ 
		this->playerMovementComp = nullptr;
		this->resourceHandler = &ResourceHandler::get();
	};
	void init(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
	{
		this->m_audioEngine = audioEngine;
		this->playerMovementComp = playerMovementComponent;
	}
	void loadSound(std::wstring sound)
	{
		if (m_audioEngine)
		{
			this->resourceHandler->loadAudio(sound, this->m_audioEngine);
		}
	}
	void playSound(std::wstring sound) //Play sound to player
	{
		if (m_effect)
		{
			if (m_effect->GetState() == DirectX::SoundState::PLAYING)
			{
				m_effect->Stop();
			}
		}
		m_effect = resourceHandler->getAudio(sound)->CreateInstance();
		m_effect->Play();
	}
	void emitSound(std::wstring sound, DirectX::XMVECTOR pos) //Used for 3d
	{
		listener.SetPosition(playerMovementComp->position);
		emitter.SetPosition(pos);
		if (m_effect)
		{
			if (m_effect->GetState() == DirectX::SoundState::PLAYING)
			{
				m_effect->Stop();
			}
		}
		m_effect = resourceHandler->getAudio(sound)->CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters);
		m_effect->Play();
		m_effect->Apply3D(listener, emitter);
	}
};