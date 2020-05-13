#include "pch.h"
#include "ParticleSystem.h"


#define RandFloat ((float)(rand()%1001)/1000.0f)

DirectX::XMMATRIX IdentityMatrix;

//using namespace DirectX;

ParticleSystem::ParticleSystem(int numParts, DirectX::XMVECTOR startOrigin)
{
	this->m_GameTime = 0.0f;
	this->m_TimeStep = 0.0f;
	this->m_ParticleLifetime = 0.0f;

	this->m_CamPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->m_EmitPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->m_EmitDirection = XMFLOAT3(0.0f, 1.0f, 0.0f);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::initialize(ID3D11Device* device, ID3D11ShaderResourceView* texArray, ID3D11ShaderResourceView* randomTex, UINT maxParticles)
{
	this->m_MaxParticles = maxParticles;

	this->m_TexArray = texArray;
	this->m_RandomTex = randomTex;

}

void ParticleSystem::setToDefault(int i)
{
}

void ParticleSystem::update(float deltaTime, float gameTime, XMVECTOR camPos, XMFLOAT3 emitPos, XMFLOAT3 emitDir)
{
	this->m_partBuffer.GameTime = gameTime;
	this->m_partBuffer.TimeStep = deltaTime;

	this->m_ParticleLifetime += deltaTime;

	XMStoreFloat3(&this->m_partBuffer.CamPosition, camPos);
	
	this->m_partBuffer.EmitPosition = emitPos;
	this->m_partBuffer.EmitDirection = emitDir;
	
}

void ParticleSystem::render(ID3D11DeviceContext* device, XMMATRIX vpMatrix)
{

}

// ----------------------------------------------------------------

ParticleFlame::ParticleFlame(ID3D11Device* device, int numparts, DirectX::XMVECTOR startOrigin, float height, char* texturefile) :
	ParticleSystem(numparts, startOrigin)
{
	this->Height = height;
	strcpy(this->TextureFile, texturefile);
	// Create texture from file (this->TextureFile)
	// Texture restore (this->TextureFile, device)

}

ParticleFlame::~ParticleFlame()
{
	if (this->Particles) {
		delete[] this->Particles;
		this->Particles = NULL;
	}
	// Destroy (this->TextureFile);
}

void ParticleFlame::SetToDefault(int i)
{
	// Position particle at origin, let it slowly move up:
	this->Particles[i].Position = DirectX::XMVectorSet(0, 0, 0, 0) + this->WorldspaceOrigin;
	this->Particles[i].Velocity = DirectX::XMVectorSet(RandFloat * 0.0005f - 0.00025f, 0.002f + RandFloat * 0.002f, RandFloat * 0.0005f - 0.00025f, 0.0f);
}

void ParticleFlame::Update(UINT deltatime)
{
	for (int i = 0; 1 < this->NumParts; i++) {
		this->Particles[i].Position += this->Particles[i].Velocity * deltatime;

		//if (Particles[i].Position.y > SystemOrigin.y + Height) SetParticleDefaults(i);
		//Particles[i].Size = 2.5f / Height * (Particles[i].Position.y - SystemOrigin.y) + 0.5f;
		// make particle larger with time
	}
}

HRESULT ParticleFlame::Render(ID3D11Device* device)
{
	HRESULT hr;

	//device->();

	DirectX::XMMATRIX matViewBak;


	DirectX::XMVECTOR TransPos;
	unsigned char Alpha;
	for (int i = 0; 1 < this->NumParts; i++) {

		// Calculate transparency: (particles dissolve with time)
		//Alpha = 255 - (int)(255.0f / Height * (Particles[i].Position - this->WorldspaceOrigin));
		//ParticleFace[0] = Vertex(TransPos + DirectX::XMVectorSet(-1.0f * Particles[i].Size, 1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 0.0f, 0.0f);
		//ParticleFace[1] = Vertex(TransPos + DirectX::XMVectorSet(1.0f * Particles[i].Size, 1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 1.0f, 0.0f);
		//ParticleFace[2] = Vertex(TransPos + DirectX::XMVectorSet(-1.0f * Particles[i].Size, -1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 0.0f, 1.0f);
		//ParticleFace[3] = Vertex(TransPos + DirectX::XMVectorSet(1.0f * Particles[i].Size, -1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 1.0f, 1.0f);


		//device->Draw   draw TRIANGLESTRIP using ParticleFace
	}

	return hr;
}
