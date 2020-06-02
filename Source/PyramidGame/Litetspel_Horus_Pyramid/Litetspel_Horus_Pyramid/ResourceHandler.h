#pragma once
#include"pch.h"
#include"WICTextureLoader.h"


class ResourceHandler
{
private:
	ResourceHandler() {};
	std::map<const std::wstring, ID3D11ShaderResourceView*> map;
	std::map<const std::wstring, DirectX::SoundEffect*> audioMap;
	bool createTextureFromFile(ID3D11Device* device, const WCHAR* txturePath)
	{
		bool couldLoad = true;
		map[txturePath] = nullptr;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, txturePath, nullptr, &map[txturePath]);
		if (FAILED(hr))
		{
			couldLoad = false;
		}
		assert(SUCCEEDED(hr) && "Error, failed to load texture file!");

		return  couldLoad;
	}
public:
	static ResourceHandler& get()
	{
		static ResourceHandler handlerInstance;
		return handlerInstance;
	}
	ResourceHandler(ResourceHandler const&) = delete;
	void operator=(ResourceHandler const&) = delete;
	~ResourceHandler()
	{
		std::map<const std::wstring, ID3D11ShaderResourceView*>::iterator i = map.begin();
		while(i != map.end())
		{
			i->second->Release();
			i++;
		}
		std::map<const std::wstring, DirectX::SoundEffect*>::iterator ai = audioMap.begin();
		while (ai != audioMap.end())
		{
			delete ai->second;
			ai++;
		}
	}

	ID3D11ShaderResourceView* getTexture(const WCHAR* texturePath)
	{
		if (map[texturePath] == nullptr)
		{
			if (m_device != nullptr)
			{
				createTextureFromFile(m_device, texturePath);
			}
			return map[texturePath];
		}
		else
		{
			return map[texturePath];
		}
	}

	void loadAudio(const std::wstring sound, const std::shared_ptr<DirectX::AudioEngine> m_audioEngine)
	{
		if (audioMap[sound] == nullptr)
		{
			audioMap[sound] = new DirectX::SoundEffect(m_audioEngine.get(), sound.c_str());
		}
	}

	DirectX::SoundEffect* getAudio(const std::wstring sound)
	{
		if (audioMap[sound] == nullptr)
		{
			assert(false && "Error, trying to get audio not yet loaded!");
		}

		return audioMap[sound];
	}

	

	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_dContext = nullptr;

};