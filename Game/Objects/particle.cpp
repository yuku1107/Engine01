/*==============================================================================

    パーティクル[particle.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/01
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/Mesh/QuadMesh.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "particle.h"

#define NUM_VERTEX (4)

void Particle::Init()
{
    m_QuadMesh = new QuadMesh;
    m_QuadMesh->Init();

    m_Texture = Texture::Get("particle");

    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        m_Particle[i].Enable = false;
        m_Particle[i].OneTime = false;
    }

    m_Scale = Vector3(0.5f, 0.5f, 0.5f);
    m_Dir = Vector3(0.0f, 0.0f, 0.0f);
    m_Tag = "PARTICLE";
    m_Color = { 1.0f,0.0f,0.0f };
    m_Mode = Mode_RtoG;
    m_DestoryCount = 300;

    m_PositionBuffer.Create(PARTICLE_MAX);
    m_ScaleBuffer.Create(PARTICLE_MAX);

    m_ShaderName = "particle";
    AddComponent<Shader>()->Set(this);
}


void Particle::Init(Vector3 pos, Vector3 dir)
{
    m_QuadMesh = new QuadMesh;
    m_QuadMesh->Init();

    m_Texture = Texture::Get("particle");

    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        m_Particle[i].Enable = false;
        m_Particle[i].OneTime = false;
    }

    m_Scale = Vector3(0.2f, 0.2f, 0.2f);
    m_Position = pos;
    m_Dir = dir;
    m_Power = m_Dir.length();
    m_Color = { 1.0f,0.0f,0.0f };
    m_Mode = Mode_RtoG;

    m_PositionBuffer.Create(PARTICLE_MAX);
    m_ScaleBuffer.Create(PARTICLE_MAX);

    m_Tag = "PARTICLE";
    m_DestoryCount = 180;
}

void Particle::UninitSelf()
{
    m_QuadMesh->Uninit();
    delete m_QuadMesh;
}

void Particle::UpdateSelf()
{
    int count = 2;
    int num = 0;

    switch (m_Type)
    {
    case 0:

        for (int i = 0; i < PARTICLE_MAX; i++)
        {
            if (m_Particle[i].Enable == false)
            {
                m_Particle[i].Enable = true;
                m_Particle[i].Life = 90;
                m_Particle[i].Position = m_Position;

                if (m_Dir.x == 0 && m_Dir.z == 0)
                {
                    m_Particle[i].Velocity.x = (rand() % 100 - 50) / 5000.0f;
                    m_Particle[i].Velocity.y = (rand() % 100 + 50) / 2000.0f;
                    m_Particle[i].Velocity.z = (rand() % 100 - 50) / 5000.0f;
                }
                else
                {
                    m_Particle[i].Velocity.x = (50 * m_Dir.x) / (15000.0f / m_Power);
                    m_Particle[i].Velocity.y = (rand() % 100 + 50) / 500.0f;
                    m_Particle[i].Velocity.z = (50 * m_Dir.z) / (15000.0f / m_Power);
                }

                count--;

                if (count == 0)
                {
                    break;
                }
            }

        }
        break;

    case 1:

        for (int i = 0; i < 50; i++)
        {
            if (m_Particle[i].OneTime == false)
            {
                if (m_Particle[i].Enable == false)
                {
                    m_Particle[i].Enable = true;
                    m_Particle[i].OneTime = true;
                    m_Particle[i].Life = 90;
                    m_Particle[i].Position = m_Position;

                    m_Dir.x = (rand() % 200 - 100) / 100.0f;
                    m_Dir.y = (rand() % 200) / 100.0f;
                    m_Dir.z = (rand() % 200 - 100) / 100.0f;

                    m_Dir = m_Dir.normalized();

                    float power = (rand() % 100) / 20.0f;

                    m_Particle[i].Velocity = m_Dir * power;


                    count--;
                    num = i;
                    if (count == 0)
                    {
                        break;
                    }
                }
            }
        }
        break;
    }


    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        if (m_Particle[i].Enable == true)
        {

            if (m_Dir.x == 0 && m_Dir.z == 0)
            {
                m_Particle[i].Velocity += Vector3(0.0f, -0.001f, 0.0f);
            }
            else
            {
                m_Particle[i].Velocity += Vector3(0.0f, -0.01f, 0.0f);
            }

            m_Particle[i].Position += m_Particle[i].Velocity;

            if (m_Particle[i].Position.y < 0.0f)
            {
                m_Particle[i].Velocity.y *= -0.7f;
            }

            m_Particle[i].Life--;

            if (m_Particle[i].Life == 0)
            {
                m_Particle[i].Enable = false;
            }
        }
    }

    switch (m_Mode)
    {
    case Mode_RtoG:
        m_Color.x -= ColorCount;
        m_Color.y += ColorCount;

        if (m_Color.y >= 1.0f)
        {
            m_Color.x = 0.0f;
            m_Color.y = 1.0f;
            m_Mode = Mode_GtoB;
        }

        break;
    case Mode_GtoB:
        m_Color.y -= ColorCount;
        m_Color.z += ColorCount;

        if (m_Color.z >= 1.0f)
        {
            m_Color.y = 0.0f;
            m_Color.z = 1.0f;
            m_Mode = Mode_BtoR;
        }
        break;
    case Mode_BtoR:
        m_Color.z -= ColorCount;
        m_Color.x += ColorCount;

        if (m_Color.x >= 1.0f)
        {
            m_Color.z = 0.0f;
            m_Color.x = 1.0f;
            m_Mode = Mode_RtoG;
        }
        break;
    }

    if (m_Particle[num].Enable == false && m_Particle[num].OneTime == true)
    {
        SetDestroy();
    }

    m_MaterialColor = XMFLOAT4(m_Color.x, m_Color.y, m_Color.z, 1.0f);

    int instanceCount = 0;

    for (int i = 0; i < PARTICLE_MAX; i++)
    {
        if (m_Particle[i].Enable)
        {
            m_InstancePosition[instanceCount] = m_Particle[i].Position;
            m_InstanceScale[instanceCount] = m_Scale;
            instanceCount++;
        }
    }

    m_PositionBuffer.Update(m_InstancePosition, instanceCount);
    m_ScaleBuffer.Update(m_InstanceScale, instanceCount);

    m_InstanceCount = instanceCount;
}

void Particle::DrawSelf()
{
    //テクスチャ設定
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
    
    Renderer::SetDepthEnable(false);
    Renderer::SetAddBlendEnable(true);

    m_QuadMesh->DrawInstance(m_Position, m_Rotation,m_Scale,m_InstanceCount,m_PositionBuffer.GetSRV(),m_ScaleBuffer.GetSRV(),m_MaterialColor);

    Renderer::SetAddBlendEnable(false);
    Renderer::SetDepthEnable(true);
}
