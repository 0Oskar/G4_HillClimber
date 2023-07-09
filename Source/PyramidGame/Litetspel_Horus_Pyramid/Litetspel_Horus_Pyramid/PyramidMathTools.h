#pragma once

#include <stdlib.h>
#include <ctime>
#include <d3d11.h>
#include <SimpleMath.h>

namespace pMath
{
    using namespace DirectX;
    using namespace SimpleMath;
    static float frand(float low, float high)
    {
        return low + static_cast<float> (rand()) / (static_cast<float> (RAND_MAX / (high - low)));
    }

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

	static Vector3 Transform(const Vector3& point, const XMMATRIX& matrix)
	{
		Vector3 result;
		Vector4 temp(point.x, point.y, point.z, 1); //need a 4-part vector in order to multiply by a 4x4 matrix
		Vector4 temp2;

		temp2.x = temp.x * XMVectorGetByIndex(matrix.r[0], 0) + temp.y * XMVectorGetByIndex(matrix.r[1], 0) + temp.z * XMVectorGetByIndex(matrix.r[2], 0) + temp.w * XMVectorGetByIndex(matrix.r[3], 0);
		temp2.y = temp.x * XMVectorGetByIndex(matrix.r[0], 1) + temp.y * XMVectorGetByIndex(matrix.r[1], 1) + temp.z * XMVectorGetByIndex(matrix.r[2], 1) + temp.w * XMVectorGetByIndex(matrix.r[3], 1);
		temp2.z = temp.x * XMVectorGetByIndex(matrix.r[0], 2) + temp.y * XMVectorGetByIndex(matrix.r[1], 2) + temp.z * XMVectorGetByIndex(matrix.r[2], 2) + temp.w * XMVectorGetByIndex(matrix.r[3], 2);
		temp2.w = temp.x * XMVectorGetByIndex(matrix.r[0], 3) + temp.y * XMVectorGetByIndex(matrix.r[1], 3) + temp.z * XMVectorGetByIndex(matrix.r[2], 3) + temp.w * XMVectorGetByIndex(matrix.r[3], 3);

		result.x = temp2.x / temp2.w; //view projection matrices make use of the W component
		result.y = temp2.y / temp2.w;
		result.z = temp2.z / temp2.w;

		return result;
	}

	static Vector3 TransformTransposed(const Vector3& point, const XMMATRIX& matrix)
	{
		Vector3 result;
		Vector4 temp(point.x, point.y, point.z, 1); //need a 4-part vector in order to multiply by a 4x4 matrix
		Vector4 temp2;

		temp2.x = temp.x * XMVectorGetByIndex(matrix.r[0], 0) + temp.y * XMVectorGetByIndex(matrix.r[0], 1) + temp.z * XMVectorGetByIndex(matrix.r[0], 2) + temp.w * XMVectorGetByIndex(matrix.r[0], 3);
		temp2.y = temp.x * XMVectorGetByIndex(matrix.r[1], 0) + temp.y * XMVectorGetByIndex(matrix.r[1], 1) + temp.z * XMVectorGetByIndex(matrix.r[1], 2) + temp.w * XMVectorGetByIndex(matrix.r[1], 3);
		temp2.z = temp.x * XMVectorGetByIndex(matrix.r[2], 0) + temp.y * XMVectorGetByIndex(matrix.r[2], 1) + temp.z * XMVectorGetByIndex(matrix.r[2], 2) + temp.w * XMVectorGetByIndex(matrix.r[2], 3);
		temp2.w = temp.x * XMVectorGetByIndex(matrix.r[3], 0) + temp.y * XMVectorGetByIndex(matrix.r[3], 1) + temp.z * XMVectorGetByIndex(matrix.r[3], 2) + temp.w * XMVectorGetByIndex(matrix.r[3], 3);

		result.x = temp2.x / temp2.w;	//view projection matrices make use of the W component
		result.y = temp2.y / temp2.w;
		result.z = temp2.z / temp2.w;

		return result;
	}

	static XMFLOAT3 F3Min(const XMFLOAT3& vec0, const XMFLOAT3& vec1)
	{
		return XMFLOAT3(min(vec0.x, vec1.x), min(vec0.y, vec1.y), min(vec0.z, vec1.z));
	}

	static XMFLOAT3 F3Max(const XMFLOAT3& vec0, const XMFLOAT3& vec1)
	{
		return XMFLOAT3(max(vec0.x, vec1.x), max(vec0.y, vec1.y), max(vec0.z, vec1.z));
	}

	static bool F3IsNaN(const XMFLOAT3& vec)
	{
		return isnan(vec.x) && isnan(vec.y) && isnan(vec.z);
	}

	static XMFLOAT3 F3Subtract(const XMFLOAT3& vec0, const XMFLOAT3& vec1)
	{
		return XMFLOAT3(vec0.x - vec1.x, vec0.y - vec1.y, vec0.z - vec1.z);
	}
	static XMFLOAT3 F3Add(const XMFLOAT3& vec0, const XMFLOAT3& vec1)
	{
		return XMFLOAT3(vec0.x + vec1.x, vec0.y + vec1.y, vec0.z + vec1.z);
	}
	static XMFLOAT3 F3Divide(const XMFLOAT3& vec, float divide)
	{
		return XMFLOAT3(vec.x / divide, vec.y / divide, vec.z / divide);
	}
	static XMFLOAT3 F3Multiply(const XMFLOAT3& vec, float multiply)
	{
		return XMFLOAT3(vec.x * multiply, vec.y * multiply, vec.z * multiply);
	}
	static XMFLOAT3 F3MultiplyPerComponent(const XMFLOAT3& vec, const XMFLOAT3& multiply)
	{
		return XMFLOAT3(vec.x * multiply.x, vec.y * multiply.y, vec.z * multiply.z);
	}
};
