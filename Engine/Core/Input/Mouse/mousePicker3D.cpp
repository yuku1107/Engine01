/*==============================================================================

    [mousePicker3D]
                                                             Author:Šs@“N‰F
                                                             Date  :2026/02/27
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/coordinate.h"
#include "Engine/Core/scene.h"
#include "Engine/Collision/collision.h"
#include "Engine/Core/Input/Mouse/mousePicker3D.h"
#include "Editor/ImGui/imGuiEngine.h"

void MousePicker3D::Update()
{
    if (!Manager::GetCreaterMode()) return;
    if (Manager::GetUIMode()) return;
    if (Manager::GetGui()->GetHoveredWindowName() != "Scene") return;

    Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
    if (!camera) return;

    m_Coordinate = Manager::GetScene()->GetGameObject<Coordinate>();
    if (!m_Coordinate) return;

    Line ray = CreateRay(camera);

    HandleAxisSelection(ray);
    HandleObjectSelection(ray);

    if (Mouse_IsReleased(MouseButton::Left))
    {
        m_IsAxisDragging = false;
        m_Coordinate->EndAxisDrag();
    }
}

Line MousePicker3D::CreateRay(Camera* camera)
{
    Vector3 mousePos = Vector3(Manager::GetGui()->GetLocalMousePos().x, Manager::GetGui()->GetLocalMousePos().y, 0.0f);

    XMFLOAT3 worldPos =Renderer::ScreenToWorld((int)mousePos.x,(int)mousePos.y,camera->GetViewMatrix(),camera->GetProjectionMatrix());

    Vector3 start = camera->GetPosition();
    Vector3 dir = XMFLOAT3ToVector3(worldPos) - start;

    return Line(start, dir, 1000.0f);
}

void MousePicker3D::HandleAxisSelection(const Line& ray)
{
    if (!Mouse_IsTrigger(MouseButton::Left)) return;
    if (!m_SelectedObject) return;

    int axisIndex = -1;

    Box* hitBox = m_Coordinate->IsHit(ray, &axisIndex);

    if (hitBox && axisIndex != -1)
    {
        m_IsAxisDragging = true;
        m_Coordinate->StartAxisDrag(axisIndex);
    }
}

void MousePicker3D::HandleObjectSelection(const Line& ray)
{
    if (!Mouse_IsTrigger(MouseButton::Left)) return;
    if (m_IsAxisDragging) return;

    auto shapes = Manager::GetCollision()->ShapeCollision(ray);

    Shape* closestShape = nullptr;
    float closestDist = FLT_MAX;

    for (auto shape : shapes)
    {
        if (!shape || !shape->GetpOwner())
            continue;

        if (shape->GetpOwner()->GetTag() == "COORDINATE")
            continue;

        if (shape->GetpOwner()->GetTag() == "LIGHT")
            continue;

        if (shape->GetpOwner()->GetTag() == "CAMERA")
            continue;

        Vector3 hitPoint;
        float hitDist;

        if (!shape->Intersect(ray, 0, &hitPoint, &hitDist))
            continue;

        if (hitDist < closestDist)
        {
            closestDist = hitDist;
            closestShape = shape;
        }
    }

    if (closestShape)
    {
        GameObject* obj = closestShape->GetpOwner();

        if (m_SelectedObject)
            m_SelectedObject->SetMouseSelection(false);

        m_SelectedObject = obj;
        m_SelectedObject->SetMouseSelection(true);

        m_Coordinate->SetTarget(obj);
    }
    else
    {
        ClearSelection();
    }
}

void MousePicker3D::ClearSelection()
{
    if (m_SelectedObject)
    {
        m_SelectedObject->SetMouseSelection(false);
        m_SelectedObject = nullptr;
    }

    if (m_Coordinate)
        m_Coordinate->SetTarget(nullptr);
}