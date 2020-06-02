#include"pch.h"
#include"Platform.h"

Platform::Platform()
{
	this->m_shouldDestroy = false;
	this->m_playerBoundingBox = nullptr;
	this->m_hasColided = false;
	this->m_pyramidBoundingBox = nullptr;
	this->m_canCheckColision = false;
	this->m_currentTime = -1;
	this->m_audioComponent = new AudioComponent();

}

Platform::~Platform()
{
	if (this->m_audioComponent)
	{
		delete m_audioComponent;
		m_audioComponent = nullptr;
	}
}

void Platform::init(bool colidable, int modelIndex, int wvpCBufferIndex, DirectX::BoundingOrientedBox* pyramidBoundingBox, Model* mdl)
{
	this->initializeDynamic(colidable, false, modelIndex, wvpCBufferIndex, 3, DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), mdl);
	this->m_pyramidBoundingBox = pyramidBoundingBox;
}
void Platform::update(float dt)
{
	if (m_hasColided)
	{
		if (m_shouldDestroy)
		{
			this->m_physicsComp->addForceDir(Direction::DOWN, dt);
			this->m_physicsComp->updatePosition(dt);

			if(this->m_pyramidBoundingBox != nullptr)
				this->m_physicsComp->handleCollision(std::vector<DirectX::BoundingBox*>(), *m_pyramidBoundingBox, dt, std::vector<DirectX::BoundingOrientedBox*>());

			if ((this->m_destroyTime + this->m_removeTime) < this->m_destructionTimer.timeElapsed())
			{
				this->m_removeMe = true;
			}
		}
		else
		{
			if (this->m_destructionTimer.timeElapsed() >= m_destroyTime)
			{
				this->destroy();
				this->m_physicsComp->addGravity(dt);
			}
		}
		int time = (int)(this->m_destructionTimer.timeElapsed() * 2);
		if (m_currentTime != time)
		{
			if (time < m_nrOfTextures)
			{
				this->m_currentTime = time;
				this->m_texturePath = m_textures[time].c_str();
				m_audioComponent->emitSound(m_crackSounds[time % m_nrOfCracKSounds], this->m_movementComp->position);
			}
		}
	}
	else
	{
		this->colidesWithPlayer();
	}
	
}
void Platform::onPlayerColide()
{
	this->m_destructionTimer.start();
	this->m_hasColided = true;
}
void Platform::onDestroy()
{
	this->m_shouldDestroy = true;
}
void Platform::destroy()
{
	this->onDestroy();
}

void Platform::setPlayerBoundingBox(DirectX::BoundingBox* boundingBox)
{
	this->m_playerBoundingBox = boundingBox;
	this->m_platformTriggerBox = this->getAABB();
	this->m_platformTriggerBox.Center.y = this->m_platformTriggerBox.Center.y + 3;
	this->m_canCheckColision = true;
}

void Platform::initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
{
	this->m_audioComponent->init(audioEngine, playerMovementComponent);
	for (int i = 0; i < m_nrOfCracKSounds; i++)
	{
		this->m_audioComponent->loadSound(m_crackSounds[i]);
	}
}

void Platform::pushToLocation(XMVECTOR position)
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, position);
	this->setPosition(position);
	this->m_platformTriggerBox = this->getAABB();
	this->m_platformTriggerBox.Center.y += 3;
}

void Platform::colidesWithPlayer()
{
	if (this->m_playerBoundingBox == nullptr)
		return;
	if (!this->m_canCheckColision)
		return;
	if (this->m_platformTriggerBox.Intersects(*m_playerBoundingBox))
	{
		this->onPlayerColide();
	}
}