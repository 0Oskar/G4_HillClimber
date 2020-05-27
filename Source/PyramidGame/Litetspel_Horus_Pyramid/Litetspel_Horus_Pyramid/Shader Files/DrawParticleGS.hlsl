#define PT_EMITTER 0
#define PT_FLARE 1

struct P_VS_OUT
{
    float3 PosW : POSITION;
    float Size : SIZE;
    float4 Color : COLOR;
    uint Type : TYPE;
};

struct P_GS_OUT
{
    float4 PosH : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

cbuffer GS_PARTICLE_BUFFER : register(b0)
{
    float3 CamPosition;

    float3 EmitPosition;
    float3 EmitDirection;

    float GameTime;
    float TimeStep;
    matrix ViewProjMatrix;

};

float2 QuadTex[4] =
{
    float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f)
};

[maxvertexcount(4)]
void main(point P_VS_OUT gIN[1], inout TriangleStream<P_GS_OUT> triStream
)
{
    if (gIN[0].Type != PT_EMITTER)
    {
        
        float3 look  = normalize(CamPosition.xyz - gIN[0].PosW);
        float3 right = normalize(cross(float3(0, 1, 0), look));
        float3 up    = cross(look, right);
        
        float halfWidth  = 0.5 * gIN[0].Size; // x and y if where float2
        float halfHeight = 0.5 * gIN[0].Size;
        
        float4 v[4];
        v[0] = float4(gIN[0].PosW + halfWidth * right - halfHeight * up, 1.0f);
        v[1] = float4(gIN[0].PosW + halfWidth * right + halfHeight * up, 1.0f);
        v[2] = float4(gIN[0].PosW - halfWidth * right - halfHeight * up, 1.0f);
        v[3] = float4(gIN[0].PosW - halfWidth * right + halfHeight * up, 1.0f);
        
        P_GS_OUT gOUT;
        [unroll]
        for (int i = 0; i < 4; ++i)
        {
            gOUT.PosH  = mul(v[i], ViewProjMatrix);
            gOUT.Tex   = QuadTex[i];
            gOUT.Color = gIN[0].Color;
            triStream.Append(gOUT);
        }          
    }
}