/*==============================================================================

    ç¿ïWån[coordinate2D.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/09/26
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Rendering/sprite2d.h"
#include "Engine/Components/shader.h"
#include "coordinate2D.h"

#define NUM_VERTEX 4	//ïKóvÇ»í∏ì_ÇÃêî

void Coordinate2D::Init()
{
    m_Position.z = 100.0f;

    m_TextureX = Texture::Get("coordinate_x");
    m_TextureY = Texture::Get("coordinate_y");

    m_SelectionSwitch = false;

    m_DrawEnable = false;

    m_Tag = "COORDINATE2D";

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);
}

void Coordinate2D::UninitSelf()
{
    
}

void Coordinate2D::UpdateSelf()
{
    if (!m_Target)
    {
        m_SelectionSwitch = false;
        return;
    }

    m_Position = m_Target->GetPosition();

    m_Pos[0] = m_Position;
    m_Scl[0] = Vector3(200.0f, 30.0f, 1.0f);

    m_Pos[1] = m_Position;
    m_Scl[1] = Vector3(30.0f, 200.0f, 1.0f);

    m_DrawEnable = Manager::GetUIMode();
}

void Coordinate2D::DrawSelf()
{
    if (m_Target)
    {
        Sprite2D::Draw(m_Pos[0], m_Scl[0], m_Rot[0], m_TextureX, m_MaterialColor, 0, 1, 1);
        Sprite2D::Draw(m_Pos[1], m_Scl[1], m_Rot[1], m_TextureY, m_MaterialColor, 0, 1, 1);
    }
}

void Coordinate2D::StartAxisDrag(int axisIndex, const Vector3& mousePos)
{
    if (!m_Target) return;

    m_SelectionNum = axisIndex;
    m_SelectionSwitch = true;

    m_ClickOffset = mousePos - m_Target->GetPosition();
}

void Coordinate2D::EndAxisDrag()
{
    m_SelectionSwitch = false;
}

void Coordinate2D::ApplyAxisMovement(const Vector3& mousePos)
{
    if (!m_Target || !m_SelectionSwitch)
        return;

    Vector3 targetPos = m_Target->GetPosition();

    switch (m_SelectionNum)
    {
    case 0: // Xé≤
        m_Target->SetPosition({mousePos.x - m_ClickOffset.x,targetPos.y,targetPos.z});
        break;

    case 1: // Yé≤
        m_Target->SetPosition({targetPos.x,mousePos.y - m_ClickOffset.y,targetPos.z});
        break;
    }
}

bool Coordinate2D::GetCollision(Vector3 pos)
{
    for (int i = 0; i < 2; i++)
    {
        if (pos.x > (m_Pos[i].x) &&
            pos.x < (m_Pos[i].x + m_Scl[i].x) &&
            pos.y >(m_Pos[i].y) &&
            pos.y < (m_Pos[i].y + m_Scl[i].y))
        {
            m_SelectionNum = i;
            return true;
        }
    }

    return false;
}