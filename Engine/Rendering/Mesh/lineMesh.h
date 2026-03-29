/*==============================================================================

    線メッシュ[lineMesh.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/09
==============================================================================*/
#ifndef LINEMESH_H
#define LINEMESH_H

#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "mesh.h"

class LineMesh :public Mesh
{
private:
    float m_Width = 0.2f;
    Vector3 m_Start, m_End;
    Vector3 m_LineDir;
    Vector3 m_WidthVec;

public:
    // 初期化
    void Init() override
    {
        m_Start = Vector3(0.0f, 0.0f, 0.0f);
        m_End = Vector3(0.0f, 0.0f, 0.0f);
        m_LineDir = m_End - m_Start;
        
        // 頂点配列作成
        VERTEX_3D vertex[4];

        // 頂点バッファ生成（Initコードと同様）
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX_3D) * 4; //頂点バッファの量
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = vertex;

        Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
    }

    void Uninit() override
    {
        if (m_VertexBuffer)
        {
            m_VertexBuffer->Release(); m_VertexBuffer = nullptr;
        }

        if (m_IndexBuffer)
        {
            m_IndexBuffer->Release(); m_IndexBuffer = nullptr;
        }
    }

    void Draw(XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)) override
    {
        //マテリア設定
        MATERIAL material{};
        material.Diffuse = { color.x, color.y, color.z, color.w };
        material.TextureEnable = true;
        Renderer::SetMaterial(material);

        // 頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        // プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        // 描画
        Renderer::GetDeviceContext()->Draw(4, 0);
    }

    void Draw(const Vector3& start, const Vector3& end, float width = 0.2f, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

        m_Start = start;
        m_End = end;
        m_Width = width;

        m_LineDir = (m_End - m_Start).normalized();
        Vector3 cameraForward = camera->GetForward().normalized();

        m_WidthVec = m_LineDir.cross(cameraForward);
        if (m_WidthVec.lengthSq() < 1e-6f)
        {
            m_WidthVec = Vector3(0.0f, 1.0f, 0.0f);
        }

        m_WidthVec = m_WidthVec.normalized() * (m_Width * 0.5f);


        //頂点データ書き換え
        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        Vector3 v0 = m_Start + m_WidthVec;
        Vector3 v1 = m_Start - m_WidthVec;
        Vector3 v2 = m_End + m_WidthVec;
        Vector3 v3 = m_End - m_WidthVec;

        // 定数バッファ更新
        XMMATRIX world = XMMatrixIdentity();

        Renderer::SetWorldMatrix(world);

        vertex[0].Position = v0.ToXMFLOAT3();
        vertex[1].Position = v1.ToXMFLOAT3();
        vertex[2].Position = v2.ToXMFLOAT3();
        vertex[3].Position = v3.ToXMFLOAT3();


        for (int i = 0; i < 4; i++)
        {
            vertex[i].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
            vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        vertex[0].TexCoord = XMFLOAT2(0.0f, 0.5f);
        vertex[1].TexCoord = XMFLOAT2(1.0f, 0.5f);
        vertex[2].TexCoord = XMFLOAT2(0.0f, 0.5f);
        vertex[3].TexCoord = XMFLOAT2(1.0f, 0.5f);

        Renderer::GetDeviceContext()->Unmap(m_VertexBuffer, 0);

        //マテリア設定
        MATERIAL material{};
        material.Diffuse = { color.x, color.y, color.z, color.w };
        material.TextureEnable = true;
        Renderer::SetMaterial(material);

        // 頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        // プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        // 描画
        Renderer::GetDeviceContext()->Draw(4, 0);

    }

};

#endif //LINEMESH_H
