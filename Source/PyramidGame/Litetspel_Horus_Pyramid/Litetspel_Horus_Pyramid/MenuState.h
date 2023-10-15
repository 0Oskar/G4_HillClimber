#pragma once
#include"iGameState.h"
#include"PyramidRoom.h"

#define MODEL_COUNT 50

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
	const std::wstring m_menuSound = L"Sounds/MainMenu.wav";

	// UI
	ID3D11ShaderResourceView* m_titelImage;
	ID3D11ShaderResourceView* m_infoImage;
	DirectX::XMFLOAT2 m_titelImagePosition;
	DirectX::XMFLOAT2 m_infoImagePosition;

	// Model Loading
	volatile bool* m_doneLoadingModelsPtr;
	std::vector<const char*> m_asyncModelNames;
	std::future<void> m_asyncModelsFuture[MODEL_IMPORT_THREAD_COUNT];
	void loadModels();
	
public:
	MenuState();
	~MenuState();
	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, const GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine, volatile bool* doneLoadingModels);
	void update(const float dt);
	void afterChange();
	states handleInput(Keyboard* keyboard, Mouse* mousePtr, const float dt);

	// Getters
	std::unordered_map<std::string, Model>* getModelsPtr();
	std::vector<GameObject*>* getGameObjectsPtr();
	std::vector<BoundingBox>* getActiveRoomBoundingBoxesPtr(); // Empty
	std::vector<BoundingOrientedBox>* getActiveRoomOrientedBoundingBoxesPtr(); // Empty
	PS_PER_FRAME_BUFFER* getPerFrameData();
	std::vector<ConstBuffer<VS_WVPW_CONSTANT_BUFFER>>* getWvpCBuffersPtr();
	Camera* getCamera();

	void drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr);
	void onEntry();
	void onLeave();
	void onPop();

	void addNewModelInitAsync(const char* modelFilePath, const MaterialData material, const wchar_t* texturePath);
	void addNewModelInit(const char* modelFilePath, const MaterialData material, const wchar_t* texturePath);
	static void loadModelsThreadSafe(uint32_t threadIndex, std::unordered_map<std::string, Model>* modelsList, std::vector<const char*>* modelNameList, uint32_t from, uint32_t to);
};