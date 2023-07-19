#include "pch.h"
#ifndef SSAOINSTANCE_H
#define SSAOINSTANCE_H
#include "ViewHelper.h"
#include "ConstantBuffer.h"
#include "Shaders.h"

const int SSAO_SAMPLE_DIRECTIONS = 14;

struct PS_SSAO_BUFFER
{
    XMFLOAT4 sampleDirections[SSAO_SAMPLE_DIRECTIONS];
    XMFLOAT2 ditherScale;
    XMFLOAT2 pad;
};

struct PS_SSAO_CAMERA_BUFFER
{
    XMMATRIX viewToTexMatrix;
    XMMATRIX projectionMatrix;
    XMMATRIX viewMatrix;
};

class SSAOInstance
{
private:
    // Device
    ID3D11DeviceContext* m_deviceContext;

    // Constant Buffers
    PS_SSAO_BUFFER m_SSAOData;
    PS_SSAO_CAMERA_BUFFER m_SSAOCameraData;
    XMMATRIX m_invProjectionMatrix;

    ConstBuffer<PS_SSAO_BUFFER> m_SSAOBuffer;
    ConstBuffer<PS_SSAO_CAMERA_BUFFER> m_SSAOCameraBuffer;
    ConstBuffer<XMMATRIX> m_SSAOMatrixBuffer;

    // Texture
    RenderTexture m_texture;
    ID3D11RenderTargetView* m_renderTargetNullptr = nullptr;

    // Shader
    Shaders m_SSAOPassShaders;

    // Sampler
    Microsoft::WRL::ComPtr< ID3D11SamplerState > m_depthNormalSamplerState;
    Microsoft::WRL::ComPtr< ID3D11SamplerState > m_randomSamplerState;

    // Dither Texture
    Microsoft::WRL::ComPtr< ID3D11Texture2D > m_ditherTexture;
    Microsoft::WRL::ComPtr< ID3D11ShaderResourceView > m_ditherTextureSRV;
    
