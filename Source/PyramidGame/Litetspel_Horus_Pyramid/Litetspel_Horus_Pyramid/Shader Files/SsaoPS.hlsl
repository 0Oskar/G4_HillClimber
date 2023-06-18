// Globals
static float occlusionRadius = 0.4f;
static float occlusionFadeStart = 0.2f;
static float occlusionFadeEnd = 2.0f;
static float surfaceEpsilon = 0.1f;
static const int SAMPLE_COUNT = 14;

struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 PositionV : POSITION;
    float2 TexCoord : TEXCOORD1;
};

cbuffer cameraBuffer : register(b0)
{
    matrix viewToTexMatrix;
    matrix projectionMatrix;
    matrix viewMatrix;
};

cbuffer SSAOData : register(b1)
{
    float4 sampleDirections[SAMPLE_COUNT];
    float2 ditherScale;
};

Texture2D DepthTexture : register(t0);
Texture2D NormalTexture : register(t1);
Texture2D<float3> DitherTexture : register(t2);

SamplerState depthNormalSampler : register(s3);
SamplerState randomSampler : register(s4);

float ndcDepthToViewDepth(float depth)
{
    return projectionMatrix[3][2] / (depth - projectionMatrix[2][2]);
}

float occlusionFunction(float distZ)
{
    float occlusion = 0.f;
    if (distZ > surfaceEpsilon)
    {
        float fadeLength = occlusionFadeEnd - occlusionFadeStart;
        occlusion = saturate((occlusionFadeEnd - distZ) / fadeLength);
    }
    return occlusion;
}

float4 main(PS_IN input) : SV_TARGET
{
    // Sample Normal
    float3 n = normalize(NormalTexture.SampleLevel(depthNormalSampler, input.TexCoord, 0.0f).xyz);
    n = mul(n, (float3x3) viewMatrix); // Convert from World to View space
    
    // Sample Depth texture to get Position
    float pz = ndcDepthToViewDepth(DepthTexture.SampleLevel(depthNormalSampler, input.TexCoord, 0.0f).r);
    float3 p = (pz / input.PositionV.z) * input.PositionV.xyz;
    
    // Get Random Vector
    float3 randVec = DitherTexture.SampleLevel(randomSampler, input.TexCoord * ditherScale, 0).rgb; // Dither Texture
    
    float occlusionSum = 0.0f;
    
    [unroll]
    for (int i = 0; i < SAMPLE_COUNT; ++i)
    {
        float3 offset = reflect(sampleDirections[i].xyz, randVec);
    
        float flip = sign(dot(offset, n));
        
        float3 q = p + flip * occlusionRadius * offset;
        
        float4 projQ = mul(float4(q, 1.0f), viewToTexMatrix);
        projQ /= projQ.w;

        float rz = ndcDepthToViewDepth(DepthTexture.SampleLevel(depthNormalSampler, projQ.xy, 0.0f).r);
        float3 r = (rz / q.z) * q;
        
        float distZ = p.z - r.z;
        float dp = max(dot(n, normalize(r - p)), 0.0f);
        float occlusion = dp * occlusionFunction(distZ);
        
        occlusionSum += occlusion;
    }
    
    occlusionSum /= SAMPLE_COUNT;
    
    float access = 1.0f - occlusionSum;
    float ao = saturate(pow(access, 2.f));
    return float4(ao, ao, ao, 1.f);
}