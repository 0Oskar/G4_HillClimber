static const int MAX_BLUR_RADIUS = 15;

Texture2D<float4> InputTex : register(t0);
RWTexture2D<float4> outputTex : register(u0);

cbuffer blurBuffer : register(b0)
{
    matrix projectionMatrix;
    int radius;
    int direction;
    float2 pad;
    float4 weights[MAX_BLUR_RADIUS / 4]; // Saved as float4 because float packing behaviour
}

[numthreads(16, 16, 1)]
void main(uint3 gruoupId : SV_GroupID, uint3 groupThredId : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchId : SV_DispatchThreadID)
{
    uint2 pixelIndex = uint2(dispatchId.x, dispatchId.y);
    float blurWeights[12] =
    {
        weights[0].x, weights[0].y, weights[0].z, weights[0].w,
        weights[1].x, weights[1].y, weights[1].z, weights[1].w,
        weights[2].x, weights[2].y, weights[2].z, weights[2].w,
    };
    // Positive and Negative directions
    int2 direction2 = int2(1 - direction, direction);
    
    float4 result = blurWeights[radius] * InputTex[pixelIndex];
    float totalWeight = blurWeights[radius];
    
    float weight = 0;
    
    for (int i = -radius; i <= radius; ++i)
    {
        if (i == 0)
            continue;
        
        weight = blurWeights[i + radius];
        result += weight * InputTex[mad(i, direction2, pixelIndex)];
        totalWeight += weight;
    }
    
    outputTex[pixelIndex] = result / totalWeight;
}