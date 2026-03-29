/*==============================================================================

    草[grass.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/18
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/Mesh/quadMesh.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "grass.h"
#include "meshField.h"

void Grass::Init()
{
    m_QuadMesh = new QuadMesh();
    m_QuadMesh->Init();

    m_Texture = Texture::Get("grass");

    m_ShaderName = "tree";
    AddComponent<Shader>()->Set(this);

    auto meshField = Manager::GetScene()->GetGameObject<MeshField>();

    // ストラクチャードバッファ生成
    {
        Vector3* position = new Vector3[GRASS_MAX];
        Vector3* scale = new Vector3[GRASS_MAX];

        for (int i = 0; i < GRASS_MAX; i++)
        {
            bool loop = true;

            while (loop)
            {
                position[i].x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
                position[i].z = (float)rand() / RAND_MAX * 100.0f - 50.0f;

                position[i].y = meshField->GetHeight(position[i]);


                if (position[i].y >= 0.0f)
                {
                    loop = false;
                }

                scale[i] = Vector3(1.0f, 1.0f, 1.0f);
            }
        }

        // StructuredBuffer作成
        m_PositionBuffer.Create(GRASS_MAX, position);
        m_ScaleBuffer.Create(GRASS_MAX, scale);
        Vector3 timeData(m_Time, 0.0f, 0.0f);
        m_TimeBuffer.Create(1, &timeData);

        delete[] position;
        delete[] scale;
    }

    m_Tag = "GRASS";
}

void Grass::UninitSelf()
{
    m_QuadMesh->Uninit();
    delete m_QuadMesh;
}

void Grass::UpdateSelf()
{
    m_Time += 0.01f;
    Vector3 timeData(m_Time, 0.0f, 0.0f);
    m_TimeBuffer.Update(&timeData, 1);
}

void Grass::DrawSelf()
{
     //テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
    
    m_TimeBuffer.BindVS(4);

    Renderer::SetATCEnable(true);
    m_QuadMesh->DrawInstance(m_Position, m_Rotation, m_Scale, GRASS_MAX, m_PositionBuffer.GetSRV(), m_ScaleBuffer.GetSRV(), m_MaterialColor);
    Renderer::SetATCEnable(false);
}