/*==============================================================================

    ç¿ïWån[coordinate.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/05/28
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Objects/SceneObjects/coordinate.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "coordinate2D.h"

void Coordinate::Init()
{
	m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\Coordinate.fbx");

    for (int i = 0; i < 3; i++)
    {
        m_Box[i] = Box(m_Pos[i], m_Scl[i], Vector3(0.0f, 0.0f, 0.0f));
        m_Box[i].SetpOwner(this);
        Manager::GetCollision()->RegisterAuto(&m_Box[i]);
    }

    m_Scale = { 0.25f,0.25f,0.25f };
    m_SelectionNum = -1;
    m_SelectionSwitch = false;

    m_Tag = "COORDINATE";

    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);
}

void Coordinate::UninitSelf()
{
	m_Model = nullptr;
}

void Coordinate::UpdateSelf()
{
    if (!m_Target || !Manager::GetCreaterMode())
    {
        m_DrawEnable = false;
        m_SelectionSwitch = false;
        return;
    }

    m_DrawEnable = true;

    m_Position = m_Target->GetPosition();
    m_Rotation = m_Target->GetRotation();

    UpdateAxisBoxes();

    if (m_SelectionSwitch)
    {
        ApplyAxisMovement();
    }

    if (m_Target->GetDestroy())
        m_Target = nullptr;
}

void Coordinate::DrawSelf()
{
    if (m_Target)
    {
        //É}ÉgÉäÉNÉXê›íË
        XMMATRIX world, scale, rotation, trancelation;

        scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
        rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
        trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

        world = scale * rotation * trancelation;

        Renderer::SetWorldMatrix(world);

        m_Model->Draw();
    }

}

void Coordinate::StartAxisDrag(int axisIndex)
{
    m_SelectionNum = axisIndex;
    m_SelectionSwitch = true;
}

void Coordinate::EndAxisDrag()
{
    m_SelectionSwitch = false;
}

void Coordinate::UpdateAxisBoxes()
{
    Vector3 right = m_Target->GetRight();
    Vector3 up = m_Target->GetUp();
    Vector3 forward = m_Target->GetForward();

    m_Pos[0] = m_Target->GetPosition() + right;
    m_Scl[0] = { 1.5f,0.3f,0.3f };
    m_Box[0].Update(m_Pos[0], m_Scl[0], right, up, forward);

    m_Pos[1] = m_Target->GetPosition() + up;
    m_Scl[1] = { 1.5f,0.3f,0.3f };
    m_Box[1].Update(m_Pos[1], m_Scl[1], up, forward, right);

    m_Pos[2] = m_Target->GetPosition() + forward;
    m_Scl[2] = { 1.5f,0.3f,0.3f };
    m_Box[2].Update(m_Pos[2], m_Scl[2], forward, right, up);
}

void Coordinate::ApplyAxisMovement()
{
    Camera* camera = Manager::GetScene()->GetGameObject<Camera>();
    if (!camera) return;

    Vector3 axis;

    switch (m_SelectionNum)
    {
    case 0: axis = m_Target->GetRight(); break;
    case 1: axis = m_Target->GetUp(); break;
    case 2: axis = m_Target->GetForward(); break;
    default: return;
    }

    axis.normalize();

    Vector3 mouseDelta = Mouse_GetDelta();

    Vector3 camRight = camera->GetRight();
    Vector3 camUp = camera->GetUp();

    Vector3 mouseMoveWorld =
        camRight * mouseDelta.x +
        camUp * mouseDelta.y;

    float move = mouseMoveWorld.dot(axis);

    if (m_SelectionNum == 1)
    {
        move = -move;
    }

    Vector3 pos = m_Target->GetPosition();
    pos += axis * move * 0.05f;

    m_Target->SetPosition(pos);
}

void Coordinate::SetTarget(GameObject* obj)
{
    if (m_Target == obj) return;

    if (m_Target)
    {
        m_Target->SetMouseSelection(false);
    }

    m_Target = obj;

    if (m_Target)
    {
        m_Target->SetMouseSelection(true);
    }
}

Box* Coordinate::IsHit(const Line& line, int* out)
{
    Box* closestBox = nullptr;
    float closestDist = FLT_MAX;
    int closestIndex = -1;

    for (int i = 0; i < 3; i++)
    {
        Vector3 hitPoint;
        float hitDist = 0.0f;

        if (LineToBox(line, m_Box[i], 1, &hitPoint, &hitDist))
        {
            if (hitDist < closestDist)
            {
                closestDist = hitDist;
                closestBox = &m_Box[i];
                closestIndex = i;
            }
        }
    }

    if (out)
        *out = closestIndex;

    return closestBox;
}