struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;
};

cbuffer materialBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
};

cbuffer lightBuffer : register(b1) //Ambient
{
    float3 lightColor;
    float strength;
};

cbuffer directionalLight : register(b2)
{
    float4 dirLightDirection;
    float4 dirLightColor;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);


float4 main(PS_IN input) : SV_TARGET
{
    float4 pixelColorFromTexture = objTexture.Sample(samplerState, input.inTextureCord);
    float3 color = lightColor * strength;
    float diffBright = saturate(dot(input.normal, dirLightDirection.xyz));
    
    color += dirLightColor.xyz * diffBright;
    float3 fColor = diffuse.xyz * pixelColorFromTexture.xyz * color.xyz;
    
    return float4(fColor.xyz, 1);
}
