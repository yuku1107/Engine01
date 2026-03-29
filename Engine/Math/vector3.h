/*==============================================================================

    ベクトルクラス[vector3.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/13
==============================================================================*/
#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class Vector3
{
public:

	float x, y, z;

	//コンストラクタ
	Vector3(){}
	//コピーコンストラクター
	Vector3(const Vector3 &a):x(a.x),y(a.y),z(a.z){}
	//3つの値で作成する
	Vector3(float nx,float ny,float nz):x(nx),y(ny),z(nz){}

	//代入
	Vector3& operator=(const Vector3& a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

	//等しさのチェック
	bool operator==(const Vector3& a) const
	{
		return x == a.x && y == a.y && z == a.z;
	}

	bool operator !=(const Vector3& a) const
	{
		return x != a.x || y != a.y || z != a.z;
	}

	//ベクトル操作
	
	//ベクトルを0に設定
	void zero()
	{
		x = y = z = 0;
	}

	//単項式のマイナスは、反転したベクトルを返す
	Vector3 operator -() const
	{
		return Vector3{ -x,-y,-z };
	}

	//二項式の+と-はベクトルを加算し、減算する
	Vector3 operator +(const Vector3& a) const
	{
		return Vector3{ x + a.x,y + a.y,z + a.z };
	}

	Vector3 operator -(const Vector3& a) const
	{
		return Vector3{ x - a.x,y - a.y,z - a.z };
	}

	//スカラ－による乗算と除算
	Vector3 operator *(float a) const
	{
		return Vector3{ x * a,y * a,z * a };
	}

	Vector3 operator *(const Vector3& a) const
	{
		return Vector3{ x * a.x,y * a.y,z * a.z };
	}

	Vector3 operator /(float a) const
	{
		float oneOverA = 1.0f / a;
		return Vector3{ x * oneOverA,y * oneOverA,z * oneOverA };
	}

	//Cの表記法に基準するための組み合わせ代入演算

	Vector3& operator +=(const Vector3& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	Vector3& operator -=(const Vector3& a)
	{
		x -= a.x;
		y -= a.y;
		z -= a.z;
		return *this;
	}

	Vector3& operator *=(const Vector3& a)
	{
		x *= a.x;
		y *= a.y;
		z *= a.z;
		return *this;
	}

	Vector3& operator *=(float a)
	{
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	Vector3& operator /=(float a)
	{
		float oneOverA = 1.0f / a;
		x *= oneOverA;
		y *= oneOverA;
		z *= oneOverA;
		return *this;
	}

	//ベクトルを正規化する
	void normalize()
	{
		float magSq = x * x + y * y + z * z;
		if (magSq > 0.0f)
		{
			float oneOverMag = 1.0f / (float)sqrt(magSq);
			x *= oneOverMag;
			y *= oneOverMag;
			z *= oneOverMag;
		}
	}

	//正規化されたベクトルを返す
	Vector3 normalized() const
	{
		float magSq = x * x + y * y + z * z;
		if (magSq > 0.0f)
		{
			float oneOverMag = 1.0f / (float)sqrt(magSq);
			return Vector3{ x * oneOverMag,y * oneOverMag,z * oneOverMag };
		}
		else
		{
			return Vector3{ 0,0,0 };
		}
	}

	//ベクトルの長さを計算する
	float length() const
	{
		return (float)sqrt(x * x + y * y + z * z);
	}

	//ベクトルの長さの2乗
	float lengthSq()const
	{
		return (float)(x * x + y * y + z * z);
	}

	//ベクトルの内積
	float  dot(const Vector3& a) const
	{
		return x * a.x + y * a.y + z * a.z;
	}

	//ベクトルの外積
	Vector3  cross(const Vector3& a) const
	{
		return Vector3{ y * a.z - z * a.y,z * a.x - x * a.z,x * a.y - y * a.x };
	}

	Vector3 abs() const
	{
		return Vector3(fabsf(x), fabsf(y), fabsf(z));
	}
	
	//インデックスアクセス
	const float& operator[](int index) const 
	{
		assert(index >= 0 && index < 3);
		return *((&x) + index);
	}

	XMFLOAT3 ToXMFLOAT3() const
	{
		return XMFLOAT3(x, y, z);
	}

	XMVECTOR ToXMVECTOR() const
	{
		return XMVectorSet(x, y, z, 0.0f);
	}

	XMVECTOR ToQuaternion()const
	{
		return XMQuaternionRotationRollPitchYaw(x, y, z);
	}


	Vector3 Zero()const
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	//投影
	static Vector3 ProjectOnPlane(const Vector3& vector, const Vector3& normal)
	{
		return vector - normal * vector.dot(normal);
	}

	//ローカル座標 → ワールド座標
	inline Vector3 Transform(const XMMATRIX& mat) const
	{
		XMVECTOR v = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR r = XMVector3TransformCoord(v, mat);

		Vector3 out;
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&out), r);
		return out;
	}
};

//二つのベクトルの外積を計算する
inline Vector3 crossProduct(const Vector3& a, const Vector3& b)
{
	return Vector3{ a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x };
}

//対称性のために、左からスカラ－を乗算する
inline Vector3 operator *(float k, const Vector3& v)
{
	return Vector3(k * v.x, k * v.y, k * v.z);
}

//２つ点の距離を計算する
inline float distance(const Vector3& a, const Vector3& b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float dz = a.z - b.z;
	return (float)sqrt(dx * dx + dy * dy + dz * dz);
}

inline Vector3 XMFLOAT3ToVector3(XMFLOAT3 vector)
{
	return Vector3(vector.x, vector.y, vector.z);
}

inline Vector3 XMVECTORToVector3(XMVECTOR vector)
{
	XMFLOAT3 vector_float3;

	XMStoreFloat3(&vector_float3, vector);

	return Vector3(vector_float3.x, vector_float3.y, vector_float3.z);
}

template <typename T>
T Lerp(const T& a, const T& b, float t)
{
	return a + (b - a) * t;
}

template <typename T>
T Max(const T& val1, const T& val2)
{
	if (val1 > val2)
	{
		return val1;
	}

	return val2;
}

template <typename T>
T Clamp(const T& val, const T& minVal, const T& maxVal)
{
	if (val < minVal)
	{
		return minVal;
	}

	if (val > maxVal)
	{
		return maxVal;
	}

	return val;
}


#endif //VECTOR3_H