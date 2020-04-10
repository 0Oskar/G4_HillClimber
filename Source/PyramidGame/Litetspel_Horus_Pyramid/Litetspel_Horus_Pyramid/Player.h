#pragma once

class Player
{
private:
	// Physics component
	DirectX::BoundingBox aabb;

	// Movement component
	float m_speed;
	DirectX::XMVECTOR m_position;
	DirectX::XMVECTOR m_rotation;

	// Direction Vectors
	DirectX::XMVECTOR m_forward;
	DirectX::XMVECTOR m_left;
	DirectX::XMVECTOR m_right;
	DirectX::XMVECTOR m_backward;

public:
	Player();
	~Player();

	// Getters
	DirectX::XMVECTOR getPosition() const;

	// Setters
	void setPosition(DirectX::XMVECTOR newPosition);
	void move(DirectX::XMVECTOR moveDistance);
};