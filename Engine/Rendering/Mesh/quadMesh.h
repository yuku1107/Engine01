/*==============================================================================

    四角形メッシュ[QuadMesh.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/21
==============================================================================*/
#ifndef QUADMESH_H
#define QUADMESH_H
#include <vector>
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "mesh.h"

#define MAX_INSTANCE_COUNT (100)

class QuadMesh :public Mesh
{
public:
    //初期化
    void Init() override
    {

        // 頂点配列作成
        VERTEX_3D vertex[4];

        // 左上、右上、左下、右下の座標計算
        float halfWidth = 0.5f;
        float Height = 1.0f;

        vertex[0].Position = XMFLOAT3(0.0f - halfWidth, 0.0f + Height, 0.0f);
        vertex[1].Position = XMFLOAT3(0.0f + halfWidth, 0.0f + Height, 0.0f);
        vertex[2].Position = XMFLOAT3(0.0f - halfWidth, 0.0f, 0.0f);
        vertex[3].Position = XMFLOAT3(0.0f + halfWidth, 0.0f, 0.0f);

        for (int i = 0; i < 4; i++)
        {
            vertex[i].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
            vertex[i].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);
        vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);
        vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);
        vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

        // 頂点バッファ生成
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

    void Draw(const Vector3& position, Vector3& rotation, const Vector3& scale, bool billboard, bool pitch = true, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), const int frame = 0, const int x = 1, const int y = 1)
    {
        //頂点データ書き換え
        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        float tw = 1.0f / (float)x;
        float th = 1.0f / (float)y;
        float tx = (frame % x) * tw;
        float ty = (frame / y) * th;

        vertex[0].Position = XMFLOAT3(-0.5f, 1.0f, 0.0f);
        vertex[0].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[0].TexCoord = XMFLOAT2(tx, ty);

        vertex[1].Position = XMFLOAT3(0.5f, 1.0f, 0.0f);
        vertex[1].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[1].TexCoord = XMFLOAT2(tx + tw, ty);

        vertex[2].Position = XMFLOAT3(-0.5f, 0.0f, 0.0f);
        vertex[2].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[2].TexCoord = XMFLOAT2(tx, ty + th);

        vertex[3].Position = XMFLOAT3(0.5f, 0.0f, 0.0f);
        vertex[3].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[3].TexCoord = XMFLOAT2(tx + tw, ty + th);

        XMMATRIX world, MaScale, MaTranslation, MaRotation;

        Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

        if (billboard)
        {
            if (pitch)
            {
                XMMATRIX view = camera->GetViewMatrix();

                //ビューの逆行列
                XMMATRIX invView;
                invView = XMMatrixInverse(nullptr, view);//逆行列
                invView.r[3].m128_f32[0] = 0.0f;
                invView.r[3].m128_f32[1] = 0.0f;
                invView.r[3].m128_f32[2] = 0.0f;

                // ワールド行列を作成
                MaScale = XMMatrixScaling(scale.x, scale.y, scale.z);
                MaTranslation = XMMatrixTranslation(position.x, position.y, position.z);

                world = MaScale * invView * MaTranslation;

                // invView からオイラー角を取得する
                XMMATRIX rotMat = invView;

                // ビルボード回転（オイラー角）を計算
                Vector3 billboardRot;
                billboardRot.x = asinf(-rotMat.r[2].m128_f32[1]);                          // Pitch（X）
                billboardRot.y = atan2f(rotMat.r[2].m128_f32[0], rotMat.r[2].m128_f32[2]); // Yaw（Y）
                billboardRot.z = atan2f(rotMat.r[0].m128_f32[1], rotMat.r[1].m128_f32[1]); // Roll（Z）

                // rotation に代入（呼び出し元で参照渡しされている場合）
                rotation = billboardRot;
            }
            else
            {

                Vector3 dir = camera->GetDirection();
                dir.y = 0.0f;

                float len2 = dir.x * dir.x + dir.z * dir.z;
                if (len2 < 0.00001f)
                {
                    dir = Vector3(0, 0, 1);
                }
                else
                {
                    dir.normalized();
                }

                XMVECTOR eye = XMVectorZero();
                XMVECTOR at = XMVectorSet(dir.x, dir.y, dir.z, 0);
                XMVECTOR up = XMVectorSet(0, 1, 0, 0);

                XMMATRIX billboardRot = XMMatrixInverse(nullptr, XMMatrixLookToLH(eye, at, up));

                MaScale = XMMatrixScaling(scale.x, scale.y, scale.z);
                MaTranslation = XMMatrixTranslation(position.x, position.y, position.z);

                world = MaScale * billboardRot * MaTranslation;
            }

        }
        else
        {
            // ワールド行列を作成
            MaScale = XMMatrixScaling(scale.x, scale.y, scale.z);
            MaRotation = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
            MaTranslation = XMMatrixTranslation(position.x, position.y, position.z);

            world = MaScale * MaRotation * MaTranslation;
        }

        // ワールド行列をセット（シェーダー用）
        Renderer::SetWorldMatrix(world);

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

    void DrawInstance(const Vector3& position, Vector3& rotation, const Vector3& scale, int num, ID3D11ShaderResourceView* posbuffer, ID3D11ShaderResourceView* scalebuffer, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),const int frame = 0, const int x = 1, const int y = 1)
    {
        //頂点データ書き換え
        D3D11_MAPPED_SUBRESOURCE msr;
        Renderer::GetDeviceContext()->Map(m_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

        float tw = 1.0f / (float)x;
        float th = 1.0f / (float)y;
        float tx = (frame % x) * tw;
        float ty = (frame / y) * th;

        vertex[0].Position = XMFLOAT3(-0.5f, 1.0f, 0.0f);
        vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[0].TexCoord = XMFLOAT2(tx, ty);

        vertex[1].Position = XMFLOAT3(0.5f, 1.0f, 0.0f);
        vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[1].TexCoord = XMFLOAT2(tx + tw, ty);

        vertex[2].Position = XMFLOAT3(-0.5f, 0.0f, 0.0f);
        vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[2].TexCoord = XMFLOAT2(tx, ty + th);

        vertex[3].Position = XMFLOAT3(0.5f, 0.0f, 0.0f);
        vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 1.0f);
        vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
        vertex[3].TexCoord = XMFLOAT2(tx + tw, ty + th);

        XMMATRIX world, MaScale, MaTranslation, MaRotation;

        Camera* camera = Manager::GetScene()->GetGameObject<Camera>();

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

        // 頂点バッファ設定
        UINT stride = sizeof(VERTEX_3D);
        UINT offset = 0;
        Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
        
        //ストラクチャードバッファ設定
        Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &posbuffer);
        Renderer::GetDeviceContext()->VSSetShaderResources(3, 1, &scalebuffer);

        // プリミティブトポロジ設定
        Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

        // 描画
        Renderer::GetDeviceContext()->DrawInstanced(4, num, 0, 0);
    }

};

#endif //QUADMESH_H
