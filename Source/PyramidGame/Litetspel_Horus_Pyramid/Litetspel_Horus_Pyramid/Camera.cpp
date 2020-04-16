#include "pch.h"
#include "Camera.h"

void Camera::setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ)
{
	float fov = (fovAngle / 360.f) * DirectX::XM_2PI;
	*m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
}

Camera::Camera()
{
	this->m_projectionMatrix = new DirectX::XMMATRIX(DirectX::XMMatrixIdentity());
	this->m_mouseSense = 0;
	this->m_movementComp = nullptr;
}

Camera::~Camera()
{
	delete this->m_projectionMatrix;
}

void Camera::initialize(float mouseSense, float fovAngle, float aspectRatio, float nearZ, float farZ)
{
	this->m_mouseSense = mouseSense / 10.f;

	this->setProjectionMatrix(fovAngle, aspectRatio, nearZ, farZ);
	this->m_movementComp->updateViewMatrix();

	// Components
	if (!this->m_movementComp)
		this->m_movementComp = new MovementComponent();
}

void Camera::update(MouseEvent mouseEvent, float dt)
{
	// Rotate Camera
	if (mouseEvent.getEvent() == Event::MouseRAW_MOVE)
	{

		DirectX::XMFLOAT2 mouseDelta = DirectX::XMFLOAT2((float)mouseEvent.getPosX() * dt, (float)mouseEvent.getPosY() * dt);

		// Set Pitch
		DirectX::XMFLOAT3 rotationF3;
		DirectX::XMStoreFloat3(&rotationF3, this->m_movementComp->rotation);
		rotationF3.x += mouseDelta.y * this->m_mouseSense;

		// Limit pitch to straight up or straight down with a little fudge-factor to avoid gimbal lock
		float limit = DirectX::XM_PI / 2.0f - 0.01f;
		rotationF3.x = max(-limit, rotationF3.x);
		rotationF3.x = min(+limit, rotationF3.x);

		// Set Yaw
		rotationF3.y += mouseDelta.x * this->m_mouseSense;;

		// Keep longitude in sane range by wrapping
		if (rotationF3.x > DirectX::XM_PI)
			rotationF3.y -= DirectX::XM_PI * 2.0f;
		else if (rotationF3.x < -DirectX::XM_PI)
			rotationF3.y += DirectX::XM_PI * 2.0f;

		// Update Movement Component
		this->m_movementComp->rotation = DirectX::XMLoadFloat3(&rotationF3);
		this->m_movementComp->updateViewMatrix();
	}
}

DirectX::XMMATRIX* Camera::getProjectionMatrixPtr() const
{
	return this->m_projectionMatrix;
}

DirectX::XMMATRIX* Camera::getViewMatrixPtr() const
{
	return this->m_movementComp->viewMatrix;
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	return *this->m_projectionMatrix;
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	return *this->m_movementComp->viewMatrix;
}

void Camera::followMoveComp(MovementComponent* moveComp)
{
	this->m_movementComp = moveComp;
}

void Camera::setPosition(DirectX::XMVECTOR newPosition)
{
	this->m_movementComp->position = newPosition;
	this->m_movementComp->updateViewMatrix();
}