#include "pch.h"
#include "Shaders.h"

Shaders::Shaders()
{
	this->m_device = nullptr;
	this->m_deviceContext = nullptr;

	this->m_vertexShader = nullptr;
	this->m_hullShader = nullptr;
	this->m_domainShader = nullptr;
	this->m_geometryShader = nullptr;
	this->m_pixelShader = nullptr;
	this->m_computeShader = nullptr;

	this->m_topology = D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

Shaders::~Shaders() {}


void Shaders::initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ShaderFiles names, LayoutType layoutType, D3D_PRIMITIVE_TOPOLOGY topology)
{
	// Device
	this->m_device = device;
	this->m_deviceContext = deviceContext;

	// Topology
	this->m_topology = topology;

	// Create Shaders
	// Binary Large OBject (BLOB), for compiled shader, and errors.
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	// Create Vertex Shader
	if (names.vs != L"")
	{
		ID3DBlob* vsBlob = nullptr;
		hr = D3DCompileFromFile(
			names.vs,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"vs_5_0",
			flags,
			0,
			&vsBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (vsBlob)
				vsBlob->Release();
			assert(SUCCEEDED(hr));
		}

		hr = this->m_device->CreateVertexShader(
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			nullptr,
			&this->m_vertexShader
		);

		assert(SUCCEEDED(hr) && "Error, Vertex shaders could not be created!");

		if (layoutType == LayoutType::POS_NOR_TEX) {
			// Vertex Layout
			hr = device->CreateInputLayout(
				PosNorTexDesc,
				3,
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				&this->m_layout
			);
			assert(SUCCEEDED(hr) && "Error, Input layout could not be created!");
		}
		else if (layoutType == LayoutType::POS_TEX)
		{
			hr = device->CreateInputLayout(
				PosTexDesc,
				2,
				vsBlob->GetBufferPointer(),
				vsBlob->GetBufferSize(),
				&this->m_layout
			);
			assert(SUCCEEDED(hr) && "Error, Input layout could not be created!");
		}

	}


	// Create Hull Shader
	if (names.hs != L"")
	{
		ID3DBlob* hsBlob = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		hr = D3DCompileFromFile(
			names.hs,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"hs_5_0",
			flags,
			0,
			&hsBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (hsBlob)
				hsBlob->Release();
			assert(SUCCEEDED(hr));
		}

		hr = this->m_device->CreateHullShader(
			hsBlob->GetBufferPointer(),
			hsBlob->GetBufferSize(),
			nullptr,
			&this->m_hullShader
		);
		assert(SUCCEEDED(hr) && "Error, Hull shaders could not be created!");
		hsBlob->Release();
	}


	// Create Domain Shader
	if (names.ds != L"")
	{
		ID3DBlob* dsBlob = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		hr = D3DCompileFromFile(
			names.ds,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"ds_5_0",
			flags,
			0,
			&dsBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (dsBlob)
				dsBlob->Release();
			assert(SUCCEEDED(hr));
		}

		hr = this->m_device->CreateDomainShader(
			dsBlob->GetBufferPointer(),
			dsBlob->GetBufferSize(),
			nullptr,
			&this->m_domainShader
		);
		assert(SUCCEEDED(hr) && "Error, Domain shaders could not be created!");
		dsBlob->Release();
	}


	// Create Geometry Shader
	if (names.gs != L"")
	{
		ID3DBlob* gsBlob = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		hr = D3DCompileFromFile(
			names.gs,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"gs_5_0",
			flags,
			0,
			&gsBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (gsBlob)
				gsBlob->Release();
			assert(SUCCEEDED(hr));
		}

		hr = this->m_device->CreateGeometryShader(
			gsBlob->GetBufferPointer(),
			gsBlob->GetBufferSize(),
			nullptr,
			&this->m_geometryShader);

		assert(SUCCEEDED(hr) && "Error, Geometry shaders could not be created!");
		gsBlob->Release();
	}


	// Create Pixel Shader
	if (names.ps != L"")
	{
		ID3DBlob* psBlob = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		hr = D3DCompileFromFile(
			names.ps,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"main",
			"ps_5_0",
			flags,
			0,
			&psBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (psBlob)
				psBlob->Release();
			assert(false);
		}

		hr = this->m_device->CreatePixelShader(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&this->m_pixelShader
		);
		assert(SUCCEEDED(hr) && "Error, Pixel shaders could not be created!");
		psBlob->Release();
	}


	// Create Compute Shader
	if (names.cs != L"")
	{
		ID3DBlob* csBlob = nullptr;
		if (errorBlob) errorBlob->Release();
		errorBlob = nullptr;

		hr = D3DCompileFromFile(
			names.cs,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			"CS_main",
			"cs_5_0",
			flags,
			0,
			&csBlob,
			&errorBlob
		);

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}
			if (csBlob)
				csBlob->Release();
			assert(SUCCEEDED(hr));
		}

		hr = this->m_device->CreateComputeShader(
			csBlob->GetBufferPointer(),
			csBlob->GetBufferSize(),
			nullptr,
			&this->m_computeShader
		);
		assert(SUCCEEDED(hr) && "Error, Compute shaders could not be created!");
		csBlob->Release();
	}
}

void Shaders::setShaders()
{
	this->m_deviceContext->IASetPrimitiveTopology(this->m_topology);
	this->m_deviceContext->IASetInputLayout(this->m_layout.Get());

	this->m_deviceContext->VSSetShader(this->m_vertexShader.Get(), nullptr, 0);
	this->m_deviceContext->HSSetShader(this->m_hullShader.Get(), nullptr, 0);
	this->m_deviceContext->DSSetShader(this->m_domainShader.Get(), nullptr, 0);
	this->m_deviceContext->GSSetShader(this->m_geometryShader.Get(), nullptr, 0);
	this->m_deviceContext->PSSetShader(this->m_pixelShader.Get(), nullptr, 0);
	this->m_deviceContext->CSSetShader(this->m_computeShader.Get(), nullptr, 0);
}

void Shaders::unbindShaders()
{
	this->m_deviceContext->VSSetShader(nullptr, nullptr, 0);
	this->m_deviceContext->HSSetShader(nullptr, nullptr, 0);
	this->m_deviceContext->DSSetShader(nullptr, nullptr, 0);
	this->m_deviceContext->GSSetShader(nullptr, nullptr, 0);
	this->m_deviceContext->PSSetShader(nullptr, nullptr, 0);
	this->m_deviceContext->CSSetShader(nullptr, nullptr, 0);
}