#pragma once

class Camera
{
private:
	// Device
	ID3D11DeviceContext* m_deviceContext;

	// Movement
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_rotation;
	float m_speed;

	// Direction Vectors
	DirectX::XMVECTOR m_forward;
	DirectX::XMVECTOR m_left;
	DirectX::XMVECTOR m_right;
	DirectX::XMVECTOR m_backward;

	// View and Projection
	DirectX::XMMATRIX m_projectionMatrix;
	DirectX::XMMATRIX m_viewMatrix;

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
	DirectX::XMVECTOR getPosition() const;
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getViewMatrix() const;

	// Setters
	void setPosition(DirectX::XMVECTOR newPosition);
	void move(DirectX::XMVECTOR moveDistance);
};