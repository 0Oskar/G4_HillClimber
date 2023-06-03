#include"pch.h"
#include"Platform.h"

Platform::Platform()
{
	m_shouldDestroy = false;
	m_playerBoundingBox = nullptr;
	m_hasColided = false;
	m_pyramidBoundingBox = nullptr;
	m_canCheckColision = false;
	m_currentTime = -1;
	m_audioComponent = new AudioComponent();

}

Platform::~Platform()
{
	if (m_audioComponent)
	{
		delete m_audioComponent;
		m_audioComponent = nullptr;
	}
}

void Platform::init(bool colidable, int wvpCBufferIndex, DirectX::BoundingOrientedBox* pyramidBoundingBox, Model* mdl)
{
	initializeDynamic(colidable, false, wvpCBufferIndex, 3, DirectX::XMFLOAT3(1, 1, 1), DirectX::XMFLOAT3(0, 0, 0), mdl);
	m_pyramidBoundingBox = pyramidBoundingBox;
}
void Platform::update(float dt)
{
	if (m_hasColided)
	{
		if (m_shouldDestroy)
		{
			m_physicsComp->addForceDir(Direction::DOWN, dt);
			m_physicsComp->updatePosition(dt);

			if(m_pyramidBoundingBox != nullptr)
				m_physicsComp->handleCollision(std::vector<DirectX::BoundingBox*>(), *m_pyramidBoundingBox, dt, std::vector<DirectX::BoundingOrientedBox*>());

			if ((m_destroyTime + m_removeTime) < m_destructionTimer.timeElapsed())
			{
				m_removeMe = true;
			}
		}
		else
		{
			if (m_destructionTimer.timeElapsed() >= m_destroyTime)
			{
				destroy();
				m_physicsComp->addGravity(dt);
			}
		}
		int time = (int)(m_destructionTimer.timeElapsed() * 2);
		if (m_currentTime != time)
		{
			if (time < m_nrOfTextures)
			{
				m_currentTime = time;
				m_texturePath = m_textures[time].c_str();
				m_audioComponent->emitSound(m_crackSounds[time % m_nrOfCracKSounds], m_movementComp->position, 0, false, 5.f);
			}
		}
	}
	else
	{
		colidesWithPlayer();
	}
	
}
void Platform::onPlayerColide()
{
	m_destructionTimer.start();
	m_hasColided = true;
}
void Platform::onDestroy()
{
	m_shouldDestroy = true;
}
void Platform::destroy()
{
	onDestroy();
}

void Platform::setPlayerBoundingBox(DirectX::BoundingBox* boundingBox)
{
	m_playerBoundingBox = boundingBox;
	m_platformTriggerBox = getAABB();
	m_platformTriggerBox.Center.y = m_platformTriggerBox.Center.y + 3;
	m_canCheckColision = true;
}

void Platform::initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
{
	m_audioComponent->init(audioEngine, playerMovementComponent);
	for (int i = 0; i < m_nrOfCracKSounds; i++)
	{
		m_audioComponent->loadSound(m_crackSounds[i]);
	}
}

void Platform::pushToLocation(XMVECTOR position)
{
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, position);
	setPosition(position);
	m_platformTriggerBox = getAABB();
	m_platformTriggerBox.Center.y += 3;
}

void Platform::colidesWithPlayer()
{
	if (m_playerBoundingBox == nullptr)
		return;
	if (!m_canCheckColision)
		return;
	if (m_platformTriggerBox.Intersects(*m_playerBoundingBox))
	{
		onPlayerColide();
	}
}