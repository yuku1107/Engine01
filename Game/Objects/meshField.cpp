
#include "main.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/shape.h"
#include "Engine/Serialization/serializer.h"
#include "meshField.h"

float g_FieldHeight[21][21] =
{
	{3.0f,3.0f,4.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,4.0f,3.0f,3.0f,3.0f,4.0f,3.0f},
	{2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,4.0f,2.0f,2.0f,2.0f,4.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f},
	{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f},
	{-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
	{2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f},
	{2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f},
	{3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f},
};

float g_TitleFieldHeight[21][21] =
{
	{3.0f,3.0f,4.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,4.0f,3.0f,3.0f,3.0f,4.0f,3.0f},
	{2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,4.0f,2.0f,2.0f,2.0f,4.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f},
	{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,1.0f,0.0f,0.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f},
	{2.0f,2.0f,2.0f,1.0f,0.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-1.0f,0.0f,0.0f},
	{2.0f,2.0f,2.0f,0.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-3.0f,-3.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-3.0f,-3.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-3.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-2.0f,0.5f,0.5f,-2.0f,-2.0f,-2.0f,-2.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,-1.0f,-3.0f,-3.0f,-3.0f,-2.0f,-2.0f,0.5f,0.5f,-2.0f,-2.0f,-2.0f,-2.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,0.0f,-1.0f,-3.0f,-3.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{3.0f,3.0f,3.0f,0.0f,0.0f,0.0f,-3.0f,-3.0f,-3.0f,-2.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-3.0f,-2.0f,-1.0f,0.0f,0.0f},
	{2.0f,2.0f,2.0f,1.0f,0.0f,0.0f,0.0f,-1.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,-2.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,0.0f,0.0f,0.0f},
	{0.0f,0.0f,1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f},
	{1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f},
	{2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f},
	{2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f,2.0f},
	{3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f,3.0f},
};


void MeshField::Init()
{
	// 頂点バッファ生成
	{
		for (int x = 0; x <= 20; x++)
		{
			for (int z = 0; z <= 20; z++)
			{
				float y = 0.0f;

				switch (m_type)
				{
				case 0:
					y = g_TitleFieldHeight[x][z];
					break;
				case 1:
					y = g_FieldHeight[x][z];
					break;
				}
				m_Vertex[x][z].Position = XMFLOAT3((x - 10) * 5.0f, y, (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);//法線ベクトル
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x * 0.5f, z * 0.5f);

				if (y < 0.0f)
				{
					m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
				}
			}
		}

		m_Vertex[10][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[10][11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[10][12].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[11][11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[11][12].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

		for (int x = 1; x < 20; x++)
		{
			for (int z = 1; z < 20; z++)
			{
				Vector3 vx, vz, vn;
				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

				vn = crossProduct(vz, vx);
				vn.normalize();

				m_Vertex[x][z].Normal.x = vn.x;
				m_Vertex[x][z].Normal.y = vn.y;
				m_Vertex[x][z].Normal.z = vn.z;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}




	// インデックスバッファ生成
	{
		unsigned int index[(22 * 2) * 20 - 2 ];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}

			if (x == 19)
				break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	m_Texture = Texture::Get("field");
	m_TextureSoil = Texture::Get("soil");

	m_Tag = "MESHFIELD";

	AddComponent<CollisionMesh>()->Set(this, Shape::Block);

	m_ShaderName = "field";
	AddComponent<Shader>()->Set(this);

}

void MeshField::Init(int type)
{
	m_type = type;
	// 頂点バッファ生成
	{
		for (int x = 0; x <= 20; x++)
		{
			for (int z = 0; z <= 20; z++)
			{
				float y = 0.0f;

				switch (m_type)
				{
				case 0:
					y = g_TitleFieldHeight[x][z];
					break;
				case 1:
					y = g_FieldHeight[x][z];
					break;
				}

				m_Vertex[x][z].Position = XMFLOAT3((x - 10) * 5.0f, y, (z - 10) * -5.0f);
				m_Vertex[x][z].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);//法線ベクトル
				m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
				m_Vertex[x][z].TexCoord = XMFLOAT2(x * 0.5f, z * 0.5f);

				if (y < 0.0f)
				{
					m_Vertex[x][z].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
				}
			}
		}

		m_Vertex[10][10].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[10][11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[10][12].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[11][11].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		m_Vertex[11][12].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

		for (int x = 1; x < 20; x++)
		{
			for (int z = 1; z < 20; z++)
			{
				Vector3 vx, vz, vn;
				vx.x = m_Vertex[x + 1][z].Position.x - m_Vertex[x - 1][z].Position.x;
				vx.y = m_Vertex[x + 1][z].Position.y - m_Vertex[x - 1][z].Position.y;
				vx.z = m_Vertex[x + 1][z].Position.z - m_Vertex[x - 1][z].Position.z;

				vz.x = m_Vertex[x][z - 1].Position.x - m_Vertex[x][z + 1].Position.x;
				vz.y = m_Vertex[x][z - 1].Position.y - m_Vertex[x][z + 1].Position.y;
				vz.z = m_Vertex[x][z - 1].Position.z - m_Vertex[x][z + 1].Position.z;

				vn = crossProduct(vz, vx);
				vn.normalize();

				m_Vertex[x][z].Normal.x = vn.x;
				m_Vertex[x][z].Normal.y = vn.y;
				m_Vertex[x][z].Normal.z = vn.z;
			}
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * 21 * 21;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_Vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}




	// インデックスバッファ生成
	{
		unsigned int index[(22 * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}

			if (x == 19)
				break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	m_Texture = Texture::Get("field");
	m_TextureSoil = Texture::Get("soil");

	m_Tag = "MESHFIELD";

	AddComponent<CollisionMesh>()->Set(this, Shape::Block);

	m_ShaderName = "field";
	AddComponent<Shader>()->Set(this);

}

void MeshField::UninitSelf()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();
}

void MeshField::UpdateSelf()
{

}

void MeshField::DrawSelf()
{
	//マトリクス設定
	XMMATRIX world, scale, rotation, trancelation;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	world = scale * rotation * trancelation;
	Renderer::SetWorldMatrix(world);

	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_VertexBuffer, &stride, &offset );

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer( 
		m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// マテリアル設定
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	material.TextureEnable = true;
	Renderer::SetMaterial( material );

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
	Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &m_TextureSoil);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	
	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);
}

float MeshField::GetHeight(Vector3 Position)
{
	int x, z;

	//ブロック番号算出
	x = static_cast<int>(Position.x / 5.0f + 10.0f);
	z = static_cast<int>(Position.z / (-5.0f) + 10.0f);

	Vector3 pos0, pos1, pos2, pos3;

	pos0 = XMFLOAT3ToVector3(m_Vertex[x][z].Position);
	pos1 = XMFLOAT3ToVector3(m_Vertex[x + 1][z].Position);
	pos2 = XMFLOAT3ToVector3(m_Vertex[x][z + 1].Position);
	pos3 = XMFLOAT3ToVector3(m_Vertex[x + 1][z + 1].Position);

	Vector3 v12, v1p;
	v12 = pos2 - pos1;
	v1p = Position - pos1;

	//外積
	float cy = crossProduct(v12, v1p)[1];

	float py;
	Vector3 n;

	if (cy > 0.0f)
	{
		//左上ポリゴン
		Vector3 v10;
		v10 = pos0 - pos1;

		//外積
		n = crossProduct(v10, v12);
	}
	else
	{
		//右下ポリゴン
		Vector3 v13;
		v13 = pos3 - pos1;

		//外積
		n = crossProduct(v12, v13);
	}

	//高さ取得
	py = -((Position.x - pos1.x) * n.x + (Position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

void MeshField::Serialize(Serializer& s)
{
	GameObject::Serialize(s);

	s.WriteInt("type", m_type);
}

void MeshField::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);

	m_type = s.ReadInt("type");
}


