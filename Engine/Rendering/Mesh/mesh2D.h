/*==============================================================================

	2Dメッシュクラス[Mesh2D.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/21
==============================================================================*/
#ifndef MESH2D_H
#define MESH2D_H

#include "mesh.h"

class Mesh2D :public Mesh
{
public:
    void Init() override
    {
        VERTEX_3D vertex[4];

        vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
        vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
        vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

        vertex[1].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
        vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
        vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

        vertex[2].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
        vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

        vertex[3].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
        vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
        vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

        // ★頂点バッファ生成
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(VERTEX_3D) * 4;
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
    }

    void Draw()
    {
        //頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        //プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        Renderer::SetBlendEnable(false);

        //ポリゴン描画 
        Renderer::GetDeviceContext()->Draw(4, 0);

        Renderer::SetBlendEnable(true);
    }

    void Draw(XMFLOAT4 color)
    {
        Renderer::SetWorldViewProjection2D();

        //マテリア設定
        MATERIAL material{};
        material.Diffuse = { color.x, color.y, color.z, color.w };
        material.TextureEnable = true;
        Renderer::SetMaterial(material);

        //頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        //プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        Renderer::SetBlendEnable(false);

        //ポリゴン描画 
        Renderer::GetDeviceContext()->Draw(4, 0);
        

        Renderer::SetBlendEnable(true);
    }

    void Draw(const Vector3& position, Vector3& rotation, const Vector3& scale, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), const int frame = 0, const int x = 1, const int y = 1)
    {
        //マトリクス設定
        Renderer::SetWorldViewProjection2D();

        //頂点データ書き換え
        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        float tw = 1.0f / (float)x;
        float th = 1.0f / (float)y;
        float tx = (frame % x) * tw;
        float ty = (frame / y) * th;

        vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
        vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[0].TexCoord = XMFLOAT2(tx, ty);

        vertex[1].Position = XMFLOAT3(1.0f, 0.0f, 0.0f);
        vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[1].TexCoord = XMFLOAT2(tx + tw, ty);

        vertex[2].Position = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[2].TexCoord = XMFLOAT2(tx, ty + th);

        vertex[3].Position = XMFLOAT3(1.0f, 1.0f, 0.0f);
        vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[3].TexCoord = XMFLOAT2(tx + tw, ty + th);

        //マトリクス設定
        XMMATRIX world, MaScale, MaRotation, MaTranslation, moveToCenter, moveBack;

        // 1. Quadを中心へ移動
        moveToCenter = XMMatrixTranslation(-0.5f, -0.5f, 0.0f);

        // 2. 回転（Z回転）
        MaRotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

        // 3. 中心へ戻す
        moveBack = XMMatrixTranslation(0.5f, 0.5f, 0.0f);

        // 4. スケール
        MaScale = XMMatrixScaling(scale.x, scale.y, 1.0f);

        // 5. 最終的な位置へ移動
        MaTranslation = XMMatrixTranslation(position.x, position.y, 0.0f);

        // ワールド行列
        world = moveToCenter * MaRotation * moveBack * MaScale * MaTranslation;


        Renderer::SetWorldMatrix(world);

        //マテリア設定
        MATERIAL material{};
        material.Diffuse = { color.x, color.y, color.z, color.w };
        material.TextureEnable = true;
        Renderer::SetMaterial(material);

        //頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

        //プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        Renderer::SetBlendEnable(false);

        //ポリゴン描画 
        Renderer::GetDeviceContext()->Draw(4, 0);

        Renderer::SetBlendEnable(true);
    }

};

#endif //MESH2D_H
