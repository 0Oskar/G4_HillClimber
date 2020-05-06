#include "pch.h"
#include "Player.h"

Player::Player() : GameObject()
{

	lastFly = false;

}

Player::~Player()
{
}

void Player::initialize(int modelIndex, int wvpCBufferIndex, float mass, DirectX::XMFLOAT3 acceleration, DirectX::XMFLOAT3 deceleration, GameObject* gObj, GameObject* hookGun, std::vector<GameObject*>* chainGObjects, std::shared_ptr<DirectX::AudioEngine> audioEngine)
{
	this->m_isStatic = true;
	this->m_collidable = true;
	this->m_modelIndex = modelIndex;
	this->m_wvpCBufferIndex = wvpCBufferIndex;

	this->m_movementComp = new MovementComponent();
	this->m_physicsComp = new PhysicsComponent();
	this->m_physicsComp->initialize(this->m_movementComp, mass, acceleration, deceleration);
	this->m_physicsComp->setBoundingBox(this->m_movementComp->getPositionF3(), DirectX::XMFLOAT3(1.f, 4.f, 1.f));
	this->m_hookHand.init(gObj, m_movementComp, &m_collidableAABBoxes, hookGun, chainGObjects, audioEngine);
}

void Player::addAABB(DirectX::BoundingBox* aabb)
{
	this->m_collidableAABBoxes.push_back(aabb);
}

void Player::addPyramidOBB(DirectX::BoundingOrientedBox* obb)
{
	this->m_pyramidOBB = *obb;
}

void Player::update(Keyboard* keyboard, Mouse* mouse, float dt)
{
	if (this->m_hookHand.shouldFly())
	{
		this->m_physicsComp->setVelocity(DirectX::XMFLOAT3(0, 0, 0));
		this->lastFly = true;
		DirectX::XMVECTOR normalziedAndDT = DirectX::XMVectorScale(DirectX::XMVector3Normalize(this->m_hookHand.m_toHeadDir), dt);
		normalziedAndDT = DirectX::XMVectorScale(normalziedAndDT, 100);
		this->m_movementComp->position = (DirectX::XMVectorAdd(this->m_movementComp->position, normalziedAndDT));
	}
	else if (lastFly)
	{
		this->m_physicsComp->setVelocity(DirectX::XMFLOAT3(0, 0, 0));
		this->lastFly = false;
	}
	else
	{
		// Gravity
		this->m_physicsComp->addGravity(dt);

		// Controls
		if (keyboard->isKeyPressed('W'))
			this->m_physicsComp->addForceDir(Direction::FORWARD, dt);

		if (keyboard->isKeyPressed('S'))
			this->m_physicsComp->addForceDir(Direction::BACKWARD, dt);

		if (keyboard->isKeyPressed('A'))
			this->m_physicsComp->addForceDir(Direction::LEFT, dt);

		if (keyboard->isKeyPressed('D'))
			this->m_physicsComp->addForceDir(Direction::RIGHT, dt);


	if (keyboard->isKeyPressed(' ')) {
		this->m_physicsComp->jump(3.f, dt);
		/*ImporterBFF::Manager* myManager = &ImporterBFF::Manager::GetInstance();
		ModelBFF myModel = myManager->LoadModel("biFile.bff");

		//std::string myString = "\n vtx:" + std::to_string(myModel.mesh.nrOfVertex) + "\n";
		for (int i = 0; i < myModel.mesh.nrOfVertex; i++)
		{
			std::string vtxNr = "\nvtx:		" + std::to_string(i) + "\n";

			std:: string vtxPos = "Pos:		" + (std::to_string(myModel.vertexArr[i].pos[0]) + " " +
				(std::to_string(myModel.vertexArr[i].pos[1])) + " " +
					(std::to_string(myModel.vertexArr[i].pos[2]))) + "\n";

			std::string uv = "uv:			" + (std::to_string(myModel.vertexArr[i].uv[0]) + " " +
				(std::to_string(myModel.vertexArr[i].uv[1]))) + "\n";

			std::string normal = "Normal:		" + (std::to_string(myModel.vertexArr[i].norm[0]) + " " +
				(std::to_string(myModel.vertexArr[i].norm[1])) + " " +
				(std::to_string(myModel.vertexArr[i].norm[2]))) + "\n";

			std::string biNormal = "Binormal:	" + (std::to_string(myModel.vertexArr[i].biNorm[0]) + " " +
				(std::to_string(myModel.vertexArr[i].biNorm[1])) + " " +
				(std::to_string(myModel.vertexArr[i].biNorm[2]))) + "\n";

			std::string tangent = "Tan:		" + (std::to_string(myModel.vertexArr[i].tan[0]) + " " +
				(std::to_string(myModel.vertexArr[i].tan[1])) + " " +
				(std::to_string(myModel.vertexArr[i].tan[2]))) + "\n";

			OutputDebugStringA((vtxNr + vtxPos + uv + normal + biNormal + tangent).c_str());
		}
		OutputDebugStringA(std::to_string(myModel.material.Diffuse[1]).c_str());*/ // <----------------------------------------------------------------------------
	}


		if (keyboard->isKeyPressed((unsigned char)16)) // Shift
			this->m_physicsComp->addForceDir(Direction::DOWN, dt);


		// For Debugging purposes
		if (keyboard->isKeyPressed('R'))
		{
			this->m_movementComp->position = DirectX::XMVectorSet(0.f, 6.f, -1.f, 1.f);
			this->m_physicsComp->setVelocity(DirectX::XMFLOAT3()); // Reset Velocity
		}

		// Handle Collisions
		this->m_physicsComp->handleCollision(this->m_collidableAABBoxes, this->m_pyramidOBB, dt);
	}

	if (keyboard->isKeyPressed('Q') || mouse->isRDown())
	{
		this->m_hookHand.retract();
	}
	if (keyboard->isKeyPressed('E') || mouse->isLDown())
	{
		this->m_hookHand.fire();
	}
	this->m_physicsComp->updatePosition(dt);
	this->m_hookHand.update(dt);


}