/*==============================================================================

    衝突用形[shape.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/18
==============================================================================*/
#ifndef SHAPE_H
#define SHAPE_H

#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/gameObject.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Components/shader.h"
#include "Engine/Collision/collision.h"

//形
class Shape :public Component
{
public:
	enum Type
	{
		Type_NONE = -1,
		Type_LINE,
		Type_PLANE,
		Type_SPHERE,
		Type_BOX,
		Type_CAPSULE,
		Type_TRIANGEL,
		Type_Mesh,
		Type_MAX,
	};

	enum  CollisionResponse
	{
		Block,   // 押し返す
		Overlap, // 通過するがイベント発火
		Ignore   // 完全に無視
	};

protected:
	//絶対座標
	Vector3 m_Position;
	Vector3 m_Scale;
	Vector3 m_Rotation;

	//相対座標
	Vector3 m_RelativePosition;
	Vector3 m_RelativeScale;
	Vector3 m_RelativeRotation;

	bool m_Destroy = false;

	class AnimationModel* m_Model;

	class Shader* m_Shader;

	Type m_Type = Type_NONE;

	CollisionResponse m_Response = Ignore;

	bool m_Visible = false;

public:
	virtual ~Shape() {}
	virtual void Uninit()
	{ 
		delete m_Shader; 
		delete m_Model; 
	}
	virtual void Update() {}
	virtual void Update(const Vector3& pos, const Vector3& scl, const Vector3& rot) {}
	virtual void Draw() {}

	virtual bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr) { return false; }

	virtual bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) { return false; }

	virtual const Vector3 GetFoodPosition()const { return Vector3(0.0f, 0.0f, 0.0f); }

	virtual const Vector3 GetNormal(const Vector3& hitPoint)const { return Vector3(0.0f, 0.0f, 0.0f); }
	
	virtual void Set(GameObject* obj, CollisionResponse response = CollisionResponse::Ignore)
	{
		m_pOwner = obj;
		m_Position = obj->GetPosition();
		m_Scale = obj->GetScale();
		m_Rotation = obj->GetRotation();
		m_Response = response; 
		Manager::GetCollision()->RegisterAuto(this);
	};
	
	void SetPosition(const Vector3& pos) { m_Position = pos; }
	const Vector3& GetPosition()const { return m_Position; }

	void SetRotation(const Vector3& rot) { m_Rotation = rot; }
	const Vector3& GetRotation()const { return m_Rotation; }

	void SetScale(const Vector3& scale) { m_Scale = scale; }
	const Vector3& GetScale()const { return m_Scale; }

	void SetRelativePosition(const Vector3& pos) { m_RelativePosition = pos; }
	const Vector3& GetRelativePosition()const { return m_RelativePosition; }

	void SetRelativeRotation(const Vector3& rot) { m_RelativeRotation = rot; }
	const Vector3& GetRelativeRotation()const { return m_RelativeRotation; }

	void SetRelativeScale(const Vector3& scale) { m_RelativeScale = scale; }
	const Vector3& GetRelativeScale()const { return m_RelativeScale; }

	void SetDestroy(const bool destory) { m_Destroy = destory; }
	const bool GetDestroy()const { return m_Destroy; }

	Type GetType() const { return m_Type; }

	void SetCollisionResponse(CollisionResponse response) { m_Response = response; }
	CollisionResponse GetCollisionResponse() const { return m_Response; }

	virtual std::string GetShapeClassName() const { return "Shape"; }
};

//線
class Line :public Shape
{
private:
	Vector3 m_Start;	//	線分の始点
	Vector3 m_End;	//	線分の終点
	Vector3 m_Dir;	//	線分の方向ベクトル
	float m_Length;	//	線分の長さ

public:
	Line() : m_Start(Vector3()), m_End(Vector3()), m_Dir(Vector3()), m_Length(0.0f) { m_Type = Type_LINE; }
	Line(const Vector3& start, const Vector3& end)
		: m_Start(start), m_End(end), m_Dir(end - start), m_Length((end - start).length())
	{
		m_Type = Type_LINE;
	}
	Line(const Vector3& start, const Vector3& dir, float length)
		: m_Start(start), m_Dir(dir), m_Length(length)
	{
		m_End = start + dir.normalized() * length; // 終点を計算
		m_Type = Type_LINE;
	}

	void Update() override
	{
		m_Start = m_pOwner->GetPosition();
		m_Dir = m_pOwner->GetRotation().normalized();
		m_Length = m_pOwner->GetScale().x;
		m_End = m_Start + m_Dir * m_Length;
		 m_Visible = (m_pOwner->GetMouseSelection() && Manager::GetCreaterMode());
	}

	void Update(const Vector3& pos, const Vector3& scl, const Vector3& rot) override
	{
		m_Start = pos;
		m_Length = scl.x;
		m_Dir = rot.normalized();
		m_End = m_Start + m_Dir * m_Length;
	}

	void Update(const Vector3& start, const Vector3& end)
	{
		m_Start = start;
		m_End = end;
		m_Dir = end - start;
		m_Length = m_Dir.length();
		m_Dir.normalized();
	}

	void Update(const Vector3& start, const Vector3& dir, float length)
	{
		m_Start = start;
		m_Dir = dir.normalized();
		m_Length = length;
		m_End = m_Start + m_Dir * m_Length;
	}

	bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr)  override
	{
		return false;
	}

	bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) override;

	void SetStart(const Vector3& start) { m_Start = start; }
	const Vector3& GetStart()const { return m_Start; }

	void SetEnd(const Vector3& end) { m_End = end; }
	const Vector3& GetEnd()const { return m_End; }

	void SetDirection(const Vector3& dir) { m_Dir = dir; }
	const Vector3& GetDirection()const { return m_Dir; }

	void SetLength(const float length) { m_Length = length; }
	const float GetLength()const { return m_Length; }

	std::string GetShapeClassName() const override { return "Line"; }
};

