struct Particle
{
    float3 Position;
    float3 Velocity;
    float Size;
    float ParticleLifetime;
    uint Type;
};

struct P_VS_OUT
{
    float3 PosW  : POSITION;
    float Size   : SIZE;
    float4 Color : COLOR;
    uint Type    : TYPE;
};

P_VS_OUT main(Particle pIN)
{
    P_VS_OUT pOUT;
    
    float t = pIN.ParticleLifetime;
    float3 Accel = { 0.0f, 7.8f, 0.0f };
    
    
    pOUT.PosW = 0.5f * t * t * Accel + t * pIN.Velocity + pIN.Position;
    float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
    pOUT.Color = float4(1.0f, 1.0f, 1.0f, opacity);
    
    pOUT.Size = pIN.Size;
    pOUT.Type = pIN.Type;
    
    return pOUT;
}