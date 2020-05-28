#pragma once
#include "VertexBuffer.h"
#include "ParticleSystem.h"
#include "ConstantBuffer.h"
#include "Shaders.h"

class ParticleRenderer
{
private:
	Microsoft::WRL::ComPtr< ID3D11RasterizerState > NoCullPR;
	Microsoft::WRL::ComPtr < ID3D11BlendState > AlphaToCoveragePR;
	Microsoft::WRL::ComPtr < ID3D11BlendState > TransparentPR;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

	ConstBuffer<GS_PARTICLE_BUFFER> m_particleBuffer;

	//Shaders m_particleShader;
	Shaders m_streamOutShader;
	Shaders m_drawShader;

	ParticleSystem* m_particleSystem;

	VertexBuffer<Particle> m_initVB; 
	VertexBuffer<Particle> m_drawVB;
	VertexBuffer<Particle> m_streamOutVB;

	bool m_firstRun;

public:
	ParticleRenderer();
	~ParticleRenderer();

	void initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, ParticleSystem* particleSystem);
	void setNoCullPR();
	void setAlphaToCoveragePR();
	void setTransparentPR();
	
	void setParticleSystem(ParticleSystem* particleSystem);
	void render(XMMATRIX vpMatrix);
	



};
