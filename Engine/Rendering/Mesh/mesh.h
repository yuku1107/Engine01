/*==============================================================================

    メッシュ[Mesh.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/21
==============================================================================*/
#ifndef MESH_H
#define MESH_H
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Rendering/Renderer/renderer.h"


class Mesh
{
protected:
    ID3D11Buffer* m_VertexBuffer = nullptr;
    ID3D11Buffer* m_IndexBuffer = nullptr;

public:
    virtual void Init() {};
    virtual void Uninit() {};
    virtual void Draw(XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)) {};
    virtual void Draw(const Vector3& position, const Vector3& rotation, const Vector3& scale, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
    {
        XMMATRIX world, MaScale, MaTranslation, MaRotation;

        // ワールド行列を作成
        MaScale = XMMatrixScaling(scale.x, scale.y, scale.z);
        MaRotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
        MaTranslation = XMMatrixTranslation(position.x, position.y, position.z);

        world = MaScale * MaRotation * MaTranslation;

        // ワールド行列をセット（シェーダー用）
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

        //インデックスバッファ設定
        Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

        //プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        //ポリゴン描画
        Renderer::GetDeviceContext()->DrawIndexed(36, 0, 0);
    }
};


#endif //MESH_H