//平面
class Plane :public Shape
{
private:
	Vector3 m_Normal;	   // 平面の法線ベクトル
	Vector3 m_Vertices[4]; // 平面の4つの頂点座標
	float m_d[5];          // 平面の方程式 Ax + By + Cz + D = 0 の D の値
	float m_D;             // 平面の方程式 Ax + By + Cz + D = 0 の D の値（平均値）
	int m_PlaneType = 0;   // 0→平面,1→ビルボード
public:
	Plane()
	{
		m_Position = m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_Scale = m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_Rotation = m_RelativeRotation = { 0.0f,0.0f,0.0f };

		// 平面の4つの頂点を初期化
		m_Vertices[0] = Vector3(-0.5f, 0.0f, 0.5f);
		m_Vertices[1] = Vector3(0.5f, 0.0f, 0.5f);
		m_Vertices[2] = Vector3(-0.5f, 0.0f, -0.5f);
		m_Vertices[3] = Vector3(0.5f, 0.0f, -0.5f);
		m_d[0] = -m_Normal.dot(m_Position);
		m_d[1] = -m_Normal.dot(m_Vertices[0]);
		m_d[2] = -m_Normal.dot(m_Vertices[1]);
		m_d[3] = -m_Normal.dot(m_Vertices[2]);
		m_d[4] = -m_Normal.dot(m_Vertices[3]);
		m_D = (m_d[0] + m_d[1] + m_d[2] + m_d[3] + m_d[4]) / 5.0f; // 平均値を使用
		m_Normal = { 0.0f,1.0f,0.0f };
		m_Type = Type_PLANE;

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_plane.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");
	}

	Plane(const Vector3 vertices[0])
	{
		// 頂点から平面の位置と法線を計算
		m_Position = (vertices[0] + vertices[1] + vertices[2] + vertices[3]) / 4.0f;

		m_Scale = Vector3((vertices[1] - vertices[0]).length(), 0.0f, (vertices[2] - vertices[1]).length());

		m_Normal = (vertices[1] - vertices[0]).cross(vertices[2] - vertices[1]).normalized();

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		for (int i = 0; i < 4; ++i)
		{
			this->m_Vertices[i] = vertices[i];
		}

		m_d[0] = -m_Normal.dot(m_Position);
		m_d[1] = -m_Normal.dot(vertices[0]);
		m_d[2] = -m_Normal.dot(vertices[1]);
		m_d[3] = -m_Normal.dot(vertices[2]);
		m_d[4] = -m_Normal.dot(vertices[3]);

		m_D = (m_d[0] + m_d[1] + m_d[2] + m_d[3] + m_d[4]) / 5.0f; // 平均値を使用

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_plane.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");

		m_Type = Type_PLANE;

	}

	// 平面/ビルボード
	Plane(const Vector3& pos, const Vector3& scl, const Vector3& rot, const int type)
	{
		m_Position = pos;
		m_Scale = scl;
		m_Rotation = rot;
		m_PlaneType = type;

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		// 回転行列
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		// X・Z軸方向のベクトル
		XMVECTOR axisX = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
		XMVECTOR axisZ = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);
		XMVECTOR normal = XMVectorSet(1.0f, 2.0f, 3.0f, 0.0f);

		if (type == 0) // 通常平面（XZ）
		{
			axisX = XMVector3Normalize(rotMat.r[0]); // X軸
			axisZ = XMVector3Normalize(rotMat.r[2]); // Z軸
			normal = XMVector3Normalize(rotMat.r[1]); // Y軸

		}
		else if (type == 1) // ビルボード（XY）
		{
			m_Scale = Vector3(scl.x, scl.z, scl.y);
			axisX = XMVector3Normalize(rotMat.r[0]); // X軸
			axisZ = XMVector3Normalize(rotMat.r[1]); // Y軸を Z軸代わりに
			normal = XMVector3Normalize(rotMat.r[2]); // Z軸を法線に
		}

		XMStoreFloat3((XMFLOAT3*)&m_Normal, normal);

		// 平面のサイズの半分
		Vector3 half = m_Scale * 0.5f;

		// 中心座標
		XMVECTOR center = XMLoadFloat3((XMFLOAT3*)&m_Position);

		// 頂点オフセット
		XMVECTOR offsetX = axisX * half.x;
		XMVECTOR offsetZ = axisZ * half.z;