    // Functions
    void initSSAOTexture(ID3D11Device* device, int width, int height)
    {
        std::string name = "SSAO Map0";
        strncpy_s(m_texture.name, name.c_str(), name.length());
        m_texture.name[name.length()] = '\0';
        // Texture
        D3D11_TEXTURE2D_DESC textureDesc;
        ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
        textureDesc.Width = width;
        textureDesc.Height = height;
        textureDesc.MipLevels = 1;
        textureDesc.ArraySize = 1;
        textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        textureDesc.SampleDesc.Count = 1;
        textureDesc.Usage = D3D11_USAGE_DEFAULT;
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        textureDesc.CPUAccessFlags = 0;
        textureDesc.MiscFlags = 0;

        HRESULT hr = device->CreateTexture2D(&textureDesc, NULL, &m_texture.rtt);
        assert(SUCCEEDED(hr) && "Error, render target texture could not be created!");

        // Render Rarget View
        D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
        ZeroMemory(&renderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        renderTargetViewDesc.Format = textureDesc.Format;
        renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        renderTargetViewDesc.Texture2D.MipSlice = 0;

        hr = device->CreateRenderTargetView(m_texture.rtt, &renderTargetViewDesc, &m_texture.rtv);
        assert(SUCCEEDED(hr) && "Error, render target view could not be created!");

        // Shader Resource View
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        hr = device->CreateShaderResourceView(m_texture.rtt, &srvDesc, &m_texture.srv);
        assert(SUCCEEDED(hr) && "Error, shader resource view could not be created!");

        // Unordered Access View
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = textureDesc.Format;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
        uavDesc.Texture2D.MipSlice = 0;

        hr = device->CreateUnorderedAccessView(m_texture.rtt, &uavDesc, &m_texture.uav);
        assert(SUCCEEDED(hr) && "Error, unordered access view could not be created!");
    }
    void initRandomTexture(ID3D11Device* device)
    {
        //HRESULT hr;

        // Dither Texture
        int ditherWidth = 4;
        int ditherHeight = 4;

        D3D11_TEXTURE2D_DESC texDesc;
        texDesc.Width = ditherWidth;
        texDesc.Height = ditherHeight;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        texDesc.MiscFlags = 0;

        int len = ditherWidth * ditherHeight;
        std::vector<XMFLOAT4> data(len);

        std::vector<float> offsets1;
        std::vector<float> offsets2;

        for (size_t i = 0; i < len; ++i)
        {
            offsets1.push_back((float)i / len);
            offsets2.push_back((float)i / len - 1);
        }

        unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(offsets1.begin(), offsets1.end(), std::default_random_engine(seed));
        seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(offsets2.begin(), offsets2.end(), std::default_random_engine(seed));

        int i = 0;
        for (int y = 0; y < ditherHeight; ++y)
        {
            for (int x = 0; x < ditherWidth; ++x)
            {
                float r = offsets1[i];
                float g = offsets2[i];

                data[i] = XMFLOAT4(r, g, 1.f, 1.f);
                ++i;
            }
        }

        D3D11_SUBRESOURCE_DATA subData;
        subData.pSysMem = data.data();
        subData.SysMemPitch = texDesc.Width * sizeof(XMFLOAT4);
        subData.SysMemSlicePitch = subData.SysMemPitch * texDesc.Height;


        /*int dimensions = 256;
        D3D11_TEXTURE2D_DESC texDesc;
        texDesc.Width = dimensions;
        texDesc.Height = dimensions;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DYNAMIC;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        texDesc.MiscFlags = 0;

        XMVECTOR* randomColors = new XMVECTOR[dimensions * dimensions];
        for (int i = 0; i < dimensions; i++)
        {
            for (int j = 0; j < dimensions; j++)
                randomColors[i * dimensions + j] = XMVectorSet(((float)(rand()) / (float)RAND_MAX), ((float)(rand()) / (float)RAND_MAX), ((float)(rand()) / (float)RAND_MAX), ((float)(rand()) / (float)RAND_MAX));
        }

        D3D11_SUBRESOURCE_DATA subData;
        subData.pSysMem = randomColors;
        subData.SysMemPitch = texDesc.Width * sizeof(XMVECTOR);
        subData.SysMemSlicePitch = subData.SysMemPitch * texDesc.Height;*/

        //hr = device->CreateTexture2D(&texDesc, &subData, m_ditherTexture.GetAddressOf());
        //assert(SUCCEEDED(hr) && "Error, failed to create dither texture!");

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = texDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = 1;

        //hr = device->CreateShaderResourceView(m_ditherTexture.Get(), &srvDesc, m_ditherTextureSRV.GetAddressOf());
        //assert(SUCCEEDED(hr) && "Error, failed to create dither texture shader resource view!");
    }
    void initSamplerStates(ID3D11Device* device)
    {
        D3D11_SAMPLER_DESC samplerStateDesc;
        samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        samplerStateDesc.MinLOD = (-FLT_MAX);
        samplerStateDesc.MaxLOD = (FLT_MAX);
        samplerStateDesc.MipLODBias = 0.0f;
        samplerStateDesc.MaxAnisotropy = 1;
        samplerStateDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        samplerStateDesc.BorderColor[0] = 1.f;
        samplerStateDesc.BorderColor[1] = 1.f;
        samplerStateDesc.BorderColor[2] = 1.f;
        samplerStateDesc.BorderColor[3] = 1.f;

        HRESULT hr = device->CreateSamplerState(&samplerStateDesc, &m_randomSamplerState);
        assert(SUCCEEDED(hr) && "Error, failed to create depth normal sampler state!");

        samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerStateDesc.BorderColor[0] = 1.f;
        samplerStateDesc.BorderColor[1] = 1.f;
        samplerStateDesc.BorderColor[2] = 1.f;
        samplerStateDesc.BorderColor[3] = 1.f;

        hr = device->CreateSamplerState(&samplerStateDesc, &m_depthNormalSamplerState);
        assert(SUCCEEDED(hr) && "Error, failed to create random sampler state!");

        m_deviceContext->PSSetSamplers(3, 1, m_depthNormalSamplerState.GetAddressOf());
        m_deviceContext->PSSetSamplers(4, 1, m_randomSamplerState.GetAddressOf());
    }
    void initSampleDirections()
    {
        // 8 cube corners
        m_SSAOData.sampleDirections[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.f);
        m_SSAOData.sampleDirections[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.f);
        m_SSAOData.sampleDirections[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.f);
        m_SSAOData.sampleDirections[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.f);
        m_SSAOData.sampleDirections[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.f);
        m_SSAOData.sampleDirections[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.f);
        m_SSAOData.sampleDirections[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.f);
        m_SSAOData.sampleDirections[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.f);

        // 6 centers of cube faces
        m_SSAOData.sampleDirections[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.f);
        m_SSAOData.sampleDirections[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.f);
        m_SSAOData.sampleDirections[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.f);
        m_SSAOData.sampleDirections[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.f);
        m_SSAOData.sampleDirections[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.f);
        m_SSAOData.sampleDirections[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.f);

        for (int i = 0; i < SSAO_SAMPLE_DIRECTIONS; ++i) {
            float s = pMath::frand(0.25f, 1.0f);
            XMVECTOR vec = XMVector3Normalize(XMLoadFloat4(&m_SSAOData.sampleDirections[i]));
            vec *= s;
            XMStoreFloat4(&m_SSAOData.sampleDirections[i], vec);
        }
    }

public:
    SSAOInstance() {}
    ~SSAOInstance()
    {
        if (m_texture.rtt)
            m_texture.rtt->Release();
        m_texture.rtt = nullptr;
        if (m_texture.rtv)
            m_texture.rtv->Release();
        m_texture.rtv = nullptr;
        if (m_texture.srv)
            m_texture.srv->Release();
        m_texture.srv = nullptr;
        if (m_texture.uav)
            m_texture.uav->Release();
        m_texture.uav = nullptr;
    }

    void initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int width, int height)
    {
        // DeviceConstext
        m_deviceContext = deviceContext;

        // Shader
        ShaderFiles shaderFiles;
        shaderFiles.vs = L"Shader Files\\SsaoVS.hlsl";
        shaderFiles.ps = L"Shader Files\\SsaoPS.hlsl";
        m_SSAOPassShaders.initialize(device, deviceContext, shaderFiles, LayoutType::POS_TEX);

        // SSAO Texture
        initSSAOTexture(device, width, height);

        // Random Texture
        initRandomTexture(device);

        // Sampler State Setup
        initSamplerStates(device);

        // Constant Buffers
        // - SSAO Buffer Sample Directions
        initSampleDirections();

        // - Dither Scale for Random Texture
        m_SSAOData.ditherScale = XMFLOAT2((float)width / 4, (float)height / 4);
        m_SSAOBuffer.init(device, deviceContext);
        m_SSAOBuffer.m_data = m_SSAOData;
        m_SSAOBuffer.upd();

        // - SSAO Matrix Buffer
        m_SSAOMatrixBuffer.init(device, deviceContext);

        // - SSAO Camera Buffer
        m_SSAOCameraBuffer.init(device, deviceContext);
    }

