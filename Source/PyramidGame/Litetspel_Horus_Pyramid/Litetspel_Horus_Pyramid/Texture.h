#pragma once
#include"pch.h"
#include"WICTextureLoader.h"
//class Texture
//{
//
//private:
//
//public:
//	Texture() { m_texturePtr = nullptr; };
//	//Texture() {}
//	bool init(ID3D11Device* device, const WCHAR* txturePath)
//	{
//		bool couldLoad = true;
//		HRESULT hr = DirectX::CreateWICTextureFromFile(device, L"Textures/pyramidTextur.png", nullptr, &m_texturePtr);
//		if (FAILED(hr))
//		{
//			couldLoad = false;
//		}
//
//		return  couldLoad;
//	}
//	ID3D11ShaderResourceView** getTexture()
//	{
//		return &m_texturePtr;
//	}
//
//};

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