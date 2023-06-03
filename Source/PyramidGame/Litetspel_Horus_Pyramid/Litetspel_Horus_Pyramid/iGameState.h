#pragma once
#include"GameObject.h"
#include"Model.h"
#include"ConstantBuffer.h"
#include"Camera.h"

enum class states
{
	NONE,
	MENU,
	GAME,
	WON,
	PAUSE,
	POP,
};

class iGameState
{

protected:
	std::unordered_map<std::string, Model> m_models;
	std::unordered_map<std::string, Model>* m_modelsPtr;
	std::vector<GameObject*> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>> m_wvpCBuffers;
	constantBufferData m_constantbufferData;
	GameOptions m_gameOptions;
	const std::string m_fileToRead = "HighScore.txt";

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;
	Camera m_camera;
	ResourceHandler* m_resourceHandler;

public:
	virtual ~iGameState() {};
	virtual void update(const float dt) = 0;
	virtual states handleInput(Keyboard* keyboard, Mouse* mouePointer, const float dt) = 0;
	virtual std::unordered_map<std::string, Model>* getModelsPtr() = 0;
	virtual std::vector<GameObject*>* getGameObjectsPtr() = 0;
	virtual std::vector<BoundingBox>* getActiveRoomBoundingBoxesPtr() = 0;
	virtual std::vector<BoundingOrientedBox>* getActiveRoomOrientedBoundingBoxesPtr() = 0;
	virtual constantBufferData* getConstantBufferData() = 0;
	virtual std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr() = 0;
	virtual DirectX::XMMATRIX* getViewMatrix() const = 0;
	virtual DirectX::XMMATRIX* getProjectionMatrix() const = 0;
	virtual void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, const GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine, volatile bool* doneLoadingModels) = 0;
	virtual XMFLOAT3 getCameraPos() const = 0;
	virtual void afterChange() = 0;
	virtual void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr) = 0;
	virtual void onEntry() {};
	virtual void onLeave() {};
	void setModelPointer(std::unordered_map<std::string, Model>* mdlPointer)
	{
		m_modelsPtr = mdlPointer;
	}
	virtual void onPop() 
	{
		for (size_t i = 0; i < m_gameObjects.size(); i++)
		{
			if(m_gameObjects.at(i) != nullptr)
				delete m_gameObjects.at(i);
		}
	};
	virtual void addGameObjectToWorld(const bool dynamic, const bool colide, const float weight, Model* mdl, const DirectX::XMVECTOR position, const DirectX::XMVECTOR scale3D, const DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), const DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), const DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1))
	{
		m_gameObjects.emplace_back(new GameObject());
		GameObject* gObject = m_gameObjects.back();
		m_wvpCBuffers.emplace_back();
		m_wvpCBuffers.back().init(m_device, m_dContext);
		int bufferIndex = (int)m_wvpCBuffers.size() - 1;
		if (dynamic)
		{
			gObject->initializeDynamic(colide, false, bufferIndex, weight, acceleration, deceleration, mdl);
		}
		else
		{
			gObject->initializeStatic(colide, bufferIndex, mdl);
		}

		gObject->setScale(scale3D);
		gObject->setPosition(position);

		if (colide)
		{


		}
		if (boundingBoxSize.x == 0 && boundingBoxSize.y == 0 && boundingBoxSize.z == 0)
		{

		}
		else
			gObject->setBoundingBox(boundingBoxSize);
	}



	bool m_majorChange;
};