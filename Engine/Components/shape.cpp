/*==============================================================================

    型[shape.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/18
==============================================================================*/
#include "main.h"
#include "Engine/Components/shape.h"
#include "meshField.h"
#include "wave.h"

bool Line::Intersect(const Shape& shape, Vector3* outNormal,float* outPenetration)
{
	if (outNormal)
	{
		*outNormal = Vector3(0, 0, 0);
	}

	if (outPenetration)
	{
		*outPenetration = 0.0f;
	}

	switch (shape.GetType())
	{
	case Type_LINE:
		return false;
		break;
	case Type_PLANE:
		return LineToPlane(*this, static_cast<const Plane&>(shape), 1, outNormal);

	case Type_SPHERE:
		return LineToSphere(*this, static_cast<const Sphere&>(shape), 1, outNormal);

	case Type_BOX:
		return LineToBox(*this, static_cast<const Box&>(shape), 1, outNormal);

	case Type_CAPSULE:
		return LineToCapsule(*this, static_cast<const Capsule&>(shape), 1, outNormal);
	
	case Type_Mesh:
		return LineToMesh(*this, static_cast<const CollisionMesh&>(shape), 1, outNormal);

	default:
		return false;
	}
}

bool Plane::Intersect(const Shape& shape, Vector3* outNormal, float* outPenetration)
{
	if (outNormal)
	{
		*outNormal = Vector3(0, 0, 0);
	}

	if (outPenetration)
	{
		*outPenetration = 0.0f;
	}

	switch (shape.GetType())
	{
	case Type_LINE:
		return LineToPlane(static_cast<const Line&>(shape), *this, 1, outNormal);

	case Type_PLANE:
		return PlaneToPlane(*this, static_cast<const Plane&>(shape));

	case Type_SPHERE:
		return PlaneToSphere(*this, static_cast<const Sphere&>(shape), outNormal, outPenetration);

	case Type_BOX:
		return FinitePlaneToBox(*this, static_cast<const Box&>(shape), outNormal, outPenetration);

	case Type_CAPSULE:
		return PlaneToCapsule(*this, static_cast<const Capsule&>(shape), outNormal, outPenetration);
	
	case Type_Mesh:
		return false;

	default:
		return false;
	}
}

bool Sphere::Intersect(const Shape& shape, Vector3* outNormal, float* outPenetration)
{
	if (outNormal)
	{
		*outNormal = Vector3(0, 0, 0);
	}

	if (outPenetration)
	{
		*outPenetration = 0.0f;
	}

	switch (shape.GetType())
	{
	case Type_LINE:
		return  LineToSphere(static_cast<const Line&>(shape), *this, 1, outNormal);

	case Type_PLANE:
		return PlaneToSphere(static_cast<const Plane&>(shape), *this, outNormal, outPenetration);

	case Type_SPHERE:
		return SphereToSphere(*this, static_cast<const Sphere&>(shape), outNormal, outPenetration);

	case Type_BOX:
		return SphereToBox(*this, static_cast<const Box&>(shape), outNormal, outPenetration);

	case Type_CAPSULE:
		return SphereToCapsule(*this, static_cast<const Capsule&>(shape), outNormal, outPenetration);

	case Type_Mesh:
		return SphereToMesh(*this, static_cast<const CollisionMesh&>(shape), outNormal, outPenetration);

	default:
		return false;
	}
}

bool Box::Intersect(const Shape& shape, Vector3* outNormal, float* outPenetration)
{
	if (outNormal)
	{
		*outNormal = Vector3(0, 0, 0);
	}

	if (outPenetration)
	{
		*outPenetration = 0.0f;
	}

	switch (shape.GetType())
	{
	case Type_LINE:
		return LineToBox(static_cast<const Line&>(shape), *this, 1, outNormal);

	case Type_PLANE:
		return FinitePlaneToBox(static_cast<const Plane&>(shape), *this, outNormal, outPenetration);

	case Type_SPHERE:
		return SphereToBox(static_cast<const Sphere&>(shape), *this, outNormal, outPenetration);

	case Type_BOX:
		return BoxToBox(*this, static_cast<const Box&>(shape), outNormal, outPenetration);

	case Type_CAPSULE:
		return BoxToCapsule(*this, static_cast<const Capsule&>(shape), outNormal, outPenetration);
	
	case Type_Mesh:
		return BoxToMesh(*this, static_cast<const CollisionMesh&>(shape), outNormal, outPenetration);

	default:
		return false;
	}
}

