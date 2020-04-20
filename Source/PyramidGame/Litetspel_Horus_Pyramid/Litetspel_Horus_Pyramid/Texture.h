#pragma once
#include"pch.h"
#include"WICTextureLoader.h"

class TextureHandler
{
private:
	TextureHandler() {};
	std::map<const WCHAR*, ID3D11ShaderResourceView*> map;

	bool createTextureFromFile(ID3D11Device* device, const WCHAR* txturePath)
	{
		bool couldLoad = true;
		map[txturePath] = nullptr;
		HRESULT hr = DirectX::CreateWICTextureFromFile(device, txturePath, nullptr, &map[txturePath]);
		if (FAILED(hr))
		{
			couldLoad = false;
		}

		return  couldLoad;
	}
public:
	static TextureHandler& get()
	{
		static TextureHandler handlerInstance;
		return handlerInstance;
	}
	TextureHandler(TextureHandler const&) = delete;
	void operator=(TextureHandler const&) = delete;
	~TextureHandler()
	{
		std::map<const WCHAR*, ID3D11ShaderResourceView*>::iterator i = map.begin();
		while(i != map.end())
		{
			i->second->Release();
			i++;
		}
	}

	ID3D11ShaderResourceView* getTexture(const WCHAR* texturePath, ID3D11Device* device = nullptr)
	{

		if (map[texturePath] == nullptr)
		{
			if (device != nullptr)
			{
				createTextureFromFile(device, texturePath);
			}
			return map[texturePath];
		}
		else
		{
			return map[texturePath];
		}
	}

};