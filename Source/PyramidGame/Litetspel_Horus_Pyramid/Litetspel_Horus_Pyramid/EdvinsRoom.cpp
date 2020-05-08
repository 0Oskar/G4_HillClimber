#include "pch.h"
#include "EdvinsRoom.h"

void EdvinsRoom::createBoundingBoxes()
{
}

void EdvinsRoom::createSceneObjects()
{
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(0.f, -10.f, 0.f, 1.f);

	//Lever
	//vec = DirectX::XMVectorSet(-15.f, 5.f, -88.f + 140.f, 1.f);
	//rotation = DirectX::XMVectorSet(0.f, pMath::convertDegreesToRadians(-90), pMath::convertDegreesToRadians(-270), 1.f);
	//this->addLeverToRoom(7, &m_models->at(7), vec, rotation, DirectX::XMFLOAT3(2.f, 2.f, 2.f));

	//lever.emplace_back(dynamic_cast<Lever*>(this->m_gameObjects.back()));


}

void EdvinsRoom::onCompleted()
{
}

EdvinsRoom::EdvinsRoom()
{
}

EdvinsRoom::~EdvinsRoom()
{
}

void EdvinsRoom::update(float dt, Camera* camera, Room*& activeRoom, bool& activeRoomChanged)
{
}

void EdvinsRoom::onEntrance()
{
}

void EdvinsRoom::init()
{
}

void EdvinsRoom::portals()
{
}
