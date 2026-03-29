/*==============================================================================

    シーン描画[scenePolygon.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/11
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Serialization/serializer.h"
#include "player.h"

#define NUM_VERTEX 4	//必要な頂点の数

void ScenePolygon2D::Init()
{
    VERTEX_3D vertex[NUM_VERTEX];

    vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = m_MaterialColor;
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(SCREEN_WIDTH, 0.0f, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = m_MaterialColor;
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(0.0f, SCREEN_HEIGHT, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = m_MaterialColor;
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = m_MaterialColor;
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    // ★頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX; //頂点バッファの量
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0; //低速

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //テクスチャ読込
    m_Texture = Renderer::GetPETexture(0);

    m_Tag = "SCENEPOLYGON2D";

    m_ShaderName = "scene";
    AddComponent<Shader>()->Set(this);

    StartFadeIn();
}

void ScenePolygon2D::Init(int num)
{
    VERTEX_3D vertex[NUM_VERTEX];

    vertex[0].Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = m_MaterialColor;
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(SCREEN_WIDTH, 0.0f, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = m_MaterialColor;
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(0.0f, SCREEN_HEIGHT, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = m_MaterialColor;
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = m_MaterialColor;
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    // ★頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX; //頂点バッファの量
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0; //低速

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //テクスチャ読込
    m_Texture = Renderer::GetPETexture(num);

    m_Tag = "SCENEPOLYGON2D";
    m_ShaderName = "scene";
    AddComponent<Shader>()->Set(this);
}

void ScenePolygon2D::Init(float x, float y, float Width, float Height, const std::string& TextureName)
{
    //頂点データ
    VERTEX_3D vertex[NUM_VERTEX];

    vertex[0].Position = XMFLOAT3(x, y, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(x + Width, y, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(x, y + Height, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(x + Width, y + Height, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    // ★頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX; //頂点バッファの量
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0; //低速

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //テクスチャ読込
    m_Texture = Texture::Get(TextureName);

    m_Tag = "SCENEPOLYGON2D";

    m_ShaderName = "scene";
    AddComponent<Shader>()->Set(this);
}

void ScenePolygon2D::Init(float x, float y, float Width, float Height, ID3D11ShaderResourceView* Texture)
{
    //頂点データ
    VERTEX_3D vertex[NUM_VERTEX];

    vertex[0].Position = XMFLOAT3(x, y, 0.0f);
    vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

    vertex[1].Position = XMFLOAT3(x + Width, y, 0.0f);
    vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

    vertex[2].Position = XMFLOAT3(x, y + Height, 0.0f);
    vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

    vertex[3].Position = XMFLOAT3(x + Width, y + Height, 0.0f);
    vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
    vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
    vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

    // ★頂点バッファ生成
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX; //頂点バッファの量
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0; //低速

    D3D11_SUBRESOURCE_DATA sd{};
    sd.pSysMem = vertex;

    Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

    //テクスチャ読込
    m_Texture = Texture;

    m_Tag = "SCENEPOLYGON2D";

    m_ShaderName = "scene";
    AddComponent<Shader>()->Set(this);
}

void ScenePolygon2D::UninitSelf()
{
    m_VertexBuffer->Release();
}

void ScenePolygon2D::UpdateSelf()
{
    float deltaTime = 1.0f / 60.0f;

    Player* player = Manager::GetScene()->GetGameObject<Player>();
    
    if (player)
    {
        m_Parameter.x = (1.0f - (float)player->GetLife() / (float)player->GetMaxLife());
        m_Parameter.y += deltaTime;
        if (m_Parameter.y > 1000.0f)
        {
            m_Parameter.y = 0.0f;
        }
    }
    else
    {
        m_Parameter.x = 0.0f;
    }

    if (m_FadeState == FadeState::FadingIn)
    {
        m_Parameter.w -= m_FadeSpeed * deltaTime;

        if (m_Parameter.w <= 0.0f)
        {
            m_Parameter.w = 0.0f;
            m_FadeState = FadeState::CompletedIn;
        }
    }
    else if (m_FadeState == FadeState::FadingOut)
    {
        m_Parameter.w += m_FadeSpeed * deltaTime;

        if (m_Parameter.w >= 1.0f)
        {
            m_Parameter.w = 1.0f;
            m_FadeState = FadeState::CompletedOut;
        }
    }
}

void ScenePolygon2D::DrawSelf()
{
    Renderer::SetParameter(m_Parameter);

    //マトリクス設定
    Renderer::SetWorldViewProjection2D();

    //マトリクス設定
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, 0.0f);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    //マテリア設定
    MATERIAL material{};
    material.Diffuse = m_MaterialColor;
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    //頂点バッファ設定
    UINT stride = sizeof(VERTEX_3D);
    UINT offset = 0;
    Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

    //テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    //プリミティブトポロジ設定
    Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    Renderer::SetBlendEnable(false);
    //ポリゴン描画
    Renderer::GetDeviceContext()->Draw(NUM_VERTEX, 0);

    Renderer::SetBlendEnable(true);

}

void ScenePolygon2D::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);

	StartFadeIn();
}
