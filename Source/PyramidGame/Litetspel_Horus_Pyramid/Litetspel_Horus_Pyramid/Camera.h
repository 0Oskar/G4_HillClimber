#pragma once

#include "MovementComponent.h"
#include "Input.h"

class Camera
{
private:
	// View and Projection
	DirectX::XMMATRIX* m_projectionMatrix;
	float m_mouseSense;
	float m_fov;
	float m_aspectRatio;
	float m_nearZ;
	float m_farZ;

	// Comonents
	MovementComponent* m_movementComp;

	// Functions
	void setProjectionMatrix(float fovAngle, float aspectRatio, float nearZ, float farZ);

public:
	Camera();
	Camera(Camera &camera);
	~Camera();

	// Getters
	DirectX::XMMATRIX* getProjectionMatrixPtr() const;
	DirectX::XMMATRIX* getViewMatrixPtr() const;
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getViewMatrix() const;
	float getFov() const;
	float getAspectRatio() const;
	float getNearZ() const;
	float getFarZ() const;
	XMFLOAT3 getPosition() const;
	XMVECTOR getPositionVector() const;
	XMFLOAT3 getRotation() const;
	XMVECTOR getRotationVector() const;

	// Setters
	void followMoveComp(MovementComponent* moveComp);
	void setPosition(DirectX::XMVECTOR newPosition);

	// Initialization
	void initialize(float mouseSense, float fovAngle, float aspectRatio, float nearZ, float farZ);

	// Update
	void updateView();
	void update(MouseEvent mouseEvent, float dt);
};