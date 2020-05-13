#include "pch.h"
#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer()
{
}

ParticleRenderer::~ParticleRenderer()
{
}

void ParticleRenderer::initlialize(ID3D11Device* device, ID3D11DeviceContext* dContext)
{
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
	p.ParticleLifetime = 0.0f;
	p.Type = 0;
	this->m_initVB.initialize(this->m_device, &p, 1, false);
	
	this->m_drawVB.initialize(this->m_device, &p, 1, false);
	this->m_streamOutVB.initialize(this->m_device, &p, 1, false);

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

void ParticleRenderer::setParticleSystem(ParticleSystem* m_particleSystem)
{

}

void ParticleRenderer::render()
{
}
