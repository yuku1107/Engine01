/*==============================================================================

	2D描画[sprite2D.h]
															 Author:郭　哲宇
															 Date  :2026/02/13
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/sprite2d.h"

Mesh2D Sprite2D::m_Mesh2D;

void Sprite2D::Init()
{
    m_Mesh2D.Init();
}

void Sprite2D::Uninit()
{
    m_Mesh2D.Uninit();
}

void Sprite2D::Draw(Vector3 position, Vector3 size, Vector3 rotation, ID3D11ShaderResourceView* tex, XMFLOAT4 color)
{
    Renderer::SetWorldViewProjection2D();

    // テクスチャ
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

    // 中心基準回転
    XMMATRIX moveToCenter = XMMatrixTranslation(-0.5f, -0.5f, 0.0f);
    XMMATRIX rot = XMMatrixRotationZ(rotation.x);
    XMMATRIX moveBack = XMMatrixTranslation(0.5f, 0.5f, 0.0f);

    XMMATRIX scale = XMMatrixScaling(size.x, size.y, 1.0f);
    XMMATRIX trans = XMMatrixTranslation(position.x, position.y, 0.0f);

    XMMATRIX world = moveToCenter * rot * moveBack * scale * trans;

    Renderer::SetWorldMatrix(world);

    MATERIAL mat{};
    mat.Diffuse = { color.x,color.y,color.z,color.w };
    mat.TextureEnable = true;
    Renderer::SetMaterial(mat);

    m_Mesh2D.Draw();
}

void Sprite2D::Draw(Vector3 position, Vector3 size, Vector3 rotation, ID3D11ShaderResourceView* tex, XMFLOAT4 color, const int frame, const int x, const int y)
{
    Renderer::SetWorldViewProjection2D();

    float tw = 1.0f / x;
    float th = 1.0f / y;
    float tx = (frame % x) * tw;
    float ty = (frame / x) * th;

    Renderer::SetSpriteUV(tx, ty, tw, th);

    // テクスチャ
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

    // 中心基準回転
    XMMATRIX moveToCenter = XMMatrixTranslation(-0.5f, -0.5f, 0.0f);
    XMMATRIX rot = XMMatrixRotationZ(rotation.x);
    XMMATRIX moveBack = XMMatrixTranslation(0.5f, 0.5f, 0.0f);

    XMMATRIX scale = XMMatrixScaling(size.x, size.y, 1.0f);
    XMMATRIX trans = XMMatrixTranslation(position.x, position.y, 0.0f);

    XMMATRIX world = moveToCenter * rot * moveBack * scale * trans;

    Renderer::SetWorldMatrix(world);

    MATERIAL mat{};
    mat.Diffuse = { color.x,color.y,color.z,color.w };
    mat.TextureEnable = true;
    Renderer::SetMaterial(mat);

    m_Mesh2D.Draw();

}

