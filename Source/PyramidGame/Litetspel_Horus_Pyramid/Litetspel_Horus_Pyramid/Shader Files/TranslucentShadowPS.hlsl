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

float4 main(PS_IN input) : SV_TARGET
{
    float opacity = diffuseM.a;
    float3 diffuse = diffuseM.rgb;
    
    return float4(float3(diffuse * (1.f - opacity)), 1);
}
