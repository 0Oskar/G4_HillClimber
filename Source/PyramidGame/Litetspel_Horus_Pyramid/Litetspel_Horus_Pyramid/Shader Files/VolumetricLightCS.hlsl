// Globals
static const float PI = 3.14159265359;
static const float PI_RCP = 0.31830988618379067153776752674503f;
static const float LIGHT_SCATTERING_G = -0.5f;
static const int RAY_STEPS = 128;
static const float2 CULL_UV_OFFSET = float2(0.1f, 0.1f);
#define CASCADING_LIGHT_COUNT 3

static const float ditherPattern[16] =
{
    0.0f, 0.5f, 0.125f, 0.625f,
    0.75f, 0.22f, 0.875f, 0.375f,
    0.1875f, 0.6875f, 0.0625f, 0.5625,
    0.9375f, 0.4375f, 0.8125f, 0.3125
};

struct LightRay
{
    float3 startPosition;
    float3 endPosition;
    float3 delta;
    float raymarchDistance;
    float stepSize;
};

// Constant Buffers
cbuffer CameraCB : register(b0)
{
    matrix viewInverseMatrix;
    matrix projInverseMatrix;
    float3 cameraPosition;
    float pad;
};

cbuffer shadowCB : register(b1)
{
    matrix lightViewMatrix[CASCADING_LIGHT_COUNT];
    matrix lightProjectionMatrix[CASCADING_LIGHT_COUNT];
    float frustumCoverage[CASCADING_LIGHT_COUNT];
    uint shadowTextureSize;
};

cbuffer SkyLightDataCB : register(b2)
{
    float3 skyLightDirection;
    float skyLightIntensity;
    float3 skyLightColor;
    float2 textureStep;
    float3 pad2;
};

// Textures
RWTexture2D<float4> OutputTexture : register(u0);
Texture2D<float> DepthTexture : register(t0);
Texture2D<float> ShadowTexture : register(t1);

// Functions
float2 getTexcoord(uint2 id)
{
    return (float2) id * textureStep;
}

// Mie scaterring approximated with Henyey-Greenstein phase function.
float ComputeScattering(float lightDotView)
{
    float result = 1.0f - LIGHT_SCATTERING_G * LIGHT_SCATTERING_G;
    result /= (4.0f * PI * pow(1.0f + LIGHT_SCATTERING_G * LIGHT_SCATTERING_G - (2.0f * LIGHT_SCATTERING_G) * lightDotView, 1.5f));
    return result;
}