bool Capsule::Intersect(const Shape& shape, Vector3* outNormal, float* outPenetration)
{
	if (outNormal)
	{
		*outNormal = Vector3(0, 0, 0);
	}

	if (outPenetration)
	{
		*outPenetration = 0.0f;
	}

	switch (shape.GetType())
	{
	case Type_LINE:
		return LineToCapsule(static_cast<const Line&>(shape), *this, 1, outNormal);

	case Type_PLANE:
		return PlaneToCapsule(static_cast<const Plane&>(shape), *this, outNormal, outPenetration);

	case Type_SPHERE:
		return SphereToCapsule(static_cast<const Sphere&>(shape), *this, outNormal, outPenetration);

	case Type_BOX:
		return BoxToCapsule(static_cast<const Box&>(shape), *this, outNormal, outPenetration);

	case Type_CAPSULE:
		return CapsuleToCapsule(*this, static_cast<const Capsule&>(shape), outNormal, outPenetration);

	case Type_Mesh:
		return CapsuleToMesh(*this, static_cast<const CollisionMesh&>(shape), outNormal, outPenetration);

	default:
		return false;
	}
}

bool CollisionMesh::Intersect(const Shape& shape, Vector3* outNormal, float* outPenetration)
{
	if (outNormal)
	{
		*outNormal = Vector3(0, 0, 0);
	}

	if (outPenetration)
	{
		*outPenetration = 0.0f;
	}

	switch (shape.GetType())
	{
	case Type_LINE:
		return LineToMesh(static_cast<const Line&>(shape), *this, 1, outNormal);

	case Type_PLANE:
		return false;

	case Type_SPHERE:
		return SphereToMesh(static_cast<const Sphere&>(shape), *this, outNormal, outPenetration);

	case Type_BOX:
		return BoxToMesh(static_cast<const Box&>(shape), *this, outNormal, outPenetration);

	case Type_CAPSULE:
		return CapsuleToMesh(static_cast<const Capsule&>(shape), *this, outNormal, outPenetration);

	default:
		return false;
	}
}

void CollisionMesh::Update()
{
	m_Position = m_pOwner->GetPosition() + m_RelativePosition;
	m_Scale = m_pOwner->GetScale() * m_RelativeScale;
	m_Rotation = m_pOwner->GetRotation() + m_RelativeRotation;
	m_Visible = (m_pOwner->GetMouseSelection() && Manager::GetCreaterMode());

	if (auto wave = dynamic_cast<Wave*>(m_pOwner))
	{
		UpdateVertex3D(wave->GetVertices());
	}
	else if (auto mesh = dynamic_cast<MeshField*>(m_pOwner))
	{
		UpdateVertex3D(mesh->GetVertices());
	}
}

void CollisionMesh::Set(GameObject* obj, CollisionResponse response)
{
	
	m_pOwner = obj;
	m_Position = m_pOwner->GetPosition() + m_RelativePosition;
	m_Scale = m_pOwner->GetScale() * m_RelativeScale;
	m_Rotation = m_pOwner->GetRotation() + m_RelativeRotation;
	m_Response = response;
	if (auto filed = dynamic_cast<MeshField*>(obj))
	{
		SetVertex3D(filed->GetVertices());
	}
	else if (auto wave = dynamic_cast<Wave*>(obj))
	{
		SetVertex3D(wave->GetVertices());
	}
	Manager::GetCollision()->RegisterAuto(this);
	
}

