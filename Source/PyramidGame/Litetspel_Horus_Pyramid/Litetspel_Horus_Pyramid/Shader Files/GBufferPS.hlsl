struct PS_IN
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 positionW : POSITION;
    float4 positionShadow : POSITION1;
};

struct PS_OUT
{
    float4 diffuseAlphaRT : SV_Target0;
    float4 normalShadowMaskRT : SV_Target1;
    float4 specularShininessRT : SV_Target2;
    float4 ambientGlobalAContributionRT : SV_Target3;
};

cbuffer materialBuffer : register(b0)
{
    float3 ambientM;
    float globalAmbientContributionM;
    float4 diffuseM;
    float4 specularM;
    float shininessM;
};

Texture2D diffuseTexture : TEXTURE : register(t0);
Texture2D shadowMap : TEXTURE : register(t1);

SamplerState samplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSampler : SAMPLER : register(s1);

float calculateShadowFactor(float4 shadowPosition)
{
    float2 shadowUV = shadowPosition.xy / shadowPosition.w * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
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

PS_OUT main(PS_IN input)
{
    PS_OUT output = (PS_OUT)0;
    
    // Diffuse
    float4 diffuse = diffuseM * diffuseTexture.Sample(samplerState, input.texcoord);
    
    // Shadow Mask
    float shadowFactor = calculateShadowFactor(input.positionShadow);
    
    // Diffuse, Alpha
    output.diffuseAlphaRT = diffuse;
    
    // Normal, Shadow Mask
    output.normalShadowMaskRT = float4(normalize(input.normal.xyz), shadowFactor);
    
    // Specular, Shininess
    output.specularShininessRT = float4(specularM.rgb, log2(shininessM) / 10.5f);
    
    // Ambient, Global Ambient Contribution
    output.ambientGlobalAContributionRT = float4(ambientM, globalAmbientContributionM);
    
    return output;
}