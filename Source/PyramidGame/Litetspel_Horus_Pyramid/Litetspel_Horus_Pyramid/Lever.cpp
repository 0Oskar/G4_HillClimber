#include "Lever.h"

Lever::Lever()
{
	m_theTexture = L" ";
	m_sound = L" ";
	activated = false;
	canUseLever = false;
	//defaultLever L"Models/Lever.obj";
	m_audioComponent = new AudioComponent();
}

void Lever::init(bool colidable, int modelIndex, int wvpCBufferIndex, Model* mdl)
{
	this->initializeStatic(colidable, modelIndex, wvpCBufferIndex, mdl);
}

void Lever::onPlayerColide()
{
	canUseLever = true;
}

void Lever::activateLever()
{
	if (this->activated == false)
	{
		this->activated == true;
		canUseLever = false;
	}
	else if (this->activated == true)
	{
		this->activated == false;
		canUseLever = false;
	}
}

bool Lever::getLeverValue()
{
	return this-> activated;
}

void Lever::setPlayerBoundingBox(DirectX::BoundingBox* boundingBox)
{
	this->m_playerBoundingBox = boundingBox;
	this->leverTriggerBox = this->getAABB();
}

void Lever::collidesWithPlayer()
{
	if (this->leverTriggerBox.Intersects(*m_playerBoundingBox))
	{
		onPlayerColide();
	}
}

void Lever::initAudioComponent(std::shared_ptr<DirectX::AudioEngine> audioEngine, MovementComponent* playerMovementComponent)
{
	this->m_audioComponent->init(audioEngine, playerMovementComponent);
	this->m_audioComponent->loadSound(m_sound);
}
