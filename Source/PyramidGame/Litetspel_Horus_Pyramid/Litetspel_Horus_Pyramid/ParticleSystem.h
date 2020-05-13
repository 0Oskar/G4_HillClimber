#pragma once
#define D3D_OVERLOADS
#include "Shaders.h"

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
//                          ParticleSystem
//----------------------------------------------------------------------------

class ParticleSystem
{
protected:
	//Vertex ParticleFace[4];
	//Particle* Particles;
	//int NumParts;


private:

	GS_PARTICLE_BUFFER m_partBuffer;
	UINT m_MaxParticles;

	float m_ParticleLifetime;

	ID3D11ShaderResourceView* m_TexArray;
	ID3D11ShaderResourceView* m_RandomTex;


public:
	ParticleSystem(int numParts, DirectX::XMVECTOR startOrigin);
	virtual ~ParticleSystem();

	void initialize(ID3D11Device* device, ID3D11ShaderResourceView* texArray, ID3D11ShaderResourceView* randomTex, UINT maxParticles);
	void setToDefault(int i);
	void update(float deltaTime, float gameTime, XMVECTOR camPos, XMFLOAT3 emitPos, XMFLOAT3 emitDir);
	void render(ID3D11DeviceContext* device, XMMATRIX vpMatrix);


};

//---------------------------------------------------------------------------
//                          ParticleFlame
//----------------------------------------------------------------------------

class ParticleFlame : public ParticleSystem
{
protected:
	char TextureFile[MAX_PATH];

public:
	ParticleFlame(ID3D11Device* device, int numparts, DirectX::XMVECTOR startOrigin, float height, char* texturefile);
	virtual ~ParticleFlame();

	float Height;

	virtual void SetToDefault(int i);
	virtual void Update(UINT deltatime);
	virtual HRESULT Render(ID3D11Device* device);
};
