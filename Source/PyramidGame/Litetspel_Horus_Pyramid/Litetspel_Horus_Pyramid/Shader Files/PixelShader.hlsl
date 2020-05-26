struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD;
    float3 posWorld : Position;
};

struct PointLight
{
    float4 plAmbient;
    float4 plDiffuse;
    float3 plPosition;
    float plRange;
    float3 att;
    float padding;
};

cbuffer materialBuffer : register(b0)
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float shininess;
};

cbuffer lightBuffer : register(b1)
{
    float3 ambientColor;
    float strength;
    PointLight pointLights[5];
    int nrOfPointLights;
    float3 padd;
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

float4 pointLightCalculation(float4 matAmbient, float4 matDiffuse, float4 lightAmbient, float4 lightDiffuse, float3 lightPos, float lightRange, float3 pposition, float3 pnormal, float3 attenuation)
{
    float4 calcAmbient = float4(0, 0, 0, 1);
    float4 calcDiffuse = float4(0, 0, 0, 1);
    float diffuseValue = 0;
    float3 lightVec = float3(0, 0, 0);
    
    lightVec = lightPos - pposition;
    float lightVecDist = length(lightVec);
    
    if(lightVecDist <= lightRange)
    {
        lightVec = lightVec / lightVecDist;
        
        calcAmbient = matAmbient * lightAmbient;
        
        diffuseValue = dot(lightVec, pnormal);
        
        if(diffuseValue > 0.0f)
        {
            calcDiffuse = diffuseValue * matDiffuse * lightDiffuse;
        }
        
        float atten = 1.0f / dot(attenuation, float3(1.0, lightVecDist, lightVecDist * lightVecDist));
        calcDiffuse *= atten;
        
        return saturate(calcAmbient + calcDiffuse);
    }
    
    return float4(0, 0, 0, 1);
}

float4 main(PS_IN input) : SV_TARGET
{
    float4 pixelColorFromTexture = objTexture.Sample(samplerState, input.inTextureCord);
    float3 ambientclr = ambientColor * strength;
    float diffBright = saturate(dot(input.normal, dirLightDirection.xyz));
    
    float3 lightDiffuse = ambientclr + dirLightColor.xyz * diffBright;
    float4 fColor = float4(diffuse.xyz * pixelColorFromTexture.xyz * lightDiffuse.xyz, 1);
    
    float3 wPosToCamera = cameraPos - input.posWorld;
    float distance = length(wPosToCamera);
    wPosToCamera = wPosToCamera / distance; //Normalize
    for (int i = 0; i < nrOfPointLights; i++)
    {
        fColor = saturate(fColor + pointLightCalculation(ambient, diffuse, pointLights[i].plAmbient, pointLights[i].plDiffuse, pointLights[i].plPosition, pointLights[i].plRange, input.posWorld, input.normal, pointLights[i].att)); 
    }
    
    //fog
    if (fogRange != 0)
    {
        float fogLerp = saturate((distance - fogStart) / fogRange);
        fColor = float4(lerp(fColor.xyz, fogColor, fogLerp), 1);
    }
    return float4(fColor.xyz, 1);
}
