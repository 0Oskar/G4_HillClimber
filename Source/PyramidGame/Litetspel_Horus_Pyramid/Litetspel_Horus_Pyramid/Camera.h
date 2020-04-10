#pragma once

#include "MovementComponent.h"

class Camera
{
private:
	// Device
	ID3D11DeviceContext* m_deviceContext;

	// View and Projection
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_viewMatrix;

	// Comonents
	MovementComponent* m_movementComp;

	// Functions
	void setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ);
	void updateViewMatrix();

public:
	Camera();
	~Camera();

	// Initialization
	void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, float speed, float fovAngle, float aspectRatio, float nearZ, float farZ);
	
	// Update
	void update(DirectX::XMFLOAT2 mouseDelta);

	// Getters
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getViewMatrix() const;

	// Setters
	void followMoveComp(MovementComponent* moveComp);
	void setPosition(DirectX::XMVECTOR newPosition);
};