		// 頂点計算
		XMVECTOR v0 = center - offsetX - offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[0], v0);

		XMVECTOR v1 = center + offsetX - offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[1], v1);

		XMVECTOR v2 = center + offsetX + offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[2], v2);

		XMVECTOR v3 = center - offsetX + offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[3], v3);

		// 平面方程式の D を頂点ごとに計算
		XMVECTOR normVec = XMLoadFloat3((XMFLOAT3*)&m_Normal);
		m_D = 0.0f;

		for (int i = 0; i < 4; ++i)
		{
			XMVECTOR v = XMLoadFloat3((XMFLOAT3*)&m_Vertices[i]);
			m_d[i] = -XMVectorGetX(XMVector3Dot(normVec, v));
			m_D += m_d[i];
		}

		m_D /= 4.0f; // Dの平均値

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_plane.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");

		m_Type = Type_PLANE;
	}

	void Set(GameObject* obj, int type = 0, CollisionResponse response = CollisionResponse::Ignore)
	{
		m_pOwner = obj;
		m_Position = obj->GetPosition();
		m_Scale = obj->GetScale();
		m_Rotation = obj->GetRotation();
		m_PlaneType = type;
		m_Response = response;
		Manager::GetCollision()->RegisterAuto(this);
	};

	void Update() override
	{
		m_Position = m_pOwner->GetPosition() + m_RelativePosition;
		m_Scale = m_pOwner->GetScale() * m_RelativeScale;
		m_Rotation = m_pOwner->GetRotation() + m_RelativeRotation;
		 m_Visible = (m_pOwner->GetMouseSelection() && Manager::GetCreaterMode());

		// 回転行列
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMVECTOR axisX, axisZ, normal;

		if (m_PlaneType == 0) // 通常の平面（XZ面、法線Y）
		{
			axisX = XMVector3Normalize(rotMat.r[0]); // X軸
			axisZ = XMVector3Normalize(rotMat.r[2]); // Z軸
			normal = XMVector3Normalize(rotMat.r[1]); // Y軸（上向き）
		}
		else if (m_PlaneType == 1) // ビルボード（XY面、法線Z）
		{
			m_Scale = Vector3(m_pOwner->GetScale().x * m_RelativeScale.x, m_pOwner->GetScale().z * m_RelativeScale.z, m_pOwner->GetScale().y * m_RelativeScale.y);
			axisX = XMVector3Normalize(rotMat.r[0]); // X軸
			axisZ = XMVector3Normalize(rotMat.r[1]); // Y軸を Z軸代わりに
			normal = XMVector3Normalize(rotMat.r[2]); // Z軸を法線に
		}

		XMStoreFloat3((XMFLOAT3*)&m_Normal, normal);

		// 平面のサイズの半分
		Vector3 half = m_Scale * 0.5f;

		// 中心座標
		XMVECTOR center = XMLoadFloat3((XMFLOAT3*)&m_Position);

		// 頂点オフセット
		XMVECTOR offsetX = axisX * half.x;
		XMVECTOR offsetZ = axisZ * half.z;

		// 頂点計算
		XMVECTOR v0 = center - offsetX - offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[0], v0);

		XMVECTOR v1 = center + offsetX - offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[1], v1);

		XMVECTOR v2 = center + offsetX + offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[2], v2);

		XMVECTOR v3 = center - offsetX + offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[3], v3);

		// 平面方程式の D を頂点ごとに計算
		XMVECTOR normVec = XMLoadFloat3((XMFLOAT3*)&m_Normal);
		m_D = 0.0f;

		for (int i = 0; i < 4; ++i)
		{
			XMVECTOR v = XMLoadFloat3((XMFLOAT3*)&m_Vertices[i]);
			m_d[i] = -XMVectorGetX(XMVector3Dot(normVec, v));
			m_D += m_d[i];
		}

		m_D /= 4.0f; // Dの平均値
	}

	void Update(const Vector3& pos, const Vector3& scl, const Vector3& rot) override
	{

		m_Position = pos + m_RelativePosition;
		m_Scale = scl * m_RelativeScale;
		m_Rotation = rot + m_RelativeRotation;

		// 回転行列
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		XMVECTOR axisX, axisZ, normal;

		if (m_PlaneType == 0) // 通常の平面（XZ面、法線Y）
		{
			axisX = XMVector3Normalize(rotMat.r[0]); // X軸
			axisZ = XMVector3Normalize(rotMat.r[2]); // Z軸
			normal = XMVector3Normalize(rotMat.r[1]); // Y軸（上向き）
		}
		else if (m_PlaneType == 1) // ビルボード（XY面、法線Z）
		{
			m_Scale = Vector3(scl.x * m_RelativeScale.x, scl.z * m_RelativeScale.z, scl.y * m_RelativeScale.y);
			axisX = XMVector3Normalize(rotMat.r[0]); // X軸
			axisZ = XMVector3Normalize(rotMat.r[1]); // Y軸を Z軸代わりに
			normal = XMVector3Normalize(rotMat.r[2]); // Z軸を法線に
		}

		XMStoreFloat3((XMFLOAT3*)&m_Normal, normal);

		// 平面のサイズの半分
		Vector3 half = m_Scale * 0.5f;

		// 中心座標
		XMVECTOR center = XMLoadFloat3((XMFLOAT3*)&m_Position);

		// 頂点オフセット
		XMVECTOR offsetX = axisX * half.x;
		XMVECTOR offsetZ = axisZ * half.z;

		// 頂点計算
		XMVECTOR v0 = center - offsetX - offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[0], v0);

		XMVECTOR v1 = center + offsetX - offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[1], v1);

		XMVECTOR v2 = center + offsetX + offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[2], v2);

		XMVECTOR v3 = center - offsetX + offsetZ;
		XMStoreFloat3((XMFLOAT3*)&m_Vertices[3], v3);

		// 平面方程式の D を頂点ごとに計算
		XMVECTOR normVec = XMLoadFloat3((XMFLOAT3*)&m_Normal);
		m_D = 0.0f;

		for (int i = 0; i < 4; ++i)
		{
			XMVECTOR v = XMLoadFloat3((XMFLOAT3*)&m_Vertices[i]);
			m_d[i] = -XMVectorGetX(XMVector3Dot(normVec, v));
			m_D += m_d[i];
		}

		m_D /= 4.0f; // Dの平均値
	}

	void Draw() override
	{
		if (!m_Visible)
		{
			return;
		}

		m_Shader->SetShader();

		//マトリクス設定
		XMMATRIX world, scale, rotation, trancelation;

		if (m_PlaneType == 0) // 通常の平面（XZ面、法線Y）
		{
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
			trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
		}
		else if (m_PlaneType == 1) // ビルボード（XY面、法線Z）
		{
			scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
			rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x - XM_PIDIV2, m_Rotation.y, m_Rotation.z);
			trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
		}

		world = scale * rotation * trancelation;

		Renderer::SetWorldMatrix(world);

		Renderer::SetRasterSolid(false);

		m_Model->Draw();

		Renderer::SetRasterSolid(true);
	}

	bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr)  override
	{
		return LineToPlane(line, *this, type, outHitPos, outHitDist);
	}

	bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) override;

	const Vector3 GetNormal(const Vector3& hitPoint)const override
	{
		return m_Normal;
	}

	const Vector3 GetNormal()const
	{
		return m_Normal;
	}

	void SetVertices(int num, const Vector3& vertices) { m_Vertices[num] = vertices; }
	const Vector3& GetVertices(int num)const { return m_Vertices[num]; }

	void SetD(const float d) { m_D = d; }
	const float GetD()const { return m_D; }


	std::string GetShapeClassName() const override { return "Plane"; }
};

