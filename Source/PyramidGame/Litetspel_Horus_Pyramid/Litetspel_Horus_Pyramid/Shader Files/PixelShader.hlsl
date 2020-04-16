struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
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

float4 main(PS_IN input) : SV_TARGET
{
    float3 color = lightColor * strength;
    float diffBright = saturate(dot(input.normal, dirLightDirection.xyz));
    
    color += dirLightColor.xyz * diffBright;
    float3 fColor = diffuse.xyz * color.xyz;
    
    return float4(fColor.xyz, 1);
}
