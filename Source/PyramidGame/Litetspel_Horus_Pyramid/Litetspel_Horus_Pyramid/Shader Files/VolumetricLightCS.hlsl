// Globals
static const float PI = 3.14159265359;
static const float PI_RCP = 0.31830988618379067153776752674503f;
static const float LIGHT_SCATTERING_G = -0.5f;
static const int RAY_STEPS = 128;
static const float2 CULL_UV_OFFSET = float2(0.1f, 0.1f);
#define CASCADING_LIGHT_COUNT 3

static const float ditherPattern[16] =
{
    0.0f, 0.5f, 0.125f, 0.625f,
    0.75f, 0.22f, 0.875f, 0.375f,
    0.1875f, 0.6875f, 0.0625f, 0.5625,
    0.9375f, 0.4375f, 0.8125f, 0.3125
};

// Constant Buffers
cbuffer CameraCB : register(b0)
{
    matrix viewInverseMatrix;
    matrix projInverseMatrix;
    float3 cameraPosition;
    float pad;
};

cbuffer shadowCB : register(b1)
{
    matrix lightViewProjMatrix[CASCADING_LIGHT_COUNT];
    float frustumCoverage0;
    float frustumCoverage1;
    float frustumCoverage2;
};

cbuffer SkyLightDataCB : register(b2)
{
    float3 skyLightDirection;
    float skyLightIntensity;
    float3 skyLightColor;
    float2 textureStep;
    float3 pad2;
};

// Textures
RWTexture2D<float4> OutputTexture : register(u0);
Texture2D<float> DepthTexture : register(t0);
Texture2D<float> ShadowTexture : register(t1);

SamplerComparisonState shadowSampler : SAMPLER : register(s1);

// Functions
float2 getTexcoord(uint2 id)
{
    return (float2) id * textureStep;
}

float sampleShadow(float2 textureOffset, float4 shadowPosition)
{
    float2 shadowUV = shadowPosition.xy / shadowPosition.w * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
    shadowUV.x /= CASCADING_LIGHT_COUNT;
    shadowUV += textureOffset;
    float shadowDepth = shadowPosition.z / shadowPosition.w;

    float shadowFactor = 0;
    const int sampleRange = 1;
    [unroll]
    for (int x = -sampleRange; x <= sampleRange; x++)
    {
        [unroll]
        for (int y = -sampleRange; y <= sampleRange; y++)
        {
            shadowFactor += ShadowTexture.SampleCmpLevelZero(shadowSampler, shadowUV, shadowDepth, int2(x, y));
        }
    }
    shadowFactor /= ((sampleRange * 2 + 1) * (sampleRange * 2 + 1));
    return shadowFactor;
}

// Mie scaterring approximated with Henyey-Greenstein phase function.
float ComputeScattering(float lightDotView)
{
    float result = 1.0f - LIGHT_SCATTERING_G * LIGHT_SCATTERING_G;
    result /= (4.0f * PI * pow(1.0f + LIGHT_SCATTERING_G * LIGHT_SCATTERING_G - (2.0f * LIGHT_SCATTERING_G) * lightDotView, 1.5f));
    return result;
}

float invLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

