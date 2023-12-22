struct PS_IN
{
    float4 Position : SV_POSITION;
    float4 HPosition : POSITIONT;
    float3 WPosition : POSITION;
};

cbuffer directionalLight : register(b2)
{
    float3 dirLightDirection;
    bool use_custom_horizon_color;
    float3 dirLightColor;
};

// Functions
float remap(float value, float2 inMinMax, float2 outMinMax)
{
    return outMinMax.x + (value - inMinMax.x) * (outMinMax.y - outMinMax.x) / (inMinMax.y - inMinMax.x);
}

float4 main(PS_IN input) : SV_TARGET
{
    float3 finalColor;
    
    // Sky
    const float3 skyColor = float3(0.25, 0.55, 0.9);
    const float3 horizonColor = use_custom_horizon_color ? skyColor : float3(0.89, 0.824, 0.651);
    const float3 groundColor = float3(1.0, 0.871, 0.55);
    const float skyStrength = 0.8;
    const float blendStrength = 1.0;
    
    float altitude = normalize(-1 * input.WPosition).y;
    float clampedAltitude = saturate(altitude);
    
    float skyBlend = pow(clamp(clampedAltitude, 0.0, 1.0), skyStrength);
    float groundBlend = pow(clamp(clampedAltitude, -1.0, 0.0) * -1.0, blendStrength);
    float horizonBlend = pow(1.0 - (skyBlend + groundBlend), blendStrength);
    
    finalColor = (skyColor * skyBlend) + (groundColor * groundBlend) + (horizonColor * horizonBlend);
    
    // Sun
    const float sunRadiusA = 0.02f;
    const float sunRadiusB = 0.035f;
    
    float angleToSun = saturate(dot(normalize(input.WPosition), -normalize(dirLightDirection)));
    float circleRadiusMin = min(sunRadiusA, sunRadiusB);
    circleRadiusMin *= circleRadiusMin;
    float circleRadiusMax = max(sunRadiusA, sunRadiusB);
    circleRadiusMax *= circleRadiusMax;
    
    float2 circleRadius = float2(1.f - circleRadiusMin, 1.f - circleRadiusMax);
    float circleResult = pow(saturate(remap(angleToSun, circleRadius, float2(1.f, 0.f))), 1.f);
    
    const float sunFinalStrength = 1.45f;
    float3 sunFinalColor = dirLightColor * circleResult;
    
    finalColor.rgb = (sunFinalColor * sunFinalStrength) + (finalColor.rgb * (1.f - length(sunFinalColor)));
    
    return float4(finalColor, 1.0);
}