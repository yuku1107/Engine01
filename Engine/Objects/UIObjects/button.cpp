/*==============================================================================

    ボタン[button.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/11
==============================================================================*/
#include "main.h"
#include "Engine/Core/scene.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Core/Input/inputMapper.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Rendering/Mesh/mesh2D.h"
#include "ENgine/Components/shader.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/UIObjects/button.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Editor/ImGui/imGuiEngine.h"

void Button::Init()
{
    UI::Init();

    m_Tag = "BUTTON";
    m_DrawEnable = false;
}

void Button::UpdateSelf()
{
    if (Manager::GetCreaterMode() && Manager::GetUIMode())
    {
        m_DrawEnable = true;
    }
    else
    {
         auto* camera = Manager::GetScene()->GetGameObject<Camera>();

         if (camera)
         {
             if (camera->GetMode() == Camera::CameraMode::Clear_Mode || camera->GetMode() == Camera::CameraMode::Fixed_Mode)
             {
                 if (camera->IsBlending())
                 {
                     m_DrawEnable = false;
                 }
                 else
                 {
                     m_DrawEnable = true;
                 }
             }
             else
             {
                 m_DrawEnable = false;
             }
         }
    }

    if (m_DrawEnable && IsClicked() && !Manager::GetCreaterMode())
    {
        if (OnClickEvent) OnClickEvent();
    }
}

void Button::DrawInspector()
{

    const std::string& currentNormal = m_NormalTexture;

    // コンボボックス開始
    if (ImGui::BeginCombo("NormalTexture", currentNormal.c_str()))
    {
        for (auto& kv : Texture::GetAll())
        {
            const std::string& name = kv.first;
            bool selected = (currentNormal == name);

            if (ImGui::Selectable(name.c_str(), selected))
            {
                // 選択されたらポリゴンに適用
                if (name != currentNormal)
                    SetNormalTexture(name);
            }

            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    const std::string& currentHover = GetHoverTexture();

    // コンボボックス開始
    if (ImGui::BeginCombo("HoverTexture", currentHover.c_str()))
    {
        for (auto& kv : Texture::GetAll())
        {
            const std::string& name = kv.first;
            bool selected = (currentHover == name);

            if (ImGui::Selectable(name.c_str(), selected))
            {
                // 選択されたらポリゴンに適用
                if (name != currentHover)
                    SetHoverTexture(name);
            }

            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
}

bool Button::IsHover()
{
    Vector3 mousePos;

    if (Manager::GetUIMode())
    {
        if (Manager::GetGui()->GetHoveredWindowName() == "Scene")
        {
            mousePos = Vector3(Manager::GetGui()->GetLocalMousePos().x, Manager::GetGui()->GetLocalMousePos().y, 0.0f);
        }
        else
        {
            return false;
        }
    }
    else if(Manager::GetCreaterMode())
    {
        return false;
    }
    else
    {
        mousePos = Mouse_GetPosition();
    }

    if (mousePos.x > m_Position.x && mousePos.x < (m_Position.x + m_Scale.x))
    {
        if (mousePos.y > m_Position.y && mousePos.y < (m_Position.y + m_Scale.y))
        {
            return true;
        }
    }

    return false;
}

bool Button::IsClicked()
{
    return IsHover() && InputMapper::GetButtonTrigger(InputAction::Attack);
}

void Button::Serialize(Serializer& s)
{
    GameObject::Serialize(s);

	s.WriteString("normalTexture", m_NormalTexture);
	s.WriteString("hoverTexture", m_HoverTexture);
}

void Button::Deserialize(Serializer& s)
{
    GameObject::Deserialize(s);

    m_NormalTexture = s.ReadString("normalTexture");
	m_HoverTexture = s.ReadString("hoverTexture");
}

