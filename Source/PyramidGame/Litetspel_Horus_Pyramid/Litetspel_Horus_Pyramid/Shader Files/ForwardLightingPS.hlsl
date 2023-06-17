struct PS_IN
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 positionW : POSITION;
    float4 positionShadow : POSITION1;
};

cbuffer materialBuffer : register(b0)
{
    float3 ambientM;
    float globalAmbientContributionM;
    float4 diffuseM;
    float4 specularM;
    float shininessM;
};

struct PointLight
{
    float3 position;
    float3 diffuse;
    float range;
};

cbuffer PerFrameBuffer : register(b1)
{
    PointLight pointLights[20];
    int nrOfPointLights;
    float3 skyLightDirection;
    float3 skyLightColor;
    float skyLightIntensity;
    float3 cameraPos;
    float fogStart;
    float fogEnd;
    float3 ambientColor;
};

Texture2D diffuseTexture : TEXTURE : register(t0);
Texture2D shadowMap : TEXTURE : register(t1);

SamplerState samplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSampler : SAMPLER : register(s1);

float3 pointLightCalculation(float3 ambient, float3 diffuse, float3 position, float3 normal, PointLight light)
{
    float3 calcAmbient = float3(0, 0, 0);
    float3 calcDiffuse = float3(0, 0, 0);
    float diffuseValue = 0;
    float3 lightVec = float3(0, 0, 0);
    
    lightVec = light.position - position;
    float lightVecDist = length(lightVec);
    
    if (lightVecDist <= light.range)
    {
        float lightDistSq = dot(lightVec, lightVec);
        float invLightDist = rsqrt(lightDistSq);
                    
        float radiusSq = light.range * light.range;
        float distanceFalloff = radiusSq * (invLightDist * invLightDist);
        float attenuation = max(0, distanceFalloff - rsqrt(distanceFalloff));
    
        lightVec = lightVec / lightVecDist;
        calcAmbient = ambient * light.diffuse;
        diffuseValue = dot(lightVec, normal);
        
        if (diffuseValue > 0.0f)
        {
            calcDiffuse = diffuseValue * diffuse * light.diffuse;
        }
        
        //float atten = 1.0f / dot(attenuation, float3(1.0, lightVecDist, lightVecDist * lightVecDist));
        calcDiffuse *= attenuation;
        
        return saturate(calcAmbient + calcDiffuse);
    }
    
    return float3(0, 0, 0);
}

float calculateShadowFactor(float4 shadowPosition)
{
    float2 shadowUV = shadowPosition.xy / shadowPosition.w * 0.5f + 0.5f;
    shadowUV.y = 1.0f - shadowUV.y;
    float shadowDepth = shadowPosition.z / shadowPosition.w;

    float shadowFactor = 0;
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
    return shadowFactor;
}

float4 main(PS_IN input) : SV_TARGET
{
    float3 diffuse = diffuseM.rgb * diffuseTexture.Sample(samplerState, input.texcoord).rgb;
    
    // Global Ambient
    const float3 skyColor = float3(0.25, 0.55, 0.9);
    const float3 horizonColor = float3(0.89, 0.824, 0.651);
    const float3 groundColor = float3(1.0, 0.871, 0.55);
    const float skyStrength = 0.8;
    const float blendStrength = 1.0;
    
    float skyContribution = dot(float3(0.f, 1.f, 0.f), input.normal) * 0.3;
    skyContribution = clamp(skyContribution, 0.0, 1.0);
    float groundContribution = dot(float3(0.f, -1.f, 0.f), input.normal);
    groundContribution = saturate(groundContribution);
    
    float3 skyAmbientColor = (skyColor * skyContribution) + (groundColor * groundContribution);
    skyAmbientColor *= diffuse * globalAmbientContributionM;
    
    // Directional Light
    float diffBright = saturate(dot(input.normal, skyLightDirection));
    float3 directionalLightDiffuse = ambientColor + ((skyLightColor * diffBright * skyLightIntensity) * calculateShadowFactor(input.positionShadow));
    
    float4 fColor = float4((diffuse * directionalLightDiffuse) + skyAmbientColor, 1.0);
    
    float3 wPosToCamera = cameraPos - input.positionW;
    float distance = length(wPosToCamera);
    wPosToCamera = wPosToCamera / distance; //Normalize
    for (int i = 0; i < nrOfPointLights; i++)
    {
        fColor = float4(saturate(fColor.rgb + pointLightCalculation(ambientM, diffuse, input.positionW, input.normal, pointLights[i])), 1.f);
    }
    
    // Fog
    if (fogEnd != 0)
    {
        float3 direction = normalize(-wPosToCamera);
        float altitude = direction.y;
        float clampedAltitude = saturate(altitude);
    
        float skyBlend = pow(clamp(clampedAltitude, 0.0, 1.0), skyStrength);
        float groundBlend = pow(clamp(clampedAltitude, -1.0, 0.0) * -1.0, blendStrength);
        float horizonBlend = pow(1.0 - (skyBlend + groundBlend), blendStrength);
    
        float3 skyFogColor = (skyColor * skyBlend) + (groundColor * groundBlend) + (horizonColor * horizonBlend);
        
        float fogLerp = saturate((distance - fogStart) / fogEnd);
        fColor = float4(lerp(fColor.xyz, skyFogColor * diffuseM.a, fogLerp), 1);
    }
    return float4(fColor.xyz, diffuseM.a);
}
