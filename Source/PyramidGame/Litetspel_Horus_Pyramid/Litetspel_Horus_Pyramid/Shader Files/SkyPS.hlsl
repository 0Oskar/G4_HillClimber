struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 HPosition : POSITIONT;
    float3 WPosition : POSITION;
};

//Texture2D objTexture : TEXTURE : register(t0);
//SamplerState samplerState : SAMPLER : register(s0);

float4 main(PS_IN input) : SV_TARGET
{
    const float3 skyColor = float3(0.25, 0.55, 0.9);
    const float3 horizonColor = float3(0.89, 0.824, 0.651);
    const float3 groundColor = float3(1.0, 0.871, 0.55);
    const float skyStrength = 0.5;
    const float blendStrength = 1.0;
    
    float height = normalize(input.WPosition).y;
    float skyBlend =    pow(clamp(height, 0.0, 1.0), skyStrength);
    float groundBlend = pow(clamp(height, -1.0, 0.0) * -1.0, blendStrength);
    float horizonBlend = pow(1.0 - (skyBlend + groundBlend), blendStrength);
    
    float3 finalColor = (skyColor * skyBlend) + (groundColor * groundBlend) + (horizonColor * horizonBlend);
    return float4(finalColor, 1.0);
}