    RenderTexture& getAORenderTexture()
    {
        return m_texture;
    }

    void updateProjMatrix(XMMATRIX projectionMatrix)
    {
        m_invProjectionMatrix = XMMatrixTranspose(XMMatrixInverse(nullptr, projectionMatrix));
        m_SSAOMatrixBuffer.m_data = m_invProjectionMatrix;
        m_SSAOMatrixBuffer.upd();

        XMMATRIX T(
            0.5f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f);
        XMMATRIX P = projectionMatrix;
        XMMATRIX pt = XMMatrixMultiply(P, T);

        m_SSAOCameraData.viewToTexMatrix = XMMatrixTranspose(pt);
        m_SSAOCameraData.projectionMatrix = XMMatrixTranspose(projectionMatrix);
    }
    void updateViewMatrix(XMMATRIX viewMatrix)
    {
        m_SSAOCameraData.viewMatrix = XMMatrixTranspose(viewMatrix);
        m_SSAOCameraBuffer.m_data = m_SSAOCameraData;
        m_SSAOCameraBuffer.upd();
    }
    void updateShaders()
    {
        m_SSAOPassShaders.reloadShaders();
    }

    void render()
    {
        // Set Render Target
        m_deviceContext->OMSetRenderTargets(1, &m_texture.rtv, nullptr);

        // Shader
        m_SSAOPassShaders.setShaders();

        // Constant Buffers
        m_deviceContext->VSSetConstantBuffers(0, 1, m_SSAOMatrixBuffer.GetAddressOf());
        m_deviceContext->PSSetConstantBuffers(0, 1, m_SSAOCameraBuffer.GetAddressOf());
        m_deviceContext->PSSetConstantBuffers(1, 1, m_SSAOBuffer.GetAddressOf());

        // Dither Texture
        //m_deviceContext->PSSetShaderResources(2, 1, m_ditherTextureSRV.GetAddressOf());
        ID3D11ShaderResourceView* srv = nullptr;
        m_deviceContext->PSSetShaderResources(2, 1, &srv);

        // Draw Fullscreen Quad
        m_deviceContext->Draw(4, 0);

        // Reset
        m_deviceContext->OMSetRenderTargets(1, &m_renderTargetNullptr, nullptr);
    }
};

#endif // !SSAOINSTANCE_H