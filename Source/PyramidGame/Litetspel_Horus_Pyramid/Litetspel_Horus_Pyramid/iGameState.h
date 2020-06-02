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
	PAUSE,
	POP,
};

class iGameState
{

protected:
	std::vector<Model> m_models;
	std::vector<Model>* m_modelsPtr;
	std::vector<GameObject*> m_gameObjects;
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>> m_wvpCBuffers;
	constantBufferData m_constantbufferData;
	GameOptions m_gameOptions;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;
	Camera m_camera;
	ResourceHandler* m_resourceHandler;

public:
	virtual void update(float dt) = 0;
	virtual states handleInput(Keyboard* keyboard, Mouse* mouePointer, float dt) = 0;
	virtual std::vector<Model>* getModelsPtr() = 0;
	virtual std::vector<GameObject*>* getGameObjectsPtr() = 0;
	virtual std::vector<BoundingBox>* getActiveRoomBoundingBoxesPtr() = 0;
	virtual std::vector<BoundingOrientedBox>* getActiveRoomOrientedBoundingBoxesPtr() = 0;
	virtual constantBufferData* getConstantBufferData() = 0;
	virtual std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr() = 0;
	virtual DirectX::XMMATRIX* getViewMatrix() const = 0;
	virtual DirectX::XMMATRIX* getProjectionMatrix() const = 0;
	virtual void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine) = 0;
	virtual XMFLOAT3 getCameraPos() = 0;
	virtual void afterChange() = 0;
	virtual void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr) = 0;
	virtual void onEntry() {};
	virtual void onLeave() {};
	void setModelPointer(std::vector<Model>* mdlPointer)
	{
		this->m_modelsPtr = mdlPointer;
	}
	virtual void onPop() 
	{
		for (size_t i = 0; i < this->m_gameObjects.size(); i++)
		{
			if(m_gameObjects.at(i) != nullptr)
				delete m_gameObjects.at(i);
		}
	};
	virtual void addGameObjectToWorld(bool dynamic, bool colide, float weight, int mdlIndx, Model* mdl, DirectX::XMVECTOR position, DirectX::XMVECTOR scale3D, DirectX::XMFLOAT3 boundingBoxSize = DirectX::XMFLOAT3(0, 0, 0), DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3 deceleration = DirectX::XMFLOAT3(1, 1, 1))
	{
		this->m_gameObjects.emplace_back(new GameObject());
		GameObject* gObject = this->m_gameObjects.back();
		this->m_wvpCBuffers.emplace_back();
		this->m_wvpCBuffers.back().init(m_device, m_dContext);
		int bufferIndex = (int)m_wvpCBuffers.size() - 1;
		if (dynamic)
		{
			gObject->initializeDynamic(colide, false, mdlIndx, bufferIndex, weight, acceleration, deceleration, mdl);
		}
		else
		{
			gObject->initializeStatic(colide, mdlIndx, bufferIndex, mdl);
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