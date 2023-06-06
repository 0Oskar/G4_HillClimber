#pragma once
#include <d3d11.h>
#include <SimpleMath.h>

namespace pMath
{
    using namespace DirectX::SimpleMath;
	static const float convertDegreesToRadians(float degree)
	{
		float radians;

		radians = degree / (180.0f / 3.1415926535f);

		return radians;
	}
    static float RayAABBIntersect(Vector3 rayOrigin, Vector3 rayDirection, Vector3 aabbMin, Vector3 aabbMax, float tmin, float tmax)
    {
        Vector3 invD = XMVectorReciprocal(rayDirection);
        Vector3 t0s = (aabbMin - rayOrigin) * invD;
        Vector3 t1s = (aabbMax - rayOrigin) * invD;

        Vector3 tsmaller = Vector3::Min(t0s, t1s);
        Vector3 tbigger = Vector3::Max(t0s, t1s);

        tmin = max(tmin, max(tsmaller.x, max(tsmaller.y, tsmaller.z)));
        tmax = min(tmax, min(tbigger.x, min(tbigger.y, tbigger.z)));

        return (tmin < tmax) ? max(tmin, 0.f) : -1.f;

        
    }
};
