/*==============================================================================

    木[tree.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/18
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/Mesh/quadMesh.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "tree.h"
#include "meshField.h"

void Tree::Init()
{
    m_Scale = { 5.0f,5.0f,1.0f };

    m_QuadMesh = new QuadMesh();
    m_QuadMesh->Init();

    m_Texture = Texture::Get("tree");

    m_ShaderName = "tree";
    AddComponent<Shader>()->Set(this);

    auto meshField = Manager::GetScene()->GetGameObject<MeshField>();

    // ストラクチャードバッファ生成
    {
        Vector3* position = new Vector3[TREE_MAX];
        Vector3* scale = new Vector3[TREE_MAX];

        for (int i = 0; i < TREE_MAX; i++)
        {
            bool loop = true;

            while (loop)
            {
                position[i].x = (float)rand() / RAND_MAX * 100.0f - 50.0f;
                position[i].z = (float)rand() / RAND_MAX * 100.0f - 50.0f;

                position[i].y = meshField->GetHeight(position[i]);

                if (abs(position[i].x) >= 3.0f && abs(position[i].z) >= 3.0f && position[i].y >= 0.0f)
                {
                    loop = false;
                }


                scale[i].x = 3.0f + ((rand() % 300) / 100.0f);
                scale[i].y = 5.0f + ((rand() % 200) / 100.0f);
                scale[i].z = 1.0f;
            }
        }


        // StructuredBuffer作成
        m_PositionBuffer.Create(TREE_MAX, position);
        m_ScaleBuffer.Create(TREE_MAX, scale);
        Vector3 timeData(m_Time, 0.0f, 0.0f);
        m_TimeBuffer.Create(1, &timeData);

        delete[] position;
        delete[] scale;
    }

    m_Tag = "TREE";
}

void Tree::UninitSelf()
{
    m_QuadMesh->Uninit();
    delete m_QuadMesh;
}

void Tree::UpdateSelf()
{
    m_Time += 0.01f;
    Vector3 timeData(m_Time, 0.0f, 0.0f);
    m_TimeBuffer.Update(&timeData, 1);
}

void Tree::DrawSelf()
{
     //テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    m_TimeBuffer.BindVS(4);

    Renderer::SetATCEnable(true);
    m_QuadMesh->DrawInstance(m_Position, m_Rotation, m_Scale, TREE_MAX, m_PositionBuffer.GetSRV(), m_ScaleBuffer.GetSRV(), m_MaterialColor);
    Renderer::SetATCEnable(false);
}