struct Particle
{
    float3 Position;
    float3 Velocity;
    float Size;
    float ParticleLifetime;
    uint Type;
};

Particle main(Particle pIN)
{
	return pIN;
}