// Main
[numthreads(16, 16, 1)]
void main(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchId : SV_DispatchThreadID)
{
    uint2 pixelIndex = uint2(dispatchId.x, dispatchId.y);
    float2 uv = getTexcoord(pixelIndex);
    float4 finalColor = (float4) 0;
    
    // Position
    float z = DepthTexture.Load(int3(pixelIndex, 0)).r;
    float x = uv.x * 2.f - 1.f;
    float y = (1.f - uv.y) * 2.f - 1.f;
    float4 startViewPosition = mul(float4(x, y, z, 1.f), projInverseMatrix);
    float4 endViewPosition = mul(float4(x, y, 0.f, 1.f), projInverseMatrix);
    startViewPosition /= startViewPosition.w;
    endViewPosition /= endViewPosition.w;
    
    float3 startPositionW = mul(startViewPosition, viewInverseMatrix).xyz;
    float3 endPositionW = mul(endViewPosition, viewInverseMatrix).xyz;
    float3 endToStartW = endPositionW - startPositionW;
    
    // Ray
    float3 deltaW = normalize(endToStartW);
    float raymarchDistanceW = length(endToStartW);
    float stepSizeW = raymarchDistanceW / RAY_STEPS;
    
    float ditherValue = ditherPattern[(x % 4.f) * 4.f + (y % 4.f)];
    float4 rayPositionW = float4(startPositionW + ditherValue * stepSizeW * deltaW, 1.f);
    
    //{
    //    float shadowDistance = distance(cameraPosition, startPositionW.xyz);
    //    uint shadowIndex = 0;
    //    float2 textureOffset = (float2) 0;
    //    float3 debugColor = float3(0.f, 0.f, 0.f);
    //    if (shadowDistance >= frustumCoverage0 && shadowDistance < frustumCoverage1)
    //    {
    //        shadowIndex = 1;
    //        textureOffset = float2((1.f / CASCADING_LIGHT_COUNT), 0.f);
    //        debugColor = float3(0.f, 1.f, 0.f) * saturate(invLerp(frustumCoverage0, frustumCoverage1, shadowDistance) + 0.1f);
    //    }
    //    else if (shadowDistance >= frustumCoverage1 && shadowDistance < frustumCoverage2)
    //    {
    //        shadowIndex = 2;
    //        textureOffset = float2(1.f - (1.f / CASCADING_LIGHT_COUNT), 0.f);
    //        debugColor = float3(0.f, 0.f, 1.f) * saturate(invLerp(frustumCoverage1, frustumCoverage2, shadowDistance) + 0.1f);
    //    }
    //    float4 rayPositionLightClipspace = mul(float4(startPositionW.xyz, 1.f), lightViewProjMatrix[shadowIndex]);
    
    //    float shadowTerm = sampleShadow(textureOffset, rayPositionLightClipspace);
    //    //OutputTexture[pixelIndex] = float4(debugColor, 1.f);
    //    OutputTexture[pixelIndex] = float4(shadowTerm, shadowTerm, shadowTerm, 1.f);
    //    return;
    //}
    
    // Light Values
    float tau = 0.0001f;
    float intensity = 100000000.f;
    float4 lightPosition = float4(skyLightDirection * 5000.f, 1.f);
    
    // Raymarch loop
    float lightContribution = 0.f;
    for (float l = raymarchDistanceW; l > stepSizeW; l -= stepSizeW)
    {
        // Shadow positioning
        float shadowDistance = distance(cameraPosition, rayPositionW.xyz);
    
        uint shadowIndex = 0;
        float2 textureOffset = (float2) 0;
        if (shadowDistance >= frustumCoverage0 && shadowDistance < frustumCoverage1)
        {
            shadowIndex = 1;
            textureOffset = float2((1.f / CASCADING_LIGHT_COUNT), 0.f);
        }
        else if (shadowDistance >= frustumCoverage1 && shadowDistance < frustumCoverage2)
        {
            shadowIndex = 2;
            textureOffset = float2(1.f - (1.f / CASCADING_LIGHT_COUNT), 0.f);
        }
        float4 rayPositionLightClipspace = mul(float4(rayPositionW.xyz, 1.f), lightViewProjMatrix[shadowIndex]);
        float shadowTerm = sampleShadow(textureOffset, rayPositionLightClipspace);
        
        // get distance in world space
        float d = length(rayPositionW - lightPosition);
        // multiplication is faster than division, so do it once
        float dRcp = 1.f / d;
    
        // accumulate to light contribution based on approximation of radiative transport equation
        lightContribution += tau * (shadowTerm * (intensity * 0.25f * PI_RCP) * dRcp * dRcp) *
        exp(-d * tau) * exp(-l * tau) * stepSizeW;
        
        // continue ray marching
        rayPositionW += stepSizeW * float4(deltaW, 0.f);
    }
    
    // Tonemap Light Contribution
    lightContribution = pow(lightContribution, 0.15f) / 2.f;
    
    OutputTexture[pixelIndex] = float4(saturate(lightContribution) * skyLightColor, 1.f);
}