#include"pch.h"
#include"WinState.h"

void WinState::readHighScoreFromFile()
{
	std::ifstream inStream;
	inStream.open(this->m_fileToRead);

	int score = 0;
	std::string name = "";
	if (inStream.is_open())
	{
		while (!inStream.eof() && nrOfHighScore != 10)
		{
			inStream >> name;
			inStream >> score;
			if (name != "")
			{
				this->score[nrOfHighScore].first = name;
				this->score[nrOfHighScore++].second = score;
				name = "";
			}

		}
		inStream.close();
	}
}

WinState::WinState()
{
	this->m_device = nullptr;
	this->m_dContext = nullptr;
	this->m_cameraMovementComponentPtr = nullptr;
	this->m_audioComponent = nullptr;
	this->ownTime = 0;
}

WinState::~WinState()
{
	if (this->m_audioComponent != nullptr)
	{
		delete this->m_audioComponent;
		this->m_audioComponent = nullptr;
	}
	if (this->m_cameraMovementComponentPtr)
	{
		delete this->m_cameraMovementComponentPtr;
		this->m_cameraMovementComponentPtr = nullptr;
	}
}

void WinState::setupLight()
{
	this->m_constantbufferData.dirBuffer.lightColor = { 1, 1, 1, 1 };
	this->m_constantbufferData.dirBuffer.lightDirection = { 1, 1, 1, 1 };
}

void WinState::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, GameOptions options, std::shared_ptr<DirectX::AudioEngine> audioEngine, volatile bool* doneLoadingModels)
{


	DirectX::XMVECTOR NormalScale = DirectX::XMVectorSet(1, 1, 1, 1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);
	DirectX::XMVECTOR rotation = DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f);


	this->m_device = device;
	this->m_dContext = dContext;
	this->m_cameraMovementComponentPtr = new MovementComponent;
	this->m_audioComponent = new AudioComponent();
	this->m_gameOptions = options;

	this->m_audioComponent->init(audioEngine, m_cameraMovementComponentPtr, 1);
	this->m_audioComponent->loadSound(this->winSound);

	this->m_camera.followMoveComp(m_cameraMovementComponentPtr);
	this->m_camera.initialize(options.mouseSensitivity, options.fov, (float)options.width / (float)options.height, 0.1f, 1000.f);

	this->readHighScoreFromFile();


	this->m_cameraMovementComponentPtr->position = { 0, 100, 0 };

}


void WinState::update(float dt)
{
	for (size_t i = 0; i < this->m_gameObjects.size(); i++)
	{
		VS_CONSTANT_BUFFER wvpData;
		DirectX::XMMATRIX viewPMtrx = this->m_camera.getViewMatrix() * this->m_camera.getProjectionMatrix();
		wvpData.wvp = this->m_gameObjects.at(i)->getWorldMatrix() * viewPMtrx;
		wvpData.worldMatrix = this->m_gameObjects[i]->getWorldMatrix();

		this->m_wvpCBuffers[this->m_gameObjects[i]->getWvpCBufferIndex()].upd(&wvpData);
		if (this->m_gameObjects[i]->m_removeMe)
		{
			this->m_gameObjects[i]->setVisibility(false);
			this->m_gameObjects[i]->setIfCollidable(false);
		}
	}

	this->m_camera.updateView();
}


states WinState::handleInput(Keyboard* keyboard, Mouse* mousePtr, float dt)
{
	states changeStateTo = states::NONE;
	if (!keyboard->empty())
	{
		KeyboardEvent keyboardEvent = keyboard->readKey();
		if (keyboardEvent.getEvent() == Event::Pressed)
		{
			if (keyboardEvent.getKey() == (char)27)
				changeStateTo = states::POP;
			else if (keyboardEvent.getKey() == (char)13)
				changeStateTo = states::GAME;
		}
	}

	return changeStateTo;
}
void WinState::afterChange()
{

}


void WinState::onEntry()
{
	this->m_audioComponent->playSound(this->winSound, 0, true, 1.0f, 0.5f);
}

void WinState::onLeave()
{
	this->m_audioComponent->stopSound(0);
}

void WinState::onPop()
{
	iGameState::onPop();

	if (this->m_cameraMovementComponentPtr != nullptr)
		delete m_cameraMovementComponentPtr;

	if (m_audioComponent != nullptr)
		delete m_audioComponent;
}

std::vector<Model>* WinState::getModelsPtr()
{
	return this->m_modelsPtr;
}

std::vector<GameObject*>* WinState::getGameObjectsPtr()
{
	return &this->m_gameObjects;
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
	return &this->m_constantbufferData;
}

std::vector<ConstBuffer<VS_CONSTANT_BUFFER>>* WinState::getWvpCBuffersPtr()
{
	return &this->m_wvpCBuffers;
}

DirectX::XMMATRIX* WinState::getViewMatrix() const
{
	return this->m_camera.getViewMatrixPtr();
}

DirectX::XMMATRIX* WinState::getProjectionMatrix() const
{
	return this->m_camera.getProjectionMatrixPtr();
}


XMFLOAT3 WinState::getCameraPos() const
{
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, this->m_cameraMovementComponentPtr->position);
	return  camPos;
}

void WinState::setGameTime(int time)
{
	this->ownTime = time;
}

void WinState::drawUI(DirectX::SpriteBatch* spriteBatchPtr, DirectX::SpriteFont* spriteFontPtr)
{
	std::string winText = "You have conquered the pyramid of the gods!";
	std::string ownScoreText = "Your time: " + std::to_string(ownTime) +  " seconds!";
	std::string titleName = "-Local highscore-";
	std::string highScoreString;
	spriteFontPtr->DrawString(spriteBatchPtr, winText.c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 2, 40), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(200.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, ownScoreText.c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 2, 60), DirectX::Colors::Green, 0.f, DirectX::XMFLOAT2(100.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, titleName.c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 2, 100), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, L"-Name-", DirectX::XMFLOAT2((float)this->m_gameOptions.width / 3, 120), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, L"-Time-", DirectX::XMFLOAT2((float)this->m_gameOptions.width / 1.5, 120), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	spriteFontPtr->DrawString(spriteBatchPtr, L"Press esc to return to the start menu", DirectX::XMFLOAT2((float)this->m_gameOptions.width / 2, (float)this->m_gameOptions.height - 60), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(170.f, 0.f));

	for (int i = 0; i < this->nrOfHighScore; i++)
	{
		highScoreString = this->score[i].first + " " + std::to_string(this->score[i].second) + " seconds!";
		spriteFontPtr->DrawString(spriteBatchPtr, this->score[i].first.c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 3, 150 + i * 30), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
		spriteFontPtr->DrawString(spriteBatchPtr, std::to_string(this->score[i].second).c_str(), DirectX::XMFLOAT2((float)this->m_gameOptions.width / 1.5 - (int)std::to_string(this->score[i].second).size() * 12, 150 + i * 30), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
		spriteFontPtr->DrawString(spriteBatchPtr, L"seconds", DirectX::XMFLOAT2((float)this->m_gameOptions.width / 1.5 + 8, 150 + i * 30), DirectX::Colors::Yellow, 0.f, DirectX::XMFLOAT2(70.f, 0.f));
	}
}