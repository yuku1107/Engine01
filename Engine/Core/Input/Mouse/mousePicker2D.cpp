/*==============================================================================

    [mousePicker2D.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2026/02/27
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Core/scene.h"
#include "Engine/Core/Input/Mouse/mousePicker2D.h"
#include "Editor/ImGui/imGuiEngine.h"
#include "Engine/Objects/SceneObjects/coordinate2D.h"

void MousePicker2D::Update()
{
    if (!Manager::GetUIMode())
        return;

    if (Manager::GetGui()->GetHoveredWindowName() != "Scene")
        return;

    m_Coordinate = Manager::GetScene()->GetGameObject<Coordinate2D>();
    if (!m_Coordinate)
        return;

    Vector3 mousePos(
        Manager::GetGui()->GetLocalMousePos().x,
        Manager::GetGui()->GetLocalMousePos().y,
        0.0f
    );

    // クリック開始
    if (Mouse_IsTrigger(MouseButton::Left))
    {
        //まず軸ヒット判定
        if (m_Coordinate->GetCollision(mousePos))
        {
            m_IsDragging = true;
            m_Coordinate->StartAxisDrag(*m_Coordinate->GetNum(),mousePos);
        }
        else
        {
            //UIオブジェクト選択
            GameObject* selected = nullptr;

            auto& objects = Manager::GetScene()->GetGameObjects(1); // UIレイヤ

            for (auto obj : objects)
            {
                if (obj->GetTag() == "UI")
                {
                    UI* ui = dynamic_cast<UI*>(obj);
                    if (ui && ui->GetCollision(mousePos))
                    {
                        selected = obj;
                        break;
                    }
                }
                else if(obj->GetTag() == "BUTTON")
                {
                    UI* ui = dynamic_cast<UI*>(obj);
                    if (ui && ui->GetCollision(mousePos))
                    {
                        selected = obj;
                        break;
                    }
                }

            }

            m_Coordinate->SetTarget(selected);
        }
    }

    // ドラッグ中
    if (m_IsDragging)
    {
        m_Coordinate->ApplyAxisMovement(mousePos);
    }

    // 離したら終了
    if (Mouse_IsReleased(MouseButton::Left))
    {
        m_IsDragging = false;
        m_Coordinate->EndAxisDrag();
    }
}