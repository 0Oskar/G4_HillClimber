struct PS_IN
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 positionW : POSITIONT;
    float4 positionShadow0 : POSITION0;
    float4 positionShadow1 : POSITION1;
    float4 positionShadow2 : POSITION2;
};

struct PS_OUT
{
    float4 diffuseAlphaRT : SV_Target0;
    float4 normalRT : SV_Target1;
    float4 specularShininessRT : SV_Target2;
    float4 ambientGlobalAContributionRT : SV_Target3;
    float4 shadowRT : SV_Target4;
};

cbuffer materialBuffer : register(b0)
{
    float3 ambientM;
    float globalAmbientContributionM;
    float4 diffuseM;
    float4 specularM;
    float shininessM;
};

cbuffer PerFrameBuffer : register(b1)
{
    bool cascadingShadowMapsToggle;
    float3 cameraPos;
    
    float frustumCoverage0;
    float frustumCoverage1;
    float frustumCoverage2;
    
    bool shadowDebug;
};

Texture2D diffuseTexture : TEXTURE : register(t0);
Texture2D shadowMap : TEXTURE : register(t1);
Texture2D translucentShadowMap : TEXTURE : register(t4);

SamplerState samplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSampler : SAMPLER : register(s1);

#define CASCADING_LIGHT_COUNT 3

float calculateShadowFactor(float2 textureOffset, float4 shadowPosition)
{
    float2 shadowUV = shadowPosition.xy / shadowPosition.w * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
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
            shadowFactor += shadowMap.SampleCmpLevelZero(shadowSampler, shadowUV, shadowDepth, int2(x, y));
        }
    }
    shadowFactor /= ((sampleRange * 2 + 1) * (sampleRange * 2 + 1));
    return shadowFactor;
}

float3 calculateTranslucentShadowFactor(Texture2D shadowMap, float4 shadowPosition)
{
    float2 shadowUV = shadowPosition.xy / shadowPosition.w * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
    float shadowDepth = shadowPosition.z / shadowPosition.w;

    float3 shadowFactor = (float3)0;
    float4 shadowSample = (float4)0;
    const int sampleRange = 1;
    [unroll]
    for (int x = -sampleRange; x <= sampleRange; x++)
    {
        [unroll]
        for (int y = -sampleRange; y <= sampleRange; y++)
        {
            shadowSample = shadowMap.Sample(samplerState, shadowUV, int2(x, y));
            if (shadowDepth < shadowSample.a)
                shadowFactor += shadowSample.rgb;
        }
    }
    shadowFactor /= ((sampleRange * 2 + 1) * (sampleRange * 2 + 1));
    return shadowFactor;
}

float invLerp(float a, float b, float v)
{
    return (v - a) / (b - a);
}

PS_OUT main(PS_IN input)
{
    PS_OUT output = (PS_OUT)0;
    
    // Diffuse
    float4 diffuse = diffuseM * diffuseTexture.Sample(samplerState, input.texcoord);
    
    // Shadow Mask
    float shadowFactor = 1.f;
    float3 shadowColor = (float3)1.f;
    float3 translucentshadowcolor = (float3)0;
    float2 textureOffset = (float2)0;
    
    if (cascadingShadowMapsToggle)
    {
        float4 shadowPosition = (float4)0;
        float shadowDistance = distance(cameraPos, input.positionW);
        
        if (shadowDistance < frustumCoverage0)
        {
            shadowPosition = input.positionShadow0;
            textureOffset = float2(-(1.f / CASCADING_LIGHT_COUNT), 0.f);
            if (shadowDebug)
                diffuse.xyz = float3(1.f, 0.f, 0.f) * saturate(invLerp(0.f, frustumCoverage0, shadowDistance) + 0.1f);
        }
        else if (shadowDistance >= frustumCoverage0 && shadowDistance < frustumCoverage1)
        {
            shadowPosition = input.positionShadow1;
            textureOffset = float2(0.f, 0.f);
            if (shadowDebug)
                diffuse.xyz = float3(0.f, 1.f, 0.f) * saturate(invLerp(frustumCoverage0, frustumCoverage1, shadowDistance) + 0.1f);
        }
        else if (shadowDistance >= frustumCoverage1 && shadowDistance < frustumCoverage2)
        {
            shadowPosition = input.positionShadow2;
            textureOffset = float2(1.f / CASCADING_LIGHT_COUNT, 0.f);
            if (shadowDebug)
                diffuse.xyz = float3(0.f, 0.f, 1.f) * saturate(invLerp(frustumCoverage1, frustumCoverage2, shadowDistance) + 0.1f);
        }
        shadowFactor = calculateShadowFactor(textureOffset, shadowPosition);
        
        translucentshadowcolor = calculateTranslucentShadowFactor(translucentShadowMap, input.positionShadow2);
        shadowColor = float3(shadowFactor, shadowFactor, shadowFactor) - translucentshadowcolor;
    }
    else
    {
        shadowFactor = calculateShadowFactor(textureOffset, input.positionShadow0);
        shadowColor = float3(shadowFactor, shadowFactor, shadowFactor);
    }
    
    // Diffuse, Alpha
    output.diffuseAlphaRT = diffuse;
    
    // Normal
    output.normalRT = float4(normalize(input.normal.xyz), 1.f);
    
    // Specular, Shininess
    output.specularShininessRT = float4(specularM.rgb, log2(shininessM) / 10.5f);
    
    // Ambient, Global Ambient Contribution
    output.ambientGlobalAContributionRT = float4(ambientM, globalAmbientContributionM);
    
    // Shadow
    output.shadowRT = float4(shadowColor, 1.f);
    
    return output;
}