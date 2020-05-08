#pragma once
#define D3D_OVERLOADS
#include "pch.h"
#include "../Litetspel_Horus_Pyramid/Shaders.h"


struct Particle {
	XMFLOAT3 Position;
	XMFLOAT3 Velocity;
	float Size;
	float ParticleLifetime;
	UINT Type;
};

struct GS_PARTICLE_BUFFER {
	XMFLOAT3 CamPosition;

	XMFLOAT3 EmitPosition;
	XMFLOAT3 EmitDirection;

	float GameTime;
	float TimeStep;
	XMMATRIX ViewProjMatrix;

};



//---------------------------------------------------------------------------
//                          Particle
//---------------------------------------------------------------------------

class OneParticle
{
public:

	//DirectX::XMVECTOR name = DirectX::XMVectorSet(1.5f, -0.7f, 2.3f, 0.f);
	DirectX::XMVECTOR Position;
	DirectX::XMVECTOR LastFramePosition;
	DirectX::XMVECTOR Velocity;
	DirectX::XMVECTOR Acceleration;
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
	OneParticle *Particles;
	int NumParts;

public:
	ParticleSystem(int numParts, DirectX::XMVECTOR startOrigin);
	virtual ~ParticleSystem();

	DirectX::XMVECTOR WorldspaceOrigin;

	virtual void InitializeParticles();
	virtual void SetToDefault(int i) = 0;
	virtual void Update(UINT deltatime) = 0;
	virtual HRESULT Render(ID3D11Device *device) = 0;


};

//---------------------------------------------------------------------------
//                          ParticleFlame
//----------------------------------------------------------------------------

class ParticleFlame : public ParticleSystem
{
protected:
	char TextureFile[MAX_PATH];

public:
	ParticleFlame(ID3D11Device *device, int numparts, DirectX::XMVECTOR startOrigin, float height, char* texturefile);
	virtual ~ParticleFlame();

	float Height;

	virtual void SetToDefault(int i);
	virtual void Update(UINT deltatime);
	virtual HRESULT Render(ID3D11Device *device);
};