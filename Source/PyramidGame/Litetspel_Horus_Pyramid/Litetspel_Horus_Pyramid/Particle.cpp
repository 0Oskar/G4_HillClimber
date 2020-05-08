#include "Particle.h"


#define RandFloat ((float)(rand()%1001)/1000.0f)

DirectX::XMMATRIX IdentityMatrix;

//using namespace DirectX;

ParticleSystem::ParticleSystem(int numParts, DirectX::XMVECTOR startOrigin)
{
	this->NumParts = numParts;
	this->Particles = new OneParticle[this->NumParts];
	this->WorldspaceOrigin = startOrigin;

	//--
	extern HRESULT CreateDDSTextureFromFile(
		ID3D11Device * d3dDevice,
		const wchar_t* szFileName,
		ID3D11Resource * *texture,
		ID3D11ShaderResourceView * *textureView,
		size_t maxsize = 0,
		DDS_ALPHA_MODE * alphaMode = nullptr
	);
	//--

}

ParticleSystem::~ParticleSystem()
{
	if (this->Particles) {
		delete[] this->Particles;
		this->Particles = NULL;
	}
}

void ParticleSystem::InitializeParticles()
{
	for (int i = 0; 1 < this->NumParts; i++) {
		SetToDefault(i);
	}
}

// ----------------------------------------------------------------

ParticleFlame::ParticleFlame(ID3D11Device *device, int numparts, DirectX::XMVECTOR startOrigin, float height, char* texturefile) :
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
	this->Particles[i].Position = DirectX::XMVectorSet(0,0,0,0) + this->WorldspaceOrigin;
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

HRESULT ParticleFlame::Render(ID3D11Device *device)
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
