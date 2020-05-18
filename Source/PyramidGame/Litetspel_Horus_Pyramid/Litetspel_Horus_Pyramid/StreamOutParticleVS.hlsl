struct Particle
{
    float3 Position         : POSITION;
    float3 Velocity         : VELOCITY;
    float Size              : SIZE;
    float ParticleLifetime  : LIFETIME;
    uint Type               : TYPE;
};

Particle main(Particle pIN)
{
	return pIN;
}