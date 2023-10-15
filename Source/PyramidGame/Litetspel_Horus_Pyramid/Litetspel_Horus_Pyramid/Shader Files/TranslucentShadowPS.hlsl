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
    float4 color = float4(diffuseM.rgb * (1.f - opacity), input.pos.z);
    
    return color;
}
