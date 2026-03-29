/*==============================================================================

    êVóvëfè–âÓÉGÉäÉA[discoveryArea.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/09
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Serialization/serializer.h"
#include "ThirdParty/ImGui/imgui.h"
#include "discoveryArea.h"
#include "player.h"

void DiscoveryArea::Init()
{
    AddComponent<Box>()->Set(this);

    m_Scale = Vector3(5.0f, 5.0f, 5.0f);
    m_Tag = "DISCOVERYAREA";
}

void DiscoveryArea::UninitSelf()
{

}

void DiscoveryArea::UpdateSelf()
{
    if (m_Player == nullptr)
    {
        Player* player = Manager::GetScene()->GetGameObject<Player>();

        if (player != nullptr)
        {
            m_Player = player;
        }
    }

    if (m_Window == nullptr)
    {
        DiscoveryWindow* window = Manager::GetScene()->GetGameObject<DiscoveryWindow>();

        if (window != nullptr)
        {
            m_Window = window;
        }
    }

    if (!m_Player || m_Player->GetDestroy()) return;

    bool inside = IsInside(m_Player->GetPosition());

    if (inside && !m_PlayerInside)
    {
        m_Window->ChangeShowState(m_ShowState);
    }

    m_PlayerInside = inside;
}

void DiscoveryArea::DrawSelf()
{
    
}

void DiscoveryArea::DrawInspector()
{
    int current = static_cast<int>(m_ShowState);

    if (ImGui::BeginCombo("ShowState", WINDOW_SHOW_STATE_NAMES[current]))
    {
        for (int i = 0; i < WINDOW_SHOW_STATE_COUNT; i++)
        {
            bool selected = (current == i);

            if (ImGui::Selectable(WINDOW_SHOW_STATE_NAMES[i], selected))
            {
                current = i;
            }

            if (selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    m_ShowState = static_cast<WINDOW_SHOW_STATE>(current);
}

bool DiscoveryArea::IsInside(const Vector3& point) const
{
    Vector3 half = m_Scale * 0.5f;

    Vector3 min = m_Position - half;
    Vector3 max = m_Position + half;

    return (point.x >= min.x && point.x <= max.x 
        && point.y >= min.y && point.y <= max.y 
        && point.z >= min.z && point.z <= max.z);
}

void DiscoveryArea::SetState(WINDOW_SHOW_STATE state)
{
    m_ShowState = state;
}

void DiscoveryArea::Serialize(Serializer& s)
{
	GameObject::Serialize(s);
	s.WriteInt("showState", static_cast<int>(m_ShowState));
}

void DiscoveryArea::Deserialize(Serializer& s)
{
	GameObject::Deserialize(s);
	m_ShowState = static_cast<WINDOW_SHOW_STATE>(s.ReadInt("showState"));
}
