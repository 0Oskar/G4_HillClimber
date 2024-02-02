#define LIGHT_COUNT 20
struct PS_IN
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
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

cbuffer ViewProjMatrices : register(b2)
{
    matrix viewInverseMatrix;
    matrix projInverseMatrix;
};

Texture2D diffuseRT                     : TEXTURE : register(t0);
Texture2D normalRT                      : TEXTURE : register(t1);
Texture2D specularShininessRT           : TEXTURE : register(t2);
Texture2D ambientGlobalAContributionRT  : TEXTURE : register(t3);
Texture2D shadowRT                      : TEXTURE : register(t4);
Texture2D ambientOcclusionRT            : TEXTURE : register(t5);
Texture2D depthBuffer                   : TEXTURE : register(t6);
Texture2D volumetricLightingTexture     : TEXTURE : register(t7);
SamplerState samplerState               : SAMPLER : register(s0);

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

float4 main(PS_IN input) : SV_TARGET
{
    float3 diffuse = diffuseRT.Sample(samplerState, input.texcoord).rgb;
    float3 normal = normalRT.Sample(samplerState, input.texcoord).xyz;
    float4 specularShininess = specularShininessRT.Sample(samplerState, input.texcoord);
    float4 ambientGlobalAContribution = ambientGlobalAContributionRT.Sample(samplerState, input.texcoord);
    float3 shadow = shadowRT.Sample(samplerState, input.texcoord).rgb;
    float ambientOcclusion = ambientOcclusionRT.Sample(samplerState, input.texcoord).r;
    
    // Get World position from Depth
    float z = depthBuffer.Sample(samplerState, input.texcoord).r;
    float x = input.texcoord.x * 2 - 1;
    float y = (1 - input.texcoord.y) * 2 - 1;
    float4 ndcPosition = float4(x, y, z, 1.f); 
    float4 viewPosition = mul(ndcPosition, projInverseMatrix);
    viewPosition /= viewPosition.w;
    float3 worldPosition = mul(viewPosition, viewInverseMatrix).xyz;
    
    // Global Ambient
    const float3 skyColor = float3(0.25, 0.55, 0.9);
    const float3 horizonColor = float3(0.2f, 0.5f, 0.8f);
    const float3 groundColor = float3(1.0, 0.86, 0.4);
    const float skyStrength = 0.8;
    const float blendStrength = 1.0;
    
    float skyContribution = dot(float3(0.f, 1.f, 0.f), normal) * 0.3;
    skyContribution = clamp(skyContribution, 0.0, 1.0);
    float groundContribution = dot(float3(0.f, -1.f, 0.f), normal);
    groundContribution = saturate(groundContribution);
    
    float3 skyAmbientColor = (skyColor * skyContribution) + (groundColor * groundContribution);
    skyAmbientColor *= diffuse * ambientGlobalAContribution.a;
    
    // Directional Light
    float diffBright = saturate(dot(normal, skyLightDirection));
    float3 directionalLightDiffuse = ambientColor + (skyLightColor * diffBright * skyLightIntensity * shadow);
    
    float3 fColor = (diffuse * directionalLightDiffuse.xyz) + skyAmbientColor;
    
    // Points Lights
    float3 wPosToCamera = cameraPos - worldPosition;
    float distance = length(wPosToCamera);
    wPosToCamera = wPosToCamera / distance; //Normalize
    for (int i = 0; i < nrOfPointLights; i++)
    {
        fColor = saturate(fColor + pointLightCalculation(ambientGlobalAContribution.rgb, diffuse, worldPosition, normal, pointLights[i]));
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
    
        float3 skyFogColor = (skyColor * skyBlend) + (horizonColor * groundBlend) + (horizonColor * horizonBlend);
        
        float fogLerp = saturate((distance - fogStart) / fogEnd);
        fColor = lerp(fColor, skyFogColor, fogLerp);
    }
    // Volumetric Lighting
    fColor.rgb += volumetricLightingTexture.Sample(samplerState, input.texcoord).rgb;
    
    return float4(fColor, 1.f);
}