//球
class Sphere :public Shape
{
private:
	float m_Radius;	//	球の半径
public:
	Sphere()
	{
		m_Position = { 0.0f,0.0f,0.0f };
		m_Radius = 1.0f;
		m_Scale = Vector3(m_Radius, m_Radius, m_Radius);

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_sphere.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");

		m_Type = Type_SPHERE;
	}

	Sphere(const Vector3& pos, float radius)
	{
		m_Position = pos;
		m_Radius = radius;
		m_Scale = Vector3(radius, radius, radius);

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_sphere.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");

		m_Type = Type_SPHERE;
	}

	void Update() override
	{
		m_Position = m_pOwner->GetPosition() + m_RelativePosition;
		m_Scale = m_pOwner->GetScale().x * m_RelativeScale;
		m_Scale.y = m_Scale.x;
		m_Scale.z = m_Scale.x;
		m_Radius = m_pOwner->GetScale().x * m_RelativeScale.x;
		 m_Visible = (m_pOwner->GetMouseSelection() && Manager::GetCreaterMode());
	}

	void Update(const Vector3& pos, const Vector3& scl, const Vector3& rot) override
	{
		m_Position = pos + m_RelativePosition;
		m_Scale = scl.x * m_RelativeScale;
		m_Scale.y = m_Scale.x;
		m_Scale.z = m_Scale.x;
		m_Radius = scl.x * m_RelativeScale.x;
	}

	void Update(const Vector3& pos, float radius)
	{
		m_Position = pos + m_RelativePosition;
		m_Scale = radius * m_RelativeScale;
		m_Scale.y = m_Scale.x;
		m_Scale.z = m_Scale.x;
		m_Radius = radius * m_RelativeScale.x;
	}

	void Draw() override
	{
		if (!m_Visible)
		{
			return;
		}

		m_Shader->SetShader();

		//マトリクス設定
		XMMATRIX world, scale, rotation, trancelation;

		scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
		rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		world = scale * rotation * trancelation;

		Renderer::SetWorldMatrix(world);

		Renderer::SetRasterSolid(false);

		m_Model->Draw();

		Renderer::SetRasterSolid(true);
	}

	bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr)  override
	{
		return LineToSphere(line, *this, type, outHitPos, outHitDist);
	}

	bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) override;

	const Vector3 GetFoodPosition()const override
	{
		return m_Position - Vector3(0, m_Radius, 0);
	}

	const Vector3 GetNormal(const Vector3& hitPoint)const override
	{
		return (hitPoint - m_Position).normalized();
	}

	void SetRadius(const float radius) { m_Radius = radius; }
	const float GetRadius()const { return m_Radius; }

	std::string GetShapeClassName() const override { return "Sphere"; }
};

//立方体
class Box :public Shape
{
private:
	Vector3 m_Halfsize;
	Vector3 m_Axis[3];     // 立方体の軸（X, Y, Z）
	Vector3 m_Vertices[8]; // 立方体の8つの頂点座標

public:
	Box()
	{
		m_Position = { 0.0f,0.0f,0.0f };
		m_Scale = { 1.0f,1.0f,1.0f };
		m_Rotation = { 0.0f,0.0f,0.0f };

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		m_Halfsize = m_Scale * 0.5f;

		// 立方体の8つの頂点を初期化
		m_Vertices[0] = Vector3(-0.5f, -0.5f, -0.5f);
		m_Vertices[1] = Vector3(0.5f, -0.5f, -0.5f);
		m_Vertices[2] = Vector3(-0.5f, 0.5f, -0.5f);
		m_Vertices[3] = Vector3(0.5f, 0.5f, -0.5f);
		m_Vertices[4] = Vector3(-0.5f, -0.5f, 0.5f);
		m_Vertices[5] = Vector3(0.5f, -0.5f, 0.5f);
		m_Vertices[6] = Vector3(-0.5f, 0.5f, 0.5f);
		m_Vertices[7] = Vector3(0.5f, 0.5f, 0.5f);

		// 立方体の軸を計算
		m_Axis[0] = Vector3(1.0f, 0.0f, 0.0f); // X軸
		m_Axis[1] = Vector3(0.0f, 1.0f, 0.0f); // Y軸
		m_Axis[2] = Vector3(0.0f, 0.0f, 1.0f); // Z軸

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_cube.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");

		m_Type = Type_BOX;
	}

