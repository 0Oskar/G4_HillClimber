#pragma once

#include "MovementComponent.h"

class Camera
{
private:
	// View and Projection
	DirectX::XMMATRIX* m_projectionMatrix;
	DirectX::XMMATRIX* m_viewMatrix;

	// Comonents
	MovementComponent* m_movementComp;

	// Functions
	void setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ);
	void updateViewMatrix();

public:
	Camera();
	~Camera();

	// Getters
	DirectX::XMMATRIX* getProjectionMatrix() const;
	DirectX::XMMATRIX* getViewMatrix() const;

	// Setters
	void followMoveComp(MovementComponent* moveComp);
	void setPosition(DirectX::XMVECTOR newPosition);
	void move(Direction dir, float dt);

	// Initialization
	void initialize(float speed, float fovAngle, float aspectRatio, float nearZ, float farZ);
	
	// Update
	void update(DirectX::XMFLOAT2 mouseDelta);

};