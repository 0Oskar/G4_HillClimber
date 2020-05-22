struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;
    float3 posWorld : Position;
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

cbuffer fog : register(b3)
{
    float3 cameraPos;
    float fogStart;
    float3 fogColor;
    float fogRange;
};

Texture2D objTexture : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);


float4 main(PS_IN input) : SV_TARGET
{
    float4 pixelColorFromTexture = objTexture.Sample(samplerState, input.inTextureCord);
    float3 lightDiffuse = lightColor * strength;
    float diffBright = saturate(dot(input.normal, dirLightDirection.xyz));
    
    lightDiffuse += dirLightColor.xyz * diffBright;
    float3 fColor = diffuse.xyz * pixelColorFromTexture.xyz * lightDiffuse.xyz;
    
    float3 wPosToCamera = cameraPos - input.posWorld;
    float distance = length(wPosToCamera);
    wPosToCamera = wPosToCamera / distance; //Normalize
    //fog
    if(fogRange != 0)
    {
       float fogLerp = saturate((distance - fogStart) / fogRange);
       fColor = lerp(fColor, fogColor, fogLerp);
    }
    return float4(fColor.xyz, 1);
}