	Box(const Vector3& pos, const Vector3& scl, const Vector3& rot)
	{
		m_Position = pos;
		m_Scale = scl;
		m_Rotation = rot;

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		// 半サイズ
		m_Halfsize = m_Scale * 0.5f;

		// 回転行列（XYZ回転＝ピッチ・ヨー・ロール）
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		// 各軸ベクトルを取得（回転行列の列ベクトル）
		XMVECTOR axisX = XMVector3Normalize(rotMat.r[0]);
		XMVECTOR axisY = XMVector3Normalize(rotMat.r[1]);
		XMVECTOR axisZ = XMVector3Normalize(rotMat.r[2]);

		// m_axis に格納
		XMStoreFloat3((XMFLOAT3*)&m_Axis[0], axisX);  // X軸
		XMStoreFloat3((XMFLOAT3*)&m_Axis[1], axisY);  // Y軸
		XMStoreFloat3((XMFLOAT3*)&m_Axis[2], axisZ);  // Z軸

		// ローカル頂点オフセット（-1 ~ +1）
		static const Vector3 localOffsets[8] =
		{
			{-1, -1, -1}, {+1, -1, -1}, {-1, +1, -1}, {+1, +1, -1},
			{-1, -1, +1}, {+1, -1, +1}, {-1, +1, +1}, {+1, +1, +1},
		};

		XMVECTOR xmPos = XMLoadFloat3((XMFLOAT3*)&m_Position);
		XMVECTOR xmAxisX = XMLoadFloat3((XMFLOAT3*)&m_Axis[0]);
		XMVECTOR xmAxisY = XMLoadFloat3((XMFLOAT3*)&m_Axis[1]);
		XMVECTOR xmAxisZ = XMLoadFloat3((XMFLOAT3*)&m_Axis[2]);

		for (int i = 0; i < 8; ++i)
		{
			Vector3 offset = localOffsets[i] * m_Halfsize;

			XMVECTOR worldVertex =
				xmPos
				+ xmAxisX * offset.x
				+ xmAxisY * offset.y
				+ xmAxisZ * offset.z;

			XMStoreFloat3((XMFLOAT3*)&m_Vertices[i], worldVertex);
		}

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_cube.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");

		m_Type = Type_BOX;
	}

	void Update() override
	{
		m_Position = m_pOwner->GetPosition() + m_RelativePosition;
		m_Scale = m_pOwner->GetScale() * m_RelativeScale;
		m_Rotation = m_pOwner->GetRotation() + m_RelativeRotation;
		 m_Visible = (m_pOwner->GetMouseSelection() && Manager::GetCreaterMode());

		// 半サイズ
		m_Halfsize = m_Scale * 0.5f;

		// 回転行列（XYZ回転＝ピッチ・ヨー・ロール）
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		// 各軸ベクトルを取得（回転行列の列ベクトル）
		XMVECTOR axisX = XMVector3Normalize(rotMat.r[0]);
		XMVECTOR axisY = XMVector3Normalize(rotMat.r[1]);
		XMVECTOR axisZ = XMVector3Normalize(rotMat.r[2]);

		// m_axis に格納
		XMStoreFloat3((XMFLOAT3*)&m_Axis[0], axisX);  // X軸
		XMStoreFloat3((XMFLOAT3*)&m_Axis[1], axisY);  // Y軸
		XMStoreFloat3((XMFLOAT3*)&m_Axis[2], axisZ);  // Z軸

		// ローカル頂点オフセット（-1 ~ +1）
		static const Vector3 localOffsets[8] =
		{
			{-1, -1, -1}, {+1, -1, -1}, {-1, +1, -1}, {+1, +1, -1},
			{-1, -1, +1}, {+1, -1, +1}, {-1, +1, +1}, {+1, +1, +1},
		};

		XMVECTOR xmPos = XMLoadFloat3((XMFLOAT3*)&m_Position);
		XMVECTOR xmAxisX = XMLoadFloat3((XMFLOAT3*)&m_Axis[0]);
		XMVECTOR xmAxisY = XMLoadFloat3((XMFLOAT3*)&m_Axis[1]);
		XMVECTOR xmAxisZ = XMLoadFloat3((XMFLOAT3*)&m_Axis[2]);

		for (int i = 0; i < 8; ++i)
		{
			Vector3 offset = localOffsets[i] * m_Halfsize;

			XMVECTOR worldVertex =
				xmPos
				+ xmAxisX * offset.x
				+ xmAxisY * offset.y
				+ xmAxisZ * offset.z;

			XMStoreFloat3((XMFLOAT3*)&m_Vertices[i], worldVertex);
		}


	}

	void Update(const Vector3& pos, const Vector3& scl, const Vector3& rot) override
	{
		m_Position = pos + m_RelativePosition;
		m_Scale = scl * m_RelativeScale;
		m_Rotation = rot + m_RelativeRotation;

		// 半サイズ
		m_Halfsize = m_Scale * 0.5f;

		// 回転行列（XYZ回転＝ピッチ・ヨー・ロール）
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		// 各軸ベクトルを取得（回転行列の列ベクトル）
		XMVECTOR axisX = XMVector3Normalize(rotMat.r[0]);
		XMVECTOR axisY = XMVector3Normalize(rotMat.r[1]);
		XMVECTOR axisZ = XMVector3Normalize(rotMat.r[2]);

		// m_axis に格納
		XMStoreFloat3((XMFLOAT3*)&m_Axis[0], axisX);  // X軸
		XMStoreFloat3((XMFLOAT3*)&m_Axis[1], axisY);  // Y軸
		XMStoreFloat3((XMFLOAT3*)&m_Axis[2], axisZ);  // Z軸

		// ローカル頂点オフセット（-1 ~ +1）
		static const Vector3 localOffsets[8] =
		{
			{-1, -1, -1}, {+1, -1, -1}, {-1, +1, -1}, {+1, +1, -1},
			{-1, -1, +1}, {+1, -1, +1}, {-1, +1, +1}, {+1, +1, +1},
		};

		XMVECTOR xmPos = XMLoadFloat3((XMFLOAT3*)&m_Position);
		XMVECTOR xmAxisX = XMLoadFloat3((XMFLOAT3*)&m_Axis[0]);
		XMVECTOR xmAxisY = XMLoadFloat3((XMFLOAT3*)&m_Axis[1]);
		XMVECTOR xmAxisZ = XMLoadFloat3((XMFLOAT3*)&m_Axis[2]);

		for (int i = 0; i < 8; ++i)
		{
			Vector3 offset = localOffsets[i] * m_Halfsize;

			XMVECTOR worldVertex =
				xmPos
				+ xmAxisX * offset.x
				+ xmAxisY * offset.y
				+ xmAxisZ * offset.z;

			XMStoreFloat3((XMFLOAT3*)&m_Vertices[i], worldVertex);
		}
	}
	
