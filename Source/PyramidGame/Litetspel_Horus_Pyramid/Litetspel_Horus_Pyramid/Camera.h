#pragma once

#include "MovementComponent.h"
#include "Input.h"

class Camera
{
private:
	// View and Projection
	DirectX::XMMATRIX* m_projectionMatrix;
	float m_mouseSense;

	// Comonents
	MovementComponent* m_movementComp;

	// Functions
	void setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ);

public:
	Camera();
	~Camera();

	// Getters
	DirectX::XMMATRIX* getProjectionMatrixPtr() const;
	DirectX::XMMATRIX* getViewMatrixPtr() const;
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getViewMatrix() const;

	// Setters
	void followMoveComp(MovementComponent* moveComp);
	void setPosition(DirectX::XMVECTOR newPosition);

	// Initialization
	void initialize(float mouseSense, float fovAngle, float aspectRatio, float nearZ, float farZ);

	// Update
	void updateView();
	void update(MouseEvent mouseEvent, float dt);
};