#pragma once
#include"iGameState.h"
#include"PyramidRoom.h"

class MenuState : public iGameState
{
private:
	MovementComponent* m_cameraMovementComponentPtr;
	AudioComponent* m_audioComponent;
	PyramidRoom m_pyramidRoom;
	BoundingOrientedBox pyramid;
	void setupLight();

	const XMVECTOR m_moveAroundPosConst = {0, 200, 300};
	const float m_radiusConst = 200.0f;
	float m_angle;
	const XMVECTOR m_lookAtPosConst = { 0, 200, 150 };

	//Sound
	const std::wstring menuSound = L"Sounds/MainMenu.wav";
public:
	MenuState();
	~MenuState();
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	void update(float dt);
	void afterChange();
	void loadModels();
	states handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt);

	//Get
	std::vector<Model>* getModelsPtr();
	std::vector<GameObject*>* getGameObjectsPtr();
	constantBufferData* getConstantBufferData();
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr();
	DirectX::XMMATRIX* getViewMatrix() const;
	DirectX::XMMATRIX* getProjectionMatrix() const;
	XMFLOAT3 getCameraPos();

	void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr);
	void onEntry();
	void onLeave();
	void onPop();

};