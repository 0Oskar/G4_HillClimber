#pragma once


class AudioComponent
{
private:
	std::shared_ptr<DirectX::AudioEngine> m_audioEngine;
	std::vector<DirectX::SoundEffect*> m_soundEffects;
	std::unique_ptr<DirectX::SoundEffectInstance> m_effect;

public:
	AudioComponent() {};
	void init(std::shared_ptr<DirectX::AudioEngine> audioEngine)
	{
		this->m_audioEngine = audioEngine;
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
	void playSound(int i)
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
};