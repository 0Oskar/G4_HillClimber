#include "pch.h"
#include "ParticleSystem.h"

DirectX::XMMATRIX IdentityMatrix;

//using namespace DirectX;

void ParticleSystem::CreateRandomTex(ID3D11Device* device)
{
		srand(time(NULL));

		XMFLOAT4 randomValues[1024];

		for (int i = 0; i < 1024; ++i)
		{
			randomValues[i].x = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - (-1.0f))));
			randomValues[i].y = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - (-1.0f))));
			randomValues[i].z = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - (-1.0f))));
			randomValues[i].w = -1.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - (-1.0f))));
		}

		D3D11_SUBRESOURCE_DATA initData;
		initData.pSysMem = randomValues;
		initData.SysMemPitch = 1024 * sizeof(XMFLOAT4);
		initData.SysMemSlicePitch = 0;

		D3D11_TEXTURE1D_DESC texDesc;
		texDesc.Width = 1024;
		texDesc.MipLevels = 1;
		texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		texDesc.Usage = D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		texDesc.ArraySize = 1;

		ID3D11Texture1D *randomTex;
		HRESULT hr = device->CreateTexture1D(&texDesc, &initData, &randomTex);
		assert(SUCCEEDED(hr) && "Error, failed to create randomTex!");

		D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
		viewDesc.Format = texDesc.Format;
		viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
		viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
		viewDesc.Texture1D.MostDetailedMip = 0;

		hr = device->CreateShaderResourceView(randomTex, &viewDesc, &this->m_RandomTex);
		assert(SUCCEEDED(hr) && "Error, failed to create randomTex SRV!");

		randomTex->Release();
}

ParticleSystem::ParticleSystem()
{
	this->m_partBuffer.GameTime = 0.0f;
	this->m_partBuffer.TimeStep = 0.0f;
	this->m_partBuffer.CamPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->m_partBuffer.EmitPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->m_partBuffer.EmitDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);

	this->m_ParticleLifetime = 0.0f;
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::initialize(ID3D11Device* device, ID3D11ShaderResourceView* texArray, UINT maxParticles, XMFLOAT3 emitPos, XMFLOAT3 emitDir)
{
	this->m_MaxParticles = maxParticles;

	this->m_TexArray = texArray;
	this->CreateRandomTex(device);

	this->m_partBuffer.EmitPosition = emitPos;
	this->m_partBuffer.EmitDirection = emitDir;

	// Create Disabled Depth Stencil State
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS; //D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = FALSE;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	//// Stencil operations if pixel is front-facing
	//dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	//dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//// Stencil operations if pixel is back-facing
	//dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	//dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HRESULT hr = device->CreateDepthStencilState(&dsDesc, &this->m_disabledDepthState);
	assert(SUCCEEDED(hr) && "Error, failed to create disabled depth stencil state!");

}

void ParticleSystem::setToDefault(int i)
{
}

void ParticleSystem::setVPMatrix(XMMATRIX VPMatrix)
{
	this->m_partBuffer.ViewProjMatrix = VPMatrix;
}

GS_PARTICLE_BUFFER* ParticleSystem::getParticleBuffer()
{
	return &this->m_partBuffer;
}

ID3D11DepthStencilState* ParticleSystem::getDisabledDepthState()
{
	return this->m_disabledDepthState.Get();
}

void ParticleSystem::update(float deltaTime, float gameTime, XMVECTOR camPos)
{
	this->m_partBuffer.GameTime = gameTime;
	this->m_partBuffer.TimeStep = deltaTime;

	this->m_ParticleLifetime += deltaTime;

	XMStoreFloat3(&this->m_partBuffer.CamPosition, camPos);
}

// ----------------------------------------------------------------
//
//ParticleFlame::ParticleFlame(ID3D11Device* device, int numparts, DirectX::XMVECTOR startOrigin, float height, char* texturefile) :
//	ParticleSystem(numparts, startOrigin)
//{
//	this->Height = height;
//	strcpy(this->TextureFile, texturefile);
//	// Create texture from file (this->TextureFile)
//	// Texture restore (this->TextureFile, device)
//
//}
//
//ParticleFlame::~ParticleFlame()
//{
//	if (this->Particles) {
//		delete[] this->Particles;
//		this->Particles = NULL;
//	}
//	// Destroy (this->TextureFile);
//}
//
//void ParticleFlame::SetToDefault(int i)
//{
//	// Position particle at origin, let it slowly move up:
//	this->Particles[i].Position = DirectX::XMVectorSet(0, 0, 0, 0) + this->WorldspaceOrigin;
//	this->Particles[i].Velocity = DirectX::XMVectorSet(RandFloat * 0.0005f - 0.00025f, 0.002f + RandFloat * 0.002f, RandFloat * 0.0005f - 0.00025f, 0.0f);
//}
//
//void ParticleFlame::Update(UINT deltatime)
//{
//	for (int i = 0; 1 < this->NumParts; i++) {
//		this->Particles[i].Position += this->Particles[i].Velocity * deltatime;
//
//		//if (Particles[i].Position.y > SystemOrigin.y + Height) SetParticleDefaults(i);
//		//Particles[i].Size = 2.5f / Height * (Particles[i].Position.y - SystemOrigin.y) + 0.5f;
//		// make particle larger with time
//	}
//}
//
//HRESULT ParticleFlame::Render(ID3D11Device* device)
//{
//	HRESULT hr;
//
//	//device->();
//
//	DirectX::XMMATRIX matViewBak;
//
//
//	DirectX::XMVECTOR TransPos;
//	unsigned char Alpha;
//	for (int i = 0; 1 < this->NumParts; i++) {
//
//		// Calculate transparency: (particles dissolve with time)
//		//Alpha = 255 - (int)(255.0f / Height * (Particles[i].Position - this->WorldspaceOrigin));
//		//ParticleFace[0] = Vertex(TransPos + DirectX::XMVectorSet(-1.0f * Particles[i].Size, 1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 0.0f, 0.0f);
//		//ParticleFace[1] = Vertex(TransPos + DirectX::XMVectorSet(1.0f * Particles[i].Size, 1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 1.0f, 0.0f);
//		//ParticleFace[2] = Vertex(TransPos + DirectX::XMVectorSet(-1.0f * Particles[i].Size, -1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 0.0f, 1.0f);
//		//ParticleFace[3] = Vertex(TransPos + DirectX::XMVectorSet(1.0f * Particles[i].Size, -1.0f * Particles[i].Size, 0.0f), DirectX::XMVectorSet(Alpha, Alpha, Alpha, 1), 0, 1.0f, 1.0f);
//
//
//		//device->Draw   draw TRIANGLESTRIP using ParticleFace
//	}
//
//	return hr;
//}
