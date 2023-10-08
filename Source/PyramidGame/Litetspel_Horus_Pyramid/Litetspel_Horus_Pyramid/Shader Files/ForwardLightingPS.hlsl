#define LIGHT_COUNT 20

struct PS_IN
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float3 positionW : POSITIONT;
    float4 positionShadow0 : POSITION0;
    float4 positionShadow1 : POSITION1;
    float4 positionShadow2 : POSITION2;
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
    float range;
    float3 diffuse;
    float pad;
};

cbuffer PerFrameBuffer : register(b1)
{
    PointLight pointLights[LIGHT_COUNT];
    int nrOfPointLights;
    float3 skyLightDirection;
    float3 skyLightColor;
    float skyLightIntensity;
    float3 cameraPos;
    float fogStart;
    float fogEnd;
    float3 ambientColor;
};

cbuffer PerFrameShadowBuffer : register(b2)
{
    bool cascadingShadowMapsToggle;
    float3 cameraPos1;
    
    float frustumCoverage0;
    float frustumCoverage1;
    float frustumCoverage2;
};

Texture2D diffuseTexture : TEXTURE : register(t0);
Texture2D shadowMap0 : TEXTURE : register(t1);
Texture2D shadowMap1 : TEXTURE : register(t2);
Texture2D shadowMap2 : TEXTURE : register(t3);
Texture2D ambientOcclusionMap : TEXTURE : register(t4);

SamplerState samplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSampler : SAMPLER : register(s1);

float3 pointLightCalculation(float3 ambient, float3 diffuse, float3 position, float3 normal, PointLight light)
{
    float3 lightVec = light.position - position;
    float3 lightDirection = normalize(lightVec);
    float diffuseValue = dot(lightDirection, normal);
    
    float3 calcDiffuse = saturate(diffuseValue) * diffuse * light.diffuse;
    
    float lightDistSq = dot(lightVec, lightVec);
    float invLightDist = rsqrt(lightDistSq);
                    
    float radiusSq = light.range * light.range;
    float distanceFalloff = radiusSq * (invLightDist * invLightDist);
    float attenuation = max(0, distanceFalloff - rsqrt(distanceFalloff));
    calcDiffuse *= attenuation;
        
    return saturate(calcDiffuse);
}

float calculateShadowFactor(Texture2D shadowMap, float4 shadowPosition)
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
    float ambientOcclusion = ambientOcclusionMap.Sample(samplerState, input.texcoord).r;
    
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
    float shadowFactor = 1.f;
    if (cascadingShadowMapsToggle)
    {
        float shadowDistance = distance(cameraPos, input.positionW);
        
        if (shadowDistance < frustumCoverage0)
        {
            shadowFactor = calculateShadowFactor(shadowMap0, input.positionShadow0);
        }
        else if (shadowDistance >= frustumCoverage0 && shadowDistance < frustumCoverage1)
        {
            shadowFactor = calculateShadowFactor(shadowMap1, input.positionShadow1);
        }
        else if (shadowDistance >= frustumCoverage1 && shadowDistance < frustumCoverage2)
        {
            shadowFactor = calculateShadowFactor(shadowMap2, input.positionShadow2);
        }
    }
    else
    {
        shadowFactor = calculateShadowFactor(shadowMap0, input.positionShadow0);
    }
    float3 directionalLightDiffuse = ambientColor + ((skyLightColor * diffBright * skyLightIntensity) * shadowFactor);
    
    float3 fColor = (diffuse * directionalLightDiffuse) + skyAmbientColor;
    
    // Points Lights
    float3 wPosToCamera = cameraPos - input.positionW;
    float distance = length(wPosToCamera);
    wPosToCamera = wPosToCamera / distance; //Normalize
    for (int i = 0; i < nrOfPointLights; i++)
    {
        fColor = saturate(fColor + pointLightCalculation(ambientM, diffuse, input.positionW, input.normal, pointLights[i]));
    }
    // Ambient Occlusion
    fColor *= ambientOcclusion;
    
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