// Main
[numthreads(16, 16, 1)]
void main(uint3 groupId : SV_GroupID, uint3 groupThreadId : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchId : SV_DispatchThreadID)
{
    uint2 pixelIndex = uint2(dispatchId.x, dispatchId.y);
    float2 uv = getTexcoord(pixelIndex);
    float4 finalColor = (float4) 0;
    
    // Position
    float z = DepthTexture.Load(int3(pixelIndex, 0)).r;
    float x = uv.x * 2.f - 1.f;
    float y = (1.f - uv.y) * 2.f - 1.f;
    float4 clipPosition = float4(x, y, clamp(z, 0.5f, 1.f), 1.f);
    float4 startViewPosition = mul(clipPosition, projInverseMatrix);
    float4 endViewPosition = mul(float4(x, y, 0.f, 1.f), projInverseMatrix);
    
    startViewPosition /= startViewPosition.w;
    endViewPosition /= endViewPosition.w;
    
    float ditherValue = ditherPattern[(clipPosition.x % 4.f) * 4.f + (clipPosition.y % 4.f)];
    
    // Worldspace
    float3 startPositionW = mul(startViewPosition, viewInverseMatrix).xyz;
    float3 endPositionW = mul(endViewPosition, viewInverseMatrix).xyz;
    float3 deltaW = normalize(endPositionW - startPositionW);
    
    float raymarchDistanceW = length(endPositionW - startPositionW);
    float stepSizeW = raymarchDistanceW / RAY_STEPS;
    float4 rayPositionW = float4(startPositionW + ditherValue * stepSizeW * deltaW, 1.f);
    
    // Lightspace
    float3 startPositionL = mul(float4(startPositionW, 1.f), lightViewMatrix[2]).xyz;
    float3 endPositionL = mul(float4(endPositionW, 1.f), lightViewMatrix[2]).xyz;
    float3 deltaL = normalize(endPositionL - startPositionL);
    float raymarchDistanceL = length(endPositionL - startPositionL);
    float stepSizeL = raymarchDistanceL / RAY_STEPS;
    
    //LightRay lr[CASCADING_LIGHT_COUNT];
    //[unroll]
    //for (uint i = 0; i < CASCADING_LIGHT_COUNT; i++)
    //{
    //    lr[i].startPosition = mul(float4(startPositionW, 1.f), lightViewMatrix[i]).xyz;
    //    lr[i].endPosition = mul(float4(endPositionW, 1.f), lightViewMatrix[i]).xyz;
    //    lr[i].delta = normalize(lr[i].endPosition - lr[i].startPosition);
    //    lr[i].raymarchDistance = length(lr[i].endPosition - lr[i].startPosition);
    //    lr[i].stepSize = lr[i].raymarchDistance / RAY_STEPS;
    //}
    
    //OutputTexture[pixelIndex] = float4(startPositionL.xyz, 1.f);
    //return;
    
    float4 rayPositionL = float4(startPositionL + ditherValue * stepSizeL * deltaL, 1.f);
    
    //float4 rayPositionLightClipspace = mul(float4(rayPositionL.xyz, 1.f), lightProjectionMatrix);
    //float2 shadowUV = rayPositionLightClipspace.xy / rayPositionLightClipspace.w * 0.5f + 0.5f;
    //shadowUV.y = 1.0f - shadowUV.y;
    //float shadowDepth = rayPositionLightClipspace.z / rayPositionLightClipspace.w;
    
    //float shadowTerm = ShadowTexture.Load(int3(shadowUV * shadowTextureSize, 0));
    //OutputTexture[pixelIndex] = float4(shadowTerm, shadowTerm, shadowTerm, 1.f);
    //return;
    
    // Light Values
    float tau = 0.0001f;
    float intensity = 100000000.f; //1000000.f; //skyLightIntensity
    float4 lightPosition = float4(skyLightDirection * 5000.f, 1.f);
    
    // Raymarch loop
    matrix lPMat;
    float2 textureOffset;
    float lightContribution = 0.f;
    for (float l = raymarchDistanceW; l > stepSizeW; l -= stepSizeW)
    {
        // Shadow positioning
        float shadowDistance = distance(cameraPosition, rayPositionW.xyz);
    
        uint shadowIndex = 0;
        //if (shadowDistance < frustumCoverage[0])
        //{
        //    shadowIndex = 0;
        //}
        //else if (shadowDistance >= frustumCoverage[0] && shadowDistance < frustumCoverage[1])
        //{
        //    shadowIndex = 1;
        //}
        //else if (shadowDistance >= frustumCoverage[1] && shadowDistance < frustumCoverage[2])
        //{
        //    shadowIndex = 2;
        //}
        //shadowIndex = step(frustumCoverage[0], shadowDistance) - step(frustumCoverage[1], shadowDistance);
        
        lPMat = lightProjectionMatrix[shadowIndex];
        textureOffset = float2(shadowTextureSize * shadowIndex, 0);
        
        float4 rayPositionLightClipspace = mul(float4(rayPositionL.xyz, 1.f), lPMat);
        float2 shadowUV = rayPositionLightClipspace.xy / rayPositionLightClipspace.w * 0.5f + 0.5f;
        shadowUV.y = 1.0f - shadowUV.y;
        float shadowPositionDepth = rayPositionLightClipspace.z / rayPositionLightClipspace.w;
        
        // check if in shadow or not, and set shadow_term accordingly
        float shadowMapDepth = 1.f;
        if (!(shadowUV.x < 0 || shadowUV.x > 1.f || shadowUV.y < 0 || shadowUV.y > 1.f))
        {
            shadowMapDepth = ShadowTexture.Load(int3((shadowUV * shadowTextureSize) * textureOffset, 0))
            -0.001f;
        }
		
        float shadowTerm = 1.0;
        if (shadowPositionDepth > shadowMapDepth) {
            shadowTerm = 0.0;
        }
        // get distance in world space
        float d = length(rayPositionW - lightPosition);
        // multiplication is faster than division, so do it once
        float dRcp = 1.f / d;
    
        // accumulate to light contribution based on approximation of radiative transport equation
        lightContribution += tau * (shadowTerm * (intensity * 0.25 * PI_RCP) * dRcp * dRcp) *
        exp(-d * tau) * exp(-l * tau) * stepSizeW;
        
        // continue ray marching
        rayPositionW += stepSizeW * float4(deltaW, 0.f);
        rayPositionL += stepSizeL * float4(deltaL, 0.f);
    }
    
    OutputTexture[pixelIndex] = float4((pow(lightContribution, 0.2f) / 1.5f) * skyLightColor, 1.f);
}