	//軸で更新
	void Update(const Vector3& pos, const Vector3& scl,
		const Vector3& axisX_in, const Vector3& axisY_in, const Vector3& axisZ_in)
	{
		m_Position = pos + m_RelativePosition;
		m_Scale = scl * m_RelativeScale;
		m_Halfsize = m_Scale * 0.5f;

		Vector3 x = axisX_in;
		Vector3 y = axisY_in;

		if (x.lengthSq() < 1e-6f) x = Vector3(1, 0, 0);
		x.normalize();

		// y を x に直交化
		y = y - x * y.dot(x);
		if (y.lengthSq() < 1e-6f)
		{
			Vector3 fallback = (fabsf(x.y) < 0.99f) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
			y = fallback - x * fallback.dot(x);
		}
		y.normalize();

		Vector3 z = y.cross(x);
		if (z.lengthSq() < 1e-6f) z = Vector3(0, 0, 1);
		z.normalize();

		y = x.cross(z);
		y.normalize();

		m_Axis[0] = x;
		m_Axis[1] = y;
		m_Axis[2] = z;

		static const Vector3 localOffsets[8] =
		{
			{-1, -1, -1}, {+1, -1, -1}, {-1, +1, -1}, {+1, +1, -1},
			{-1, -1, +1}, {+1, -1, +1}, {-1, +1, +1}, {+1, +1, +1},
		};

		for (int i = 0; i < 8; ++i)
		{
			Vector3 o = localOffsets[i] * m_Halfsize;
			m_Vertices[i] = m_Position
				+ m_Axis[0] * o.x
				+ m_Axis[1] * o.y
				+ m_Axis[2] * o.z;
		}
	}

	void Draw() override
	{
		if (!m_Visible)
		{
			return;
		}

		m_Shader->SetShader();

		//マトリクス設定
		XMMATRIX scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

		// m_Axis から回転行列を作る
		XMMATRIX rot =
			XMMATRIX(
				m_Axis[0].x, m_Axis[0].y, m_Axis[0].z, 0.0f,   // Right
				m_Axis[1].x, m_Axis[1].y, m_Axis[1].z, 0.0f,   // Up
				m_Axis[2].x, m_Axis[2].y, m_Axis[2].z, 0.0f,   // Forward
				0.0f, 0.0f, 0.0f, 1.0f
			);

		XMMATRIX trans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		XMMATRIX world = scale * rot * trans;
		Renderer::SetWorldMatrix(world);
		Renderer::SetWorldMatrix(world);

		Renderer::SetRasterSolid(false);

		m_Model->Draw();

		Renderer::SetRasterSolid(true);
	}
	
	bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr)  override
	{
		return LineToBox(line, *this, type, outHitPos, outHitDist);
	}

	bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) override;

	const Vector3 GetFoodPosition()const override
	{
		return m_Position - GetAxis(1) * m_Halfsize.y;
	}

	const Vector3 GetNormal(const Vector3& hitPoint)const override
	{
		Vector3 localPoint = WorldToLocal(hitPoint);
		Vector3 localNormal;

		Vector3 absPoint = localPoint.abs();
		Vector3 halfSize = m_Halfsize;

		// X軸に最も近い
		if (absPoint.x >= absPoint.y && absPoint.x >= absPoint.z)
		{
			localNormal = Vector3((localPoint.x >= 0.0f) ? 1.0f : -1.0f, 0.0f, 0.0f);
		}
		// Y軸に最も近い
		else if (absPoint.y >= absPoint.x && absPoint.y >= absPoint.z)
		{
			localNormal = Vector3(0.0f, (localPoint.y >= 0.0f) ? 1.0f : -1, 0.0f);
		}
		// Z軸に最も近い
		else
		{
			localNormal = Vector3(0.0f, 0.0f, (localPoint.z >= 0.0f) ? 1.0f : -1.0f);
		}
		// ローカル法線 → ワールド法線
		return LocalToWorldDirection(localNormal).normalized();
	}

	void SetHalfsize(const Vector3& halfsize) { m_Halfsize = halfsize; }
	const Vector3& GetHalfsize()const { return m_Halfsize; }

	void SetAxis(int num, const Vector3& axis) { m_Axis[num] = axis; }
	const Vector3& GetAxis(int num)const { return m_Axis[num]; }

	void SetVertices(int num, const Vector3& vertices) { m_Vertices[num] = vertices; }
	const Vector3& GetVertices(int num)const { return m_Vertices[num]; }

	Vector3 WorldToLocal(const Vector3& worldPoint) const
	{
		// 差分を計算（ワールド → OBBの中心を原点とする）
		Vector3 diff = worldPoint - m_Position;

		// 各軸への投影成分を求める（ローカル座標系での位置）
		float x = diff.dot(m_Axis[0]); // 右軸
		float y = diff.dot(m_Axis[1]); // 上軸
		float z = diff.dot(m_Axis[2]); // 前軸

		return Vector3(x, y, z);
	}

	Vector3 LocalToWorldDirection(const Vector3& localDir) const
	{
		return (m_Axis[0] * localDir.x + m_Axis[1] * localDir.y + m_Axis[2] * localDir.z);
	}

	std::string GetShapeClassName() const override { return "Box"; }
};

