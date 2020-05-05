#pragma once
#define D3D_OVERLOADS
#include "pch.h"
#include "../Litetspel_Horus_Pyramid/Shaders.h"

//---------------------------------------------------------------------------
//                          ParticleSystem
//---------------------------------------------------------------------------

class OneParticle
{
public:
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 LastFramePosition;
	DirectX::XMFLOAT3 Velocity;
	DirectX::XMFLOAT3 Acceleration;
	float Size;
	UINT ParticleLifetime;                  // Use DWORD "Unsigned long" if to short
	bool Alive;

};

//---------------------------------------------------------------------------
//                          ParticleSystem
//----------------------------------------------------------------------------

class ParticleSystem
{
protected:
	Vertex ParticleFace[4];
	OneParticle* Particles;
	int NumParts;

public:
	ParticleSystem(int numParts, DirectX::XMFLOAT3 startPos);
	virtual ~ParticleSystem();

	DirectX::XMFLOAT3 WorldspacePos;

	virtual void InitializeParticles();
	virtual void SetToDefault(int i) = 0;
	virtual void Update(UINT deltatime) = 0;
	virtual HRESULT Render(ID3D11Device device) = 0;


};

//---------------------------------------------------------------------------
//                          ParticleFlame
//----------------------------------------------------------------------------

class ParticleFlame : public ParticleSystem
{
protected:
	char TextureFile[MAX_PATH];

public:
	ParticleFlame(ID3D11Device device, int numparts, DirectX::XMFLOAT3 WorldspacePos, char* texturefile)
	virtual ~ParticleFlame();

	virtual void SetToDefault(int i);
	virtual void Update(UINT deltatime);
	virtual HRESULT Render(ID3D11Device device);
};