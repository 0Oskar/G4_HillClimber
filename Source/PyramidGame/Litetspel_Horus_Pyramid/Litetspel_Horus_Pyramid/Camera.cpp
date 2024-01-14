#include "pch.h"
#include "Camera.h"

void Camera::setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ)
{
	m_fov = (fovAngle / 360.f) * DirectX::XM_2PI;
	m_aspectRatio = aspectRatio;
	m_nearZ = nearZ;
	m_farZ = farZ;
	*m_projectionMatrix = XMMatrixPerspectiveFovLH(m_fov, m_aspectRatio, m_nearZ, m_farZ);
}

Camera::Camera()
{
	m_projectionMatrix = new DirectX::XMMATRIX(DirectX::XMMatrixIdentity());
	m_mouseSense = 0;
	m_movementComp = nullptr;
}

Camera::Camera(Camera& camera)
{
	m_mouseSense = camera.m_mouseSense;
	m_projectionMatrix = new DirectX::XMMATRIX(DirectX::XMMatrixIdentity());
	setProjectionMatrix(camera.m_fov, camera.m_aspectRatio, camera.m_nearZ, camera.m_farZ);

	m_movementComp = new MovementComponent(*camera.m_movementComp);
	m_movementComp->updateViewMatrix();
}

Camera::~Camera()
{
	delete m_projectionMatrix;
}

void Camera::initialize(float mouseSense, float fovAngle, float aspectRatio, float nearZ, float farZ)
{
	m_mouseSense = mouseSense / 10.f;

	setProjectionMatrix(fovAngle, aspectRatio, nearZ, farZ);
	// Components
	if (!m_movementComp)
		m_movementComp = new MovementComponent();

	m_movementComp->updateViewMatrix();

}

void Camera::updateView()
{
	m_movementComp->updateViewMatrix();
}

void Camera::update(MouseEvent mouseEvent, float dt)
{
	// Rotate Camera
	if (mouseEvent.getEvent() == Event::MouseRAW_MOVE)
	{
		DirectX::XMFLOAT2 mouseDelta = DirectX::XMFLOAT2((float)mouseEvent.getPosX(), (float)mouseEvent.getPosY());

		// Set Pitch
		DirectX::XMFLOAT3 rotationF3;
		DirectX::XMStoreFloat3(&rotationF3, m_movementComp->rotation);
		rotationF3.x += mouseDelta.y * m_mouseSense;

		// Limit pitch to straight up or straight down with a little fudge-factor to avoid gimbal lock
		float limit = DirectX::XM_PI / 2.0f - 0.01f;
		rotationF3.x = max(-limit, rotationF3.x);
		rotationF3.x = min(+limit, rotationF3.x);

		// Set Yaw
		rotationF3.y += mouseDelta.x * m_mouseSense;;

		// Keep longitude in sane range by wrapping
		if (rotationF3.x > DirectX::XM_PI)
			rotationF3.y -= DirectX::XM_PI * 2.0f;
		else if (rotationF3.x < -DirectX::XM_PI)
			rotationF3.y += DirectX::XM_PI * 2.0f;

		// Update Movement Component
		m_movementComp->rotation = DirectX::XMLoadFloat3(&rotationF3);
		m_movementComp->updateViewMatrix();
	}
}

DirectX::XMMATRIX* Camera::getProjectionMatrixPtr() const
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX* Camera::getViewMatrixPtr() const
{
	return m_movementComp->viewMatrix;
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	return *m_projectionMatrix;
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	return *m_movementComp->viewMatrix;
}

float Camera::getFov() const
{
	return m_fov;
}

float Camera::getAspectRatio() const
{
	return m_aspectRatio;
}

float Camera::getNearZ() const
{
	return m_nearZ;
}

float Camera::getFarZ() const
{
	return m_farZ;
}

XMFLOAT3 Camera::getPosition() const
{
	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, m_movementComp->position);
	return  camPos;
}

XMVECTOR Camera::getPositionVector() const
{
	return m_movementComp->position;
}

XMFLOAT3 Camera::getRotation() const
{
	XMFLOAT3 camRotation;
	XMStoreFloat3(&camRotation, m_movementComp->rotation);
	return  camRotation;
}

XMVECTOR Camera::getRotationVector() const
{
	return m_movementComp->rotation;
}

void Camera::followMoveComp(MovementComponent* moveComp)
{
	m_movementComp = moveComp;
}

void Camera::setPosition(DirectX::XMVECTOR newPosition)
{
	m_movementComp->position = newPosition;
	m_movementComp->updateViewMatrix();
}