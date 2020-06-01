struct PS_IN
{
    float4 pos : SV_POSITION;
    float3 posWorld : Position;
    float3 normal : NORMAL;
    float2 inTextureCord : TEXCOORD0;
    float4 lightSpacePos : TEXCOORD1;
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
Texture2D shadowMap : TEXTURE : register(t1);

static const float SHADOW_MAP_SIZE = 2048.0f;
static const float SHADOW_MAP_TEXEL_SIZE = 1.0f / SHADOW_MAP_SIZE;

SamplerState samplerState : SAMPLER : register(s0);
SamplerComparisonState shadowSampler : register(s1);

float inShadowCheck(PS_IN input, float2 shadowTexCoords, float pixelDepth)
{
    // Use an offset value to mitigate shadow artifacts due to imprecise 
    // floating-point values (shadow acne).
    //
    // This is an approximation of epsilon * tan(acos(saturate(NdotL))):
    float3 lightPos = float3(20.0f, 15.0f, 20.0f);
    float3 lightVec = lightPos - input.posWorld;
    float lightVecDist = length(lightVec);
    lightVec = lightVec / lightVecDist;
        
    float3 diffuseValue = dot(lightVec, input.normal);
    float margin = acos(saturate(diffuseValue));
    #ifdef LINEAR
        // The offset can be slightly smaller with smoother shadow edges.
        float epsilon = 0.0005 / margin;
    #else
        float epsilon = 0.001 / margin;
    #endif
    
    // Clamp epsilon to a fixed range so it doesn't go overboard.
    epsilon = clamp(epsilon, 0, 0.1);

    // Use the SampleCmpLevelZero Texture2D method (or SampleCmp) to sample from 
    // the shadow map, just as you would with Direct3D feature level 10_0 and
    // higher.  Feature level 9_1 only supports LessOrEqual, which returns 0 if
    // the pixel is in the shadow.
    return float(shadowMap.SampleCmpLevelZero(shadowSampler, shadowTexCoords, pixelDepth + epsilon));
}

float calculateShadow(SamplerComparisonState shadowSampler, Texture2D shadowMap, float4 lightSpacePos)
{
    // Complete projection by doing division by w.
    lightSpacePos.xyz /= lightSpacePos.w;
	
	// Depth in NDC space.
    float depth = lightSpacePos.z;

    const float texelSize = SHADOW_MAP_TEXEL_SIZE;

    float percentLit = 0.0f;
    const float2 offsets[9] =
    {
        float2(-texelSize, -texelSize), float2(0.0f, -texelSize),   float2(texelSize, -texelSize),
		float2(-texelSize, 0.0f),       float2(0.0f, 0.0f),         float2(texelSize, 0.0f),
		float2(-texelSize, texelSize),  float2(0.0f, texelSize),    float2(texelSize, texelSize)
    };

	[unroll]
    for (int i = 0; i < 9; ++i)
        percentLit += shadowMap.SampleCmpLevelZero(shadowSampler, lightSpacePos.xy + offsets[i], depth).r;

    return percentLit /= 9.0f;
}

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
    
    // Shadow Mapping
    // Compute texture coordinates for the current point's location on the shadow map.
    float2 shadowTexCoords;
    shadowTexCoords.x = 0.5f + (input.lightSpacePos.x / input.lightSpacePos.w * 0.5f);
    shadowTexCoords.y = 0.5f - (input.lightSpacePos.y / input.lightSpacePos.w * 0.5f);
    float pixelDepth = input.lightSpacePos.z / input.lightSpacePos.w;

    // Check if the pixel texture coordinate is in the view frustum of the 
    // light before doing any shadow work.
    if ((saturate(shadowTexCoords.x) == shadowTexCoords.x) &&
    (saturate(shadowTexCoords.y) == shadowTexCoords.y) &&
    (pixelDepth > 0))
    {
        if (inShadowCheck(input, shadowTexCoords, pixelDepth) == 0)
            return float4(1.f, 0.f, 0.f, 1.f);
            //return float4(diffuse.xyz * pixelColorFromTexture.xyz, 1.f);
    }

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
