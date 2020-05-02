#pragma once
#include"MovementComponent.h"

class AudioComponent
{
private:
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	std::vector<DirectX::SoundEffect*> m_soundEffects;
	std::unique_ptr<DirectX::SoundEffectInstance> m_effect;
	MovementComponent* playerMovementComp;
	DirectX::AudioListener listener;
	DirectX::AudioEmitter emitter;

public:
	AudioComponent() { this->playerMovementComp = nullptr; };
	void init(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
	{
		this->m_audioEngine = audioEngine;
		this->playerMovementComp = playerMovementComponent;
	}
	int loadSound(std::wstring sound)
	{
		if (m_audioEngine)
		{
			m_soundEffects.emplace_back(new DirectX::SoundEffect(m_audioEngine.get(), sound.c_str()));
		}
		else
			return -1;
		return m_soundEffects.size() - 1;
	}
	void playSound(int i) //Play sound to player
	{
		if (m_effect)
		{
			if (m_effect->GetState() == DirectX::SoundState::PLAYING)
			{
				m_effect->Stop();
			}
		}
		m_effect = m_soundEffects.at(i)->CreateInstance();
		m_effect->Play();
	}
	void emitSound(int i, DirectX::XMVECTOR pos) //Used for 3d
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
		m_effect = m_soundEffects.at(i)->CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters);
		m_effect->Play();
		m_effect->Apply3D(listener, emitter);
	}
};