#include "pch.h"
#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer()
{
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext, ParticleSystem* particleSystem)
{
	this->m_device = device;
	this->m_dContext = dContext;
	this->m_particleSystem = particleSystem;
	this->m_particleBuffer.init(device, dContext);
	this->m_firstRun = true;

	// Shaders Out
	ShaderFiles shadersOut;
	shadersOut.vs = L"Shader Files\\StreamOutParticleVS.hlsl";
	shadersOut.gs = L"Shader Files\\StreamOutParticleGS.hlsl";
	this->m_streamOutShader.initialize(device, dContext, shadersOut, LayoutType::PARTICLE, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, true);
	// Shader draw
	ShaderFiles shadersDraw;
	shadersDraw.vs = L"Shader Files\\DrawParticleVS.hlsl";
	shadersDraw.gs = L"Shader Files\\DrawParticleGS.hlsl";
	shadersDraw.ps = L"Shader Files\\DrawParticlePS.hlsl";
	this->m_drawShader.initialize(device, dContext, shadersDraw, LayoutType::PARTICLE, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);


	// No Cull
	D3D11_RASTERIZER_DESC NoCullDesc;
	ZeroMemory(&NoCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	NoCullDesc.FillMode = D3D11_FILL_SOLID;
	NoCullDesc.CullMode = D3D11_CULL_NONE;
	NoCullDesc.FrontCounterClockwise = false;
	NoCullDesc.DepthClipEnable = true;

	HRESULT hr = (device->CreateRasterizerState(&NoCullDesc, NoCullPR.GetAddressOf()));
	assert(SUCCEEDED(hr) && "Error when creating rasterizerDesc, in ParticleRenderer!");

	// Alpha to Coverage
	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = (device->CreateBlendState(&alphaToCoverageDesc, AlphaToCoveragePR.GetAddressOf()));
	assert(SUCCEEDED(hr) && "Error when creating blendDesc_Alpha, in ParticleRenderer!");

	// Transparent
	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable			  = true;
	transparentDesc.RenderTarget[0].SrcBlend			  = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend			  = D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp				  = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha		  = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha		  = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha		  = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = (device->CreateBlendState(&transparentDesc, TransparentPR.GetAddressOf()));
	assert(SUCCEEDED(hr) && "Error when creating blendDesc_Transparent, in ParticleRenderer!");

	Particle p;
	p.ParticleLifetime = 10.0f; // make longer maybe?
	p.Type = 0;
	this->m_initVB.initialize(this->m_device, &p, 1, false);
	
	this->m_drawVB.initialize(this->m_device, &p, 1, false, true);
	this->m_streamOutVB.initialize(this->m_device, &p, 1, false, true);

}

void ParticleRenderer::setNoCullPR()
{
	this->m_dContext->RSSetState(NoCullPR.Get());
}

void ParticleRenderer::setAlphaToCoveragePR()
{
	this->m_dContext->OMSetBlendState(AlphaToCoveragePR.Get(), nullptr, 0xFFFFFFFF);
}

void ParticleRenderer::setTransparentPR()
{
	this->m_dContext->OMSetBlendState(TransparentPR.Get(), nullptr, 0xFFFFFFFF);
}

void ParticleRenderer::setParticleSystem(ParticleSystem* particleSystem)
{
	this->m_particleSystem = particleSystem;
}

void ParticleRenderer::render(XMMATRIX vpMatrix)
{
	//
	// Set constants.
	//
	this->m_particleSystem->setVPMatrix(vpMatrix);

	this->m_particleBuffer.upd(this->m_particleSystem->getParticleBuffer());

	//
	// Set IA stage.
	//
	m_streamOutShader.setShaders();
	

	UINT stride = sizeof(Particle);
	UINT offset = 0;

	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	
	if (this->m_firstRun)
		this->m_dContext->IASetVertexBuffers(0, 1, m_initVB.GetAddressOf(), &stride, &offset);
	else
		this->m_dContext->IASetVertexBuffers(0, 1, m_drawVB.GetAddressOf(), &stride, &offset);


	this->m_dContext->SOSetTargets(1, m_streamOutVB.GetAddressOf(), &offset);
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	if (this->m_firstRun) 
	{
		this->m_dContext->Draw(1, 0);
		this->m_firstRun = false;
	}
	else 
	{
		this->m_dContext->DrawAuto();
	}
	
	

	// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = { 0 };
	this->m_dContext->SOSetTargets(1, bufferArray, &offset);

	// ping-pong the vertex buffers
	std::swap(m_drawVB, m_streamOutVB);

	//
	// Draw the updated particle system we just streamed-out. 
	//
	m_drawShader.setShaders(); // flytta tillbaka till "m_streamOutShader.setShaders();"

	this->m_dContext->IASetVertexBuffers(0, 1, m_drawVB.GetAddressOf(), &stride, &offset);

	this->m_dContext->DrawAuto();
}