void CollisionMesh::SetVertex3D(const VERTEX_3D(*vertices)[21])
{
	// 衝突判定用三角形リストを生成
	m_Triangles.clear();
	m_Triangles.reserve(20 * 20 * 2); 

	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			// 4頂点を取得
			Vector3 v00 = XMFLOAT3ToVector3(vertices[x][z].Position);
			Vector3 v10 = XMFLOAT3ToVector3(vertices[x + 1][z].Position);
			Vector3 v01 = XMFLOAT3ToVector3(vertices[x][z + 1].Position);
			Vector3 v11 = XMFLOAT3ToVector3(vertices[x + 1][z + 1].Position);

			// 1枚目の三角形 (v00, v10, v01)
			Triangle t1(v00, v10, v01);
			// 2枚目の三角形 (v10, v11, v01)
			Triangle t2(v10, v11, v01);

			m_Triangles.push_back(t1);
			m_Triangles.push_back(t2);
		}
	}

	m_VertexBufferData.reserve(m_Triangles.size() * 3);

	for (const auto& tri : m_Triangles)
	{
		VERTEX_3D v0, v1, v2;

		v0.Position = tri.GetA().ToXMFLOAT3();
		v1.Position = tri.GetB().ToXMFLOAT3();
		v2.Position = tri.GetC().ToXMFLOAT3();

		Vector3 normal = tri.GetNormal();
		v0.Normal = v1.Normal = v2.Normal = normal.ToXMFLOAT3();

		v0.Diffuse = v1.Diffuse = v2.Diffuse = XMFLOAT4(1, 1, 1, 1);

		m_VertexBufferData.push_back(v0);
		m_VertexBufferData.push_back(v1);
		m_VertexBufferData.push_back(v2);
	}

	// 頂点バッファ作成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = static_cast<UINT>(sizeof(VERTEX_3D) * m_VertexBufferData.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = m_VertexBufferData.data();

	HRESULT hr = Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
}

void CollisionMesh::UpdateVertex3D(const VERTEX_3D(*vertices)[21])
{
	// --- ワールド行列 ---
	XMMATRIX matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	XMMATRIX matRot = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	XMMATRIX matTrans = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	XMMATRIX world = matScale * matRot * matTrans;

	size_t index = 0;

	for (int x = 0; x < 20; x++)
	{
		for (int z = 0; z < 20; z++)
		{
			// --- ローカル頂点 ---
			Vector3 L00 = XMFLOAT3ToVector3(vertices[x][z].Position);
			Vector3 L10 = XMFLOAT3ToVector3(vertices[x + 1][z].Position);
			Vector3 L01 = XMFLOAT3ToVector3(vertices[x][z + 1].Position);
			Vector3 L11 = XMFLOAT3ToVector3(vertices[x + 1][z + 1].Position);

			// --- ワールド頂点（衝突判定用） ---
			Vector3 W00 = L00.Transform(world);
			Vector3 W10 = L10.Transform(world);
			Vector3 W01 = L01.Transform(world);
			Vector3 W11 = L11.Transform(world);

			//-----------------------------------------
			// ★ 衝突判定用の三角形は World 座標で更新
			//-----------------------------------------
			m_Triangles[index].Update(W00, W10, W01);

			//-----------------------------------------
			// ★ GPU用の頂点バッファは Local 座標のまま
			//-----------------------------------------
			m_VertexBufferData[index * 3 + 0].Position = L00.ToXMFLOAT3();
			m_VertexBufferData[index * 3 + 1].Position = L10.ToXMFLOAT3();
			m_VertexBufferData[index * 3 + 2].Position = L01.ToXMFLOAT3();

			Vector3 n1 = (L10 - L00).cross(L01 - L00).normalized();
			m_VertexBufferData[index * 3 + 0].Normal =
				m_VertexBufferData[index * 3 + 1].Normal =
				m_VertexBufferData[index * 3 + 2].Normal = n1.ToXMFLOAT3();

			index++;

			// --- 2枚目 ---
			m_Triangles[index].Update(W10, W11, W01);

			m_VertexBufferData[index * 3 + 0].Position = L10.ToXMFLOAT3();
			m_VertexBufferData[index * 3 + 1].Position = L11.ToXMFLOAT3();
			m_VertexBufferData[index * 3 + 2].Position = L01.ToXMFLOAT3();

			Vector3 n2 = (L11 - L10).cross(L01 - L10).normalized();
			m_VertexBufferData[index * 3 + 0].Normal =
				m_VertexBufferData[index * 3 + 1].Normal =
				m_VertexBufferData[index * 3 + 2].Normal = n2.ToXMFLOAT3();

			index++;
		}
	}

	// --- GPUへ転送 ---
	auto context = Renderer::GetDeviceContext();
	D3D11_MAPPED_SUBRESOURCE msr;
	if (SUCCEEDED(context->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr)))
	{
		memcpy(msr.pData, m_VertexBufferData.data(),
			sizeof(VERTEX_3D) * m_VertexBufferData.size());
		context->Unmap(m_VertexBuffer, 0);
	}
}

void CollisionMesh::Draw()
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

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	auto context = Renderer::GetDeviceContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::SetRasterSolid(false);
	context->Draw(static_cast<UINT>(m_VertexBufferData.size()), 0);
	Renderer::SetRasterSolid(true);

}
