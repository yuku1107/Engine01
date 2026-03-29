/*==============================================================================

    UI描画[ui.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2026/02/13
==============================================================================*/
#include "main.h"
#include "Engine/Core/scene.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Rendering/sprite2d.h"
#include "Engine/Components/shader.h"
#include "Engine/Objects/UIObjects/ui.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "ThirdParty/ImGui/imgui.h"

void UI::Init()
{
    //デフォルト
    if (m_Scale.length() == 0.0f)
    {
        m_Scale = Vector3(200.0f, 100.0f, 1.0f);
    }

    //テクスチャ読込
    if (m_TextureName.empty())
    {
        m_TextureName = GetDefaultTexture();
    }

    m_Texture = Texture::Get(m_TextureName);

    m_ShaderName = GetShaderName();
    AddComponent<Shader>()->Set(this);

    m_Tag = "UI";
}

void UI::Init(float x, float y, float Width, float Height, const std::string& name)
{
    m_Position = Vector3(x, y, 0.0f);
    m_Scale = Vector3(Width, Height, 1.0f);

    //テクスチャ読込
    m_TextureName = name;
    m_Texture = Texture::Get(m_TextureName);

    m_ShaderName = GetShaderName();
    AddComponent<Shader>()->Set(this);

    m_Tag = "UI";

    m_DrawEnable = true;
}

void UI::Init(float x, float y, float Width, float Height, const std::string& name, const char* FileName)
{

    m_Position = Vector3(x, y, 0.0f);
    m_Scale = Vector3(Width, Height, 1.0f);

    //テクスチャ読込
    m_TextureName = name;
    m_Texture = Texture::Load(m_TextureName, FileName);

    m_ShaderName = GetShaderName();
    AddComponent<Shader>()->Set(this);

    m_Tag = "UI";
}

void UI::UninitSelf()
{

}

void UI::UpdateSelf()
{

}

void UI::DrawSelf()
{
    Sprite2D::Draw(m_Position, m_Scale, m_Rotation, m_Texture, m_MaterialColor, m_Frame, m_SplitX, m_SplitY);
}

void UI::DrawInspector()
{
    // コンボボックス開始
    if (ImGui::BeginCombo("Texture", m_TextureName.c_str()))
    {
        for (auto& kv : Texture::GetAll())
        {
            const std::string& textureName = kv.first;
            bool is_selected = (m_TextureName == textureName);

            if (ImGui::Selectable(textureName.c_str(), is_selected))
            {
                // 選択されたらポリゴンに適用
                SetTextureName(textureName);
            }

            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
}

void UI::SetTextureName(const std::string& name)
{
    if (m_TextureName == name)
        return;

    m_TextureName = name;

    if (!m_TextureName.empty())
    {
        if (auto srv = Texture::Get(m_TextureName))
            m_Texture = srv;
    }
}

bool UI::GetCollision(Vector3 pos)
{
    if (pos.x > m_Position.x && pos.x < (m_Position.x + m_Scale.x))
    {
        if (pos.y > m_Position.y && pos.y < (m_Position.y + m_Scale.y))
        {
            return true;
        }
    }

    return false;
}

void SpinnerUI::UpdateSelf()
{
    m_Rotation.x += 0.1f;
}

void ClearUI::UpdateSelf()
{

    if (Manager::GetCreaterMode() && Manager::GetUIMode())
    {
        SetDrawEnable(true);
    }
    else
    {
        auto* camera = Manager::GetScene()->GetGameObject<Camera>();

        if (camera)
        {
            if (camera->GetMode() == Camera::CameraMode::Clear_Mode)
            {
                m_DrawEnable = true;
            }
            else
            {
                m_DrawEnable = false;
            }
        }
    }
}

void DigitUI::SetDigit(int digit)
{
    digit = Clamp(digit, 0, 9);
    SetFrame(digit);
}

void MouseUI::UpdateSelf()
{
    m_Scale = Vector3(50.0f, 50.0f, 1.0f);

    m_Position = Mouse_GetPosition();

    auto* camera = Manager::GetScene()->GetGameObject<Camera>();

    if (!camera)
    {
        SetTextureName("cursor");
        m_Position.x -= m_Scale.x * 0.5f;
        m_Position.y -= m_Scale.y * 0.5f;
        return;
    }

    switch (camera->GetMode())
    {
    case Camera::Fixed_Mode:
    case Camera::Clear_Mode:
        SetTextureName("cursor");
        m_Position.x -= m_Scale.x * 0.5f;
        m_Position.y -= m_Scale.y * 0.5f;
        if (camera->IsBlending())
        {
            m_DrawEnable = false;
        }
        else
        {
            m_DrawEnable = true;
        }
        break;
    case Camera::ThirdPerson_Shooting:
        SetTextureName("reticle");
        m_Position.x = SCREEN_WIDTH * 0.5f - m_Scale.x * 0.5f;
        m_Position.y = SCREEN_HEIGHT * 0.5f - m_Scale.y * 0.5f;
        m_DrawEnable = true;
        break;
    default:
        m_DrawEnable = false;
        break;
    }
}
