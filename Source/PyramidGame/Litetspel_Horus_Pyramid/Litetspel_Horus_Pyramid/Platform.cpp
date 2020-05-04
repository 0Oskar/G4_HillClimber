#include"pch.h"
#include"Platform.h"

Platform::Platform()
{
	this->m_shouldDestroy = false;
	this->m_playerBoundingBox = nullptr;
	this->m_hasColided = false;
	this->m_pyramidBoundingBox = nullptr;
	this->canCheckColision = false;
	this->currentTime = -1;
	this->m_audioComponent = new AudioComponent();

}

void Platform::init(bool colidable, int modelIndex, int wvpCBufferIndex, DirectX::BoundingOrientedBox* pyramidBoundingBox, Model* mdl)
{
	this->initializeDynamic(colidable, true, modelIndex, wvpCBufferIndex, 3, DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), mdl);
	this->m_pyramidBoundingBox = pyramidBoundingBox;
}
void Platform::update(float dt)
{
	if (m_hasColided)
	{
		OutputDebugString(L"PLATFORM");
		if (m_shouldDestroy)
		{
			this->m_physicsComp->updatePosition(dt);
			this->m_physicsComp->handleCollision(std::vector<DirectX::BoundingBox*>(), *m_pyramidBoundingBox, dt);

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
		int time = (int)this->m_destructionTimer.timeElapsed();
		if (currentTime != time)
		{
			if (time < m_nrOfTextures)
			{
				this->currentTime = time;
				this->m_texturePath = m_textures[time].c_str();
				m_audioComponent->emitSound(m_crackSounds[time % nrOfCracKSounds], this->m_movementComp->position);
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
	this->platformTriggerBox = this->getAABB();
	this->platformTriggerBox.Center.y = this->platformTriggerBox.Center.y + 3;
	this->canCheckColision = true;
}

void Platform::initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
{
	this->m_audioComponent->init(audioEngine, playerMovementComponent);
	for (int i = 0; i < nrOfCracKSounds; i++)
	{
		this->m_audioComponent->loadSound(m_crackSounds[i]);
	}
}

void Platform::colidesWithPlayer()
{
	if (this->m_playerBoundingBox == nullptr)
		return;
	if (!this->canCheckColision)
		return;
	if (this->platformTriggerBox.Intersects(*m_playerBoundingBox))
	{
		this->onPlayerColide();
	}
}