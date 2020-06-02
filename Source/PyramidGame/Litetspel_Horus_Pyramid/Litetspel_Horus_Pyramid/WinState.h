#pragma once
#include"iGameState.h"
#include"AudioComponent.h"

class WinState : public iGameState
{
private:
	MovementComponent* m_cameraMovementComponentPtr;
	AudioComponent* m_audioComponent;
	void setupLight();
	std::pair<std::string, int> score[10];
	int nrOfHighScore;
	int ownTime;

	//Sound
	const std::wstring winSound = L"Sounds/MainMenu.wav";


	void readHighScoreFromFile();
public:
	WinState();
	~WinState();
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, const GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine);
	void update(const float dt);
	void afterChange();
	states handleInput(Keyboard* keyboard, Mouse* mousePtr, const float dt);

	//Get
	std::vector<Model>* getModelsPtr();
	std::vector<GameObject*>* getGameObjectsPtr();
	constantBufferData* getConstantBufferData();
	std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* getWvpCBuffersPtr();
	DirectX::XMMATRIX* getViewMatrix() const;
	DirectX::XMMATRIX* getProjectionMatrix() const;
	XMFLOAT3 getCameraPos() const;

	void setGameTime(const int time);

	void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr);
	void onEntry();
	void onLeave();
	void onPop();
};