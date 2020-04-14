#include "pch.h"
#include "Camera.h"

void Camera::setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ)
{
	float fov = (fovAngle / 360.f) * DirectX::XM_2PI;
	this->m_projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);
}

void Camera::updateViewMatrix()
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(this->m_movementComp->rotation);
	// Look at
	DirectX::XMVECTOR lookAt = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 0.f, 1.f, 0.f), rotationMatrix);
	// Add position to lookAt
	lookAt = DirectX::XMVectorAdd(this->m_movementComp->position, lookAt);
	// Up
	DirectX::XMVECTOR up = XMVector3TransformCoord(DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f), rotationMatrix);

	// Update View Matrix with new Rotation
	this->m_viewMatrix = DirectX::XMMatrixLookAtLH(this->m_movementComp->position, lookAt, up);

	// Update Direction Vectors
	this->m_movementComp->updateDirVectors();
}

Camera::Camera()
{
	this->m_deviceContext = nullptr;

	this->m_projectionMatrix = DirectX::XMMatrixIdentity();
	this->m_viewMatrix = DirectX::XMMatrixIdentity();

	this->m_movementComp = nullptr;
}

Camera::~Camera() {}

void Camera::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float speed, float fovAngle, float aspectRatio, float nearZ, float farZ)
{
	this->m_deviceContext = deviceContext;
	this->m_movementComp->speed = speed;

	this->setProjectionMatrix(fovAngle, aspectRatio, nearZ, farZ);
	this->updateViewMatrix();

	// Components
	if (!this->m_movementComp)
		this->m_movementComp = new MovementComponent();
}

void Camera::update(DirectX::XMFLOAT2 mouseDelta)
{
	// Set Pitch
	DirectX::XMFLOAT3 rotationF3;
	DirectX::XMStoreFloat3(&rotationF3, this->m_movementComp->rotation);
	rotationF3.x += mouseDelta.y;

	// limit pitch to straight up or straight down
	// with a little fudge-factor to avoid gimbal lock
	float limit = DirectX::XM_PI / 2.0f - 0.01f;

	rotationF3.x = max(-limit, rotationF3.x);
	rotationF3.x = min(+limit, rotationF3.x);

	// Set Yaw
	rotationF3.y += mouseDelta.x;

	// keep longitude in sane range by wrapping
	if (rotationF3.x > DirectX::XM_PI)
		rotationF3.y -= DirectX::XM_PI * 2.0f;

	else if (rotationF3.x < -DirectX::XM_PI)
		rotationF3.y += DirectX::XM_PI * 2.0f;

	this->m_movementComp->rotation = DirectX::XMLoadFloat3(&rotationF3);

	this->updateViewMatrix();
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	return m_projectionMatrix;
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	return m_viewMatrix;
}

void Camera::followMoveComp(MovementComponent* moveComp)
{
	this->m_movementComp = moveComp;
}

void Camera::setPosition(DirectX::XMVECTOR newPosition)
{
	this->m_movementComp->position = newPosition;
	this->updateViewMatrix();
}

void Camera::move(Direction dir, float dt)
{
	this->m_movementComp->move(dir, dt);
	this->updateViewMatrix();
}

