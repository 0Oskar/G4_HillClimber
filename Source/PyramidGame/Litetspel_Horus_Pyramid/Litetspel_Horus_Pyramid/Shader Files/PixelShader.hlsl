struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 color : COLOR;
};

cbuffer materialBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
};

float4 main(PS_IN input) : SV_TARGET
{
    return float4(diffuse);
}
