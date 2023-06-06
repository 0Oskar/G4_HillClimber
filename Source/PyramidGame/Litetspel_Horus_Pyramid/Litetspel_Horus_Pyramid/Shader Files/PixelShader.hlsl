struct PS_IN
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 positionW : POSITION;
    float4 positionShadow : POSITION1;
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
    float3 ambientM;
    float globalAmbientContributionM;
    float4 diffuseM;
    float4 specularM;
    float shininessM;
};

cbuffer lightBuffer : register(b1)
{
    float3 directionalAmbientColor;
    float directionalAmbientStrength;
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


Texture2D diffuseTexture : TEXTURE : register(t0);
Texture2D shadowMap : TEXTURE : register(t1);

SamplerState samplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSampler : SAMPLER : register(s1);

float3 pointLightCalculation(float3 matAmbient, float4 matDiffuse, float3 lightAmbient, float4 lightDiffuse, float3 lightPos, float lightRange, float3 pposition, float3 pnormal, float3 attenuation)
{
    float3 calcAmbient = float3(0, 0, 0);
    float3 calcDiffuse = float3(0, 0, 0);
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
            calcDiffuse = diffuseValue * matDiffuse.xyz * lightDiffuse.xyz;
        }
        
        float atten = 1.0f / dot(attenuation, float3(1.0, lightVecDist, lightVecDist * lightVecDist));
        calcDiffuse *= atten;
        
        return saturate(calcAmbient + calcDiffuse);
    }
    
    return float3(0, 0, 0);
}

float calculateShadowFactor(float4 shadowPosition)
{
    // Shadow Mapping
    float2 shadowUV = shadowPosition.xy / shadowPosition.w * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;

    float shadowDepth = shadowPosition.z / shadowPosition.w;

    float shadowFactor = 0;
    //[flatten]
    //if (shadowDepth < 0.f || shadowDepth > 1.f) // if the pixels depth is beyond the shadow map, skip shadow sampling(Pixel is lit by light)
    //{
    //    shadowFactor = 1.f;
    //}
    //else
    {
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
    }
    return shadowFactor;
}

float4 main(PS_IN input) : SV_TARGET
{
    float4 diffuse = diffuseM * diffuseTexture.Sample(samplerState, input.texcoord);
    
    // Global Ambient
    const float3 skyColor = float3(0.25, 0.55, 0.9);
    const float3 horizonColor = float3(0.89, 0.824, 0.651);
    const float3 groundColor = float3(1.0, 0.871, 0.55);
    const float skyStrength = 0.5;
    const float blendStrength = 1.0;
    
    float skyContribution = dot(float3(0.f, 1.f, 0.f), input.normal) * 0.3;
    skyContribution = clamp(skyContribution, 0.0, 1.0);
    float groundContribution = dot(float3(0.f, -1.f, 0.f), input.normal);
    groundContribution = saturate(groundContribution);
    
    float3 skyAmbientColor = (skyColor * skyContribution) + (groundColor * groundContribution);
    skyAmbientColor *= diffuse.rgb * globalAmbientContributionM;
    
    // Directional Light
    float3 ambientDirectionalLight = directionalAmbientColor * directionalAmbientStrength;
    float diffBright = saturate(dot(input.normal, dirLightDirection.xyz));
    float3 directionalLightDiffuse = ambientDirectionalLight + ((dirLightColor.xyz * diffBright) * calculateShadowFactor(input.positionShadow));
    
    float4 fColor = float4((diffuse.xyz * directionalLightDiffuse.xyz) + skyAmbientColor, 1.0);
    
    float3 wPosToCamera = cameraPos - input.positionW;
    float distance = length(wPosToCamera);
    wPosToCamera = wPosToCamera / distance; //Normalize
    for (int i = 0; i < nrOfPointLights; i++)
    {
        fColor = float4(saturate(fColor.rgb + pointLightCalculation(ambientM, diffuseM, pointLights[i].plAmbient.rgb, pointLights[i].plDiffuse, pointLights[i].plPosition, pointLights[i].plRange, input.positionW, input.normal, pointLights[i].att)), 1.f);
    }
    
    // Fog
    if (fogRange != 0)
    {
        float height = normalize(input.positionW).y;
        float skyBlend = pow(clamp(height, 0.0, 1.0), skyStrength);
        float groundBlend = pow(clamp(height, -1.0, 0.0) * -1.0, blendStrength);
        float horizonBlend = pow(1.0 - (skyBlend + groundBlend), blendStrength);
    
        float3 skyFogColor = (skyColor * skyBlend) + (groundColor * groundBlend) + (horizonColor * horizonBlend);
        
        float fogLerp = saturate((distance - fogStart) / fogRange);
        fColor = float4(lerp(fColor.xyz, skyFogColor, fogLerp), 1);
    }
    return float4(fColor.xyz, 1.0);
}
