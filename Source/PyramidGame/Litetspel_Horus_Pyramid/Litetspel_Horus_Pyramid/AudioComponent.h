#pragma once
#include"MovementComponent.h"
#include"ResourceHandler.h"

class AudioComponent
{
private:
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	std::vector<std::unique_ptr<DirectX::SoundEffectInstance>> m_effect;
	MovementComponent* playerMovementComp;
	DirectX::AudioListener listener;
	DirectX::AudioEmitter emitter;
	ResourceHandler* resourceHandler;
public:
	AudioComponent() 
	{ 
		playerMovementComp = nullptr;
		resourceHandler = &ResourceHandler::get();
	};
	~AudioComponent()
	{
		//playerMovementComp = nullptr;
		for (int i = 0; i < (int)m_effect.size(); i++)
		{
			if (m_effect.at(i) != nullptr)
			{
				if (m_effect.at(i)->GetState() == DirectX::SoundState::PLAYING)
				{
					m_effect.at(i)->Stop();
				}
				//m_effect.at(i).release();
			}
		}
	}
	void init(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent, size_t nrOfChannels = 1)
	{
		m_audioEngine = audioEngine;
		playerMovementComp = playerMovementComponent;
		m_effect.resize(nrOfChannels);
		for (size_t i = 0; i < nrOfChannels; i++)
		{
			m_effect.emplace_back();
		}
	}
	void loadSound(std::wstring sound)
	{
		if (m_audioEngine)
		{
			resourceHandler->loadAudio(sound, m_audioEngine);
		}
	}
	void playSound(std::wstring sound, int channelNr = 0, bool loop = false, float volume = 1.0f, float pitch = 1.0f) //Play sound to player
	{
		if (m_effect.at(channelNr))
		{
			if (m_effect.at(channelNr)->GetState() == DirectX::SoundState::PLAYING)
			{
				m_effect.at(channelNr)->Stop();
			}
		}
		m_effect.at(channelNr) = resourceHandler->getAudio(sound)->CreateInstance();
		m_effect.at(channelNr)->SetVolume(volume);
		m_effect.at(channelNr)->SetPitch(pitch);
		m_effect.at(channelNr)->Play(loop);
	}
	void stopSound(int channelNr)
	{
		if (m_effect.at(channelNr))
		{
			if (m_effect.at(channelNr)->GetState() == DirectX::SoundState::PLAYING)
			{
				m_effect.at(channelNr)->Stop();
			}
		}
	}
	void emitSound(std::wstring sound, DirectX::XMVECTOR pos, int channelNr = 0, bool loop = false, float volume = 1.0f, float pitch = 1.0f) //Used for 3d
	{
		listener.SetPosition(playerMovementComp->position);
		emitter.SetPosition(pos);
		if (m_effect.at(channelNr))
		{
			if (m_effect.at(channelNr)->GetState() == DirectX::SoundState::PLAYING)
			{
				m_effect.at(channelNr)->Stop();
			}
		}
		m_effect.at(channelNr) = resourceHandler->getAudio(sound)->CreateInstance(DirectX::SOUND_EFFECT_INSTANCE_FLAGS::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters);
		m_effect.at(channelNr)->SetVolume(volume);
		m_effect.at(channelNr)->SetPitch(pitch);
		m_effect.at(channelNr)->Play(loop);
		m_effect.at(channelNr)->Apply3D(listener, emitter);
	}
};