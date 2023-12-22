struct PS_IN
{
    float4 pos : SV_POSITION;
};

cbuffer materialBuffer : register(b0)
{
    float3 ambientM;
    float globalAmbientContributionM;
    float4 diffuseM;
    float4 specularM;
    float shininessM;
};

Texture2D shadowMap : TEXTURE : register(t0);
SamplerComparisonState shadowSampler : SAMPLER : register(s1);

#define CASCADING_LIGHT_COUNT 3

float4 main(PS_IN input) : SV_TARGET
{
    float2 shadowMapOffset = float2(1.f / CASCADING_LIGHT_COUNT, 0.f);
    float shadowFactor = shadowMap.SampleCmpLevelZero(shadowSampler, input.pos.xy + shadowMapOffset, input.pos.z);
    if (shadowFactor > 0.f)
        discard;
    
    float opacity = diffuseM.a;
    float4 color = float4(diffuseM.rgb * (1.f - opacity), input.pos.z);
    
    return color;
}
