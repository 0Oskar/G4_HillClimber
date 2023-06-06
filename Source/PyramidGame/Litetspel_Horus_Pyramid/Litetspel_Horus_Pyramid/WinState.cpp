#include"pch.h"
#include"WinState.h"

void WinState::readHighScoreFromFile()
{
	std::ifstream inStream;
	inStream.open(m_fileToRead);

	int score = 0;
	std::string name = "";
	if (inStream.is_open())
	{
		while (!inStream.eof() && m_nrOfHighScore != 10)
		{
			inStream >> name;
			inStream >> score;
			if (name != "")
			{
				m_score[m_nrOfHighScore].first = name;
				m_score[m_nrOfHighScore++].second = score;
				name = "";
			}

		}
		inStream.close();
	}
}

WinState::WinState()
{
	m_device = nullptr;
	m_dContext = nullptr;
	m_cameraMovementComponentPtr = nullptr;
	m_audioComponent = nullptr;
	m_ownTime = 0;
}

WinState::~WinState()
{
	if (m_audioComponent != nullptr)
	{
		delete m_audioComponent;
		m_audioComponent = nullptr;
	}
	if (m_cameraMovementComponentPtr)
	{
		delete m_cameraMovementComponentPtr;
		m_cameraMovementComponentPtr = nullptr;
	}
}

void WinState::setupLight()
{
	m_constantbufferData.dirBuffer.lightColor = { 1, 1, 1, 1 };
	m_constantbufferData.dirBuffer.lightDirection = { 1, 1, 1, 1 };
}

void WinState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine, volatile bool* doneLoadingModels)
{


	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	m_device = device;
	m_dContext = dContext;
	m_cameraMovementComponentPtr = new MovementComponent;
	m_audioComponent = new AudioComponent();
	m_gameOptions = options;

	m_audioComponent->init(audioEngine, m_cameraMovementComponentPtr, 1);
	m_audioComponent->loadSound(winSound);

	m_camera.followMoveComp(m_cameraMovementComponentPtr);
	m_camera.initialize(options.mouseSensitivity, options.fov, (float)options.width / (float)options.height, 0.1f, 10000.f);

	readHighScoreFromFile();


	m_cameraMovementComponentPtr->position = { 0, 100, 0 };

}


void WinState::update(float dt)
{
	for (size_t i = 0; i < m_gameObjects.size(); i++)
	{
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = m_camera.getViewMatrix() * m_camera.getProjectionMatrix();
		wvpData.wvp = m_gameObjects.at(i)->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = m_gameObjects[i]->getWorldMatrix();

		m_wvpCBuffers[m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (m_gameObjects[i]->m_removeMe)
		{
			m_gameObjects[i]->setVisibility(false);
			m_gameObjects[i]->setIfCollidable(false);
		}
	}

	m_camera.updateView();
}


states WinState::handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt)
{
	states changeStateTo = states::NONE;
	if (!keyboard->empty())
	{
		KeyboardEvent keyboardEvent = keyboard->readKey();
		if (keyboardEvent.getEvent() == Event::Pressed)
		{
			if (keyboardEvent.getKey() == VK_ESCAPE)
				changeStateTo = states::POP;
			else if (keyboardEvent.getKey() == VK_RETURN) // ENTER
				changeStateTo = states::GAME;
			else if (keyboardEvent.getKey() == VK_OEM_PLUS) // '+' key
				changeStateTo = states::RELOAD_SHADERS;
		}
	}

	return changeStateTo;
}
void WinState::afterChange()
{

}


void WinState::onEntry()
{
	m_audioComponent->playSound(winSound, 0, true, 1.0f, 0.5f);
}

void WinState::onLeave()
{
	m_audioComponent->stopSound(0);
}

void WinState::onPop()
{
	iGameState::onPop();

	if (m_cameraMovementComponentPtr != nullptr)
		delete m_cameraMovementComponentPtr;

	if (m_audioComponent != nullptr)
		delete m_audioComponent;
}

std::unordered_map<std::string, Model>* WinState::getModelsPtr()
{
	return m_modelsPtr;
}

std::vector<GameObject*>* WinState::getGameObjectsPtr()
{
	return &m_gameObjects;
}

std::vector<BoundingBox>* WinState::getActiveRoomBoundingBoxesPtr()
{
	return nullptr;
}

std::vector<BoundingOrientedBox>* WinState::getActiveRoomOrientedBoundingBoxesPtr()
{
	return nullptr;
}

constantBufferData* WinState::getConstantBufferData()
{
	return &m_constantbufferData;
}

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* WinState::getWvpCBuffersPtr()
{
	return &m_wvpCBuffers;
}

DirectX::XMMATRIX* WinState::getViewMatrix() const
{
	return m_camera.getViewMatrixPtr();
}

DirectX::XMMATRIX* WinState::getProjectionMatrix() const
{
	return m_camera.getProjectionMatrixPtr();
}


XMFLOAT3 WinState::getCameraPos() const
{
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, m_cameraMovementComponentPtr->position);
	return  camPos;
}

void WinState::setGameTime(int time)
{
	m_ownTime = time;
}

void WinState::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	std::string winText = "You have conquered the pyramid of the gods!";
	std::string ownScoreText = "Your time: " + std::to_string(m_ownTime) +  " seconds!";
	std::string titleName = "-Local highscore-";
	std::string highScoreString;
	spriteFontPtr->DrawString(spriteBatchPtr, winText.c_str(), DirectX::XMFLOAT2((float)m_gameOptions.width / 2.f, 40.f), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(200.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, ownScoreText.c_str(), DirectX::XMFLOAT2((float)m_gameOptions.width / 2.f, 60.f), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(100.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, titleName.c_str(), DirectX::XMFLOAT2((float)m_gameOptions.width / 2.f, 100.f), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, L"-Name-", DirectX::XMFLOAT2((float)m_gameOptions.width / 3.f, 120.f), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, L"-Time-", DirectX::XMFLOAT2((float)m_gameOptions.width / 1.5f, 120.f), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, L"Press esc to return to the start menu", DirectX::XMFLOAT2((float)m_gameOptions.width / 2.f, (float)m_gameOptions.height - 60), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(170.f, 0.f));

	for (int i = 0; i < m_nrOfHighScore; i++)
	{
		highScoreString = m_score[i].first + " " + std::to_string(m_score[i].second) + " seconds!";
		spriteFontPtr->DrawString(spriteBatchPtr, m_score[i].first.c_str(), DirectX::XMFLOAT2((float)m_gameOptions.width / 3.f, 150.f + i * 30.f), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
		spriteFontPtr->DrawString(spriteBatchPtr, std::to_string(m_score[i].second).c_str(), DirectX::XMFLOAT2((float)m_gameOptions.width / 1.5f - (int)std::to_string((float)m_score[i].second).size() * 12.f, 150.f + i * 30.f), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
		spriteFontPtr->DrawString(spriteBatchPtr, L"seconds", DirectX::XMFLOAT2((float)m_gameOptions.width / 1.5f + 8.f, 150.f + i * 30.f), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	}
}