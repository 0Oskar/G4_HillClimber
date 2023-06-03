#include "pch.h"
#include "Lever.h"


Lever::Lever()
{
	m_theTexture = L" ";
	m_sound = L" ";
	activated = true;
	canUseLever = false;
	//defaultLever L"Models/Lever.obj";
	m_audioComponent = new AudioComponent();
}

Lever::~Lever()
{
	if (m_audioComponent != nullptr)
	{
		delete m_audioComponent;
		m_audioComponent = nullptr;
	}
}

void Lever::init(bool colidable, int wvpCBufferIndex, Model* mdl)
{
	initializeStatic(colidable, wvpCBufferIndex, mdl);
}

void Lever::onPlayerColide()
{
	canUseLever = true;
}

void Lever::toggleActivateLever()
{
	if (activated == false)
	{
		activated = true;
		canUseLever = false;
	}
	else if (activated == true)
	{
		activated = false;
		canUseLever = false;
	}
}

bool Lever::getLeverValue()
{
	return  activated;
}

bool Lever::getCanUseLever()
{
	if (activated == false)
	{
		return false;
	}
	return canUseLever;
}

void Lever::setPlayerBoundingBox(DirectX::BoundingBox* boundingBox)
{
	m_playerBoundingBox = boundingBox;
	leverTriggerBox = getAABB();
}

void Lever::setCnaUseLever(bool state)
{
	canUseLever = state;
}

void Lever::collidesWithPlayer()
{
	if (leverTriggerBox.Intersects(*m_playerBoundingBox))
	{
		onPlayerColide();
	}
	else
	{
		canUseLever = false;
	}
}

void Lever::initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
{
	m_audioComponent->init(audioEngine, playerMovementComponent);
	m_audioComponent->loadSound(m_sound);
}
