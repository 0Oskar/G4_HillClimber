#define PT_EMITTER 0
#define PT_FLARE 1

struct Particle
{
    float3 Position;
    float3 Velocity;
    float Size;
    float ParticleLifetime;
    uint Type;
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

Texture1D RandomTex;

SamplerState sampLinear
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

float3 RandUnitVec3(float offset)
{
	// Use game time plus offset to sample random texture.
    float u = (GameTime + offset);
	
	// coordinates in [-1,1]
    float3 v = RandomTex.SampleLevel(sampLinear, u, 0).xyz;
	
	// project onto unit sphere
    return normalize(v);
}

struct GSOutput
{
	float4 pos : SV_POSITION;
};

[maxvertexcount(2)]
void main(Particle pIN[1], inout PointStream<Particle> pOutput)
{
	//for (uint i = 0; i < 3; i++)
	//{
        pIN[0].ParticleLifetime += TimeStep;
        
        if (pIN[0].Type == PT_EMITTER)
        {
            if(pIN[0].ParticleLifetime > 0.005)
            {
                float3 velocityRand = RandUnitVec3(0.0f);
                velocityRand.x *= 0.5f;
                velocityRand.z *= 0.5f;
                
                Particle p;
                p.Position         = EmitPosition.xyz;
                p.Velocity         = 4.0f * velocityRand;
                p.Size             = float2(3.0f, 3.0f);
                p.ParticleLifetime = 0.0f;
                p.Type             = PT_FLARE;
                
                pOutput.Append(p);
                
                pIN[0].ParticleLifetime = 0.0f;
            }
            
            pOutput.Append(pIN[0]);
        }
        else
        {
            if (pIN[0].ParticleLifetime <= 1.0f)
            {
                pOutput.Append(pIN[0]);
            }
        }
			
	//}
}