//カプセル
class Capsule : public Shape
{
private:
	Vector3 m_PointA;  // カプセルの始点（ローカル空間）
	Vector3 m_PointB;  // 終点
	float m_Radius;    // 半径
public:
	Capsule()
	{
		m_Position = { 0.0f,0.0f,0.0f };
		m_Scale = { 1.0f,1.0f,1.0f };
		m_Rotation = { 1.0f,1.0f,1.0f };

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		m_PointA = { 0.0f,0.5f,0.0f };
		m_PointB = { 0.0f,-0.5f,0.0f };
		m_Radius = 1.0f;

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_capsule.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");
		m_Type = Type_CAPSULE;
	}

	Capsule(const Vector3& pos, const Vector3& scl, const Vector3& rot)
	{
		m_Position = pos;
		m_Scale = scl;
		m_Rotation = rot;

		m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_RelativeRotation = { 0.0f,0.0f,0.0f };

		// 回転行列（XYZ回転＝ピッチ・ヨー・ロール）
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);

		// 各軸ベクトルを取得（回転行列の列ベクトル）
		XMVECTOR axisX = XMVector3Normalize(rotMat.r[0]);
		XMVECTOR axisY = XMVector3Normalize(rotMat.r[1]);
		XMVECTOR axisZ = XMVector3Normalize(rotMat.r[2]);

		XMVECTOR xmPos = XMLoadFloat3((XMFLOAT3*)&m_Position);

		// ローカル頂点
		const Vector3 localA = { 0.0f,0.5f,0.0f };
		const Vector3 localB = { 0.0f,-0.5f,0.0f };

		Vector3 offsetA = localA * m_Scale.x;
		Vector3 offsetB = localB * m_Scale.x;

		XMVECTOR VectoA = xmPos + axisX * offsetA.x + axisY * offsetA.y + axisZ * offsetA.z;
		XMVECTOR VectoB = xmPos + axisX * offsetB.x + axisY * offsetB.y + axisZ * offsetB.z;

		XMStoreFloat3((XMFLOAT3*)&m_PointA, VectoA);
		XMStoreFloat3((XMFLOAT3*)&m_PointB, VectoB);

		m_Radius = m_Scale.y;

		m_Model = new AnimationModel;
		m_Model->Load("Assets\\model\\Collision_capsule.fbx");

		m_Shader = new Shader;
		m_Shader->Load("grayscale");
		m_Type = Type_CAPSULE;
	}

	void Update()override 
	{
		m_Position = m_pOwner->GetPosition() + m_RelativePosition;
		m_Scale = m_pOwner->GetScale() * m_RelativeScale;
		m_Rotation = m_pOwner->GetRotation() + m_RelativeRotation;
		 m_Visible = (m_pOwner->GetMouseSelection() && Manager::GetCreaterMode());

		// 回転行列（XYZ回転＝ピッチ・ヨー・ロール）
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		// 各軸ベクトルを取得（回転行列の列ベクトル）
		XMVECTOR axisX = XMVector3Normalize(rotMat.r[0]);
		XMVECTOR axisY = XMVector3Normalize(rotMat.r[1]);
		XMVECTOR axisZ = XMVector3Normalize(rotMat.r[2]);

		XMVECTOR xmPos = XMLoadFloat3((XMFLOAT3*)&m_Position);

		// ローカル頂点
		const Vector3 localA = { 0.0f,0.5f,0.0f };
		const Vector3 localB = { 0.0f,-0.5f,0.0f };

		Vector3 offsetA = localA * m_Scale.y;
		Vector3 offsetB = localB * m_Scale.y;

		XMVECTOR VectoA = xmPos + axisX * offsetA.x + axisY * offsetA.y + axisZ * offsetA.z;
		XMVECTOR VectoB = xmPos + axisX * offsetB.x + axisY * offsetB.y + axisZ * offsetB.z;

		XMStoreFloat3((XMFLOAT3*)&m_PointA, VectoA);
		XMStoreFloat3((XMFLOAT3*)&m_PointB, VectoB);


		m_Radius = m_Scale.z = m_Scale.x * 0.5f;
	}

	void Update(const Vector3& pos, const Vector3& scl, const Vector3& rot) override
	{
		m_Position = pos + m_RelativePosition;
		m_Scale = scl * m_RelativeScale;
		m_Rotation = rot + m_RelativeRotation;

		// 回転行列（XYZ回転＝ピッチ・ヨー・ロール）
		XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		// 各軸ベクトルを取得（回転行列の列ベクトル）
		XMVECTOR axisX = XMVector3Normalize(rotMat.r[0]);
		XMVECTOR axisY = XMVector3Normalize(rotMat.r[1]);
		XMVECTOR axisZ = XMVector3Normalize(rotMat.r[2]);

		XMVECTOR xmPos = XMLoadFloat3((XMFLOAT3*)&m_Position);

		// ローカル頂点
		const Vector3 localA = { 0.0f,0.5f,0.0f };
		const Vector3 localB = { 0.0f,-0.5f,0.0f };

		Vector3 offsetA = localA * m_Scale.y;
		Vector3 offsetB = localB * m_Scale.y;

		XMVECTOR VectoA = xmPos + axisX * offsetA.x + axisY * offsetA.y + axisZ * offsetA.z;
		XMVECTOR VectoB = xmPos + axisX * offsetB.x + axisY * offsetB.y + axisZ * offsetB.z;

		XMStoreFloat3((XMFLOAT3*)&m_PointA, VectoA);
		XMStoreFloat3((XMFLOAT3*)&m_PointB, VectoB);


		m_Radius = m_Scale.z = m_Scale.x * 0.5f;
	}

	void Draw() override
	{
		if (!m_Visible)
		{
			return;
		}

		m_Shader->SetShader();
		//マトリクス設定
		XMMATRIX world, scale, rotation, trancelation;

		scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.x);
		rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
		trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

		world = scale * rotation * trancelation;

		Renderer::SetWorldMatrix(world);

		Renderer::SetRasterSolid(false);

		m_Model->Draw();

		Renderer::SetRasterSolid(true);
	}

	bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr)  override
	{
		return LineToCapsule(line, *this, type, outHitPos, outHitDist);
	}

	bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) override;

	const Vector3 GetFoodPosition()const override
	{
		return GetPointB() - Vector3(0.0f, m_Radius - 0.01f, 0.0f);
	}

	const Vector3 GetNormal(const Vector3& hitPoint)const override
	{
		Line lineC(m_PointA, m_PointB);

		Vector3 closestPoint = ClosestPointToLine(hitPoint, lineC, 0);
		return (hitPoint - closestPoint).normalized();
	}

	void SetPointA(const Vector3& point) { m_PointA = point; }
	const Vector3& GetPointA()const { return m_PointA; }

	void SetPointB(const Vector3& point) { m_PointB = point; }
	const Vector3& GetPointB()const { return m_PointB; }

	void SetRadius(const float radius) { m_Radius = radius; }
	const float GetRadius()const { return m_Radius; }

	std::string GetShapeClassName() const override { return "Capsule"; }
};

//三角形
class Triangle : public Shape
{
private:
	Vector3 m_A, m_B, m_C; // 頂点3つ
	Vector3 m_Normal;	   // 平面の法線ベクトル
public:
	Triangle() : m_A(Vector3()), m_B(Vector3()), m_C(Vector3()), m_Normal(Vector3()) { m_Type = Type_TRIANGEL; }
	Triangle(const Vector3& a, const Vector3& b, const Vector3& c) : m_A(a), m_B(b), m_C(c) 
	{ 
		m_Position = (m_A + m_B + m_C) / 3.0f;
		UpdateNormal();
		m_Type = Type_TRIANGEL;
	}

	void Update(const Vector3& a, const Vector3& b, const Vector3& c)
	{
		m_A = a;
		m_B = b;
		m_C = c;
		UpdateNormal();
		m_Position = (m_A + m_B + m_C) / 3.0f;
	}

	const Vector3 GetNormal() const { return m_Normal; }

	const Vector3& GetA() const { return m_A; }
	const Vector3& GetB() const { return m_B; }
	const Vector3& GetC() const { return m_C; }

private:
	void UpdateNormal()
	{
		Vector3 edge1 = m_B - m_A;
		Vector3 edge2 = m_C - m_A;
		m_Normal = crossProduct(edge1, edge2);
		m_Normal.normalize();
	}
};

class CollisionMesh :public Shape
{
private:
	std::vector<Triangle> m_Triangles;
	std::vector<VERTEX_3D> m_VertexBufferData;
	ID3D11Buffer* m_VertexBuffer = nullptr;

	struct TriIndex
	{
		int ax, az;
		int bx, bz;
		int cx, cz;
	};

	std::vector<TriIndex> m_TriangleIndices;

public:
	CollisionMesh() 
	{ 
		m_Position = m_RelativePosition = { 0.0f,0.0f,0.0f };
		m_Scale = m_RelativeScale = { 1.0f,1.0f,1.0f };
		m_Rotation = m_RelativeRotation = { 0.0f,0.0f,0.0f };

		m_Shader = new Shader;
		m_Shader->Load("grayscale");
		m_Type = Type_Mesh; 
	}

	CollisionMesh(VERTEX_3D vertices[21][21])
	{
		SetVertex3D(vertices);

		m_Type = Type_Mesh;
	}

	void Update()override;
	
	void Set(GameObject* obj, CollisionResponse response = CollisionResponse::Ignore) override;

	void Set(GameObject* obj, VERTEX_3D(*vertices)[21],CollisionResponse response = CollisionResponse::Ignore)
	{
		m_pOwner = obj;
		m_Position = obj->GetPosition();
		m_Scale = obj->GetScale();
		m_Rotation = obj->GetRotation();
		m_Response = response;
		SetVertex3D(vertices);
		Manager::GetCollision()->RegisterAuto(this);
	};

	bool Intersect(const Line& line, const int type, Vector3* outHitPos = nullptr, float* outHitDist = nullptr)  override
	{
		return LineToMesh(line, *this, type, outHitPos, outHitDist);
	}

	bool Intersect(const Shape& shape, Vector3* outNormal = nullptr, float* outPenetration = nullptr) override;

	void SetVertex3D(const VERTEX_3D(*vertices)[21]);
	void UpdateVertex3D(const VERTEX_3D(*vertices)[21]);

	void Draw() override;

	const std::vector<Triangle>& GetTriangles() const { return m_Triangles; }
};

#endif //SHAPE_H
