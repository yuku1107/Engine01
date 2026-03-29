/*==============================================================================

    カメラ[camera.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/Input/deviceInput.h"
#include "Engine/Core/Input/inputMapper.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/shape.h"
#include "Engine/Collision/collision.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Serialization/serializer.h"
#include "Engine/Math/vector3.h"
#include "Editor/ImGui/imGuiEngine.h"
#include "Game/Input/gameInput.h"


void Camera::Init()
{
    m_Tag = "CAMERA";
}

void Camera::UninitSelf()
{

}

void Camera::UpdateSelf()
{
    if (Manager::GetCreaterMode())
    {
        ChangeMode(CameraMode::Creater_Mode);
    }
    else if (!m_Target)
    {
        ChangeMode(CameraMode::Fixed_Mode);
    }
    else
    {
        if (m_Mode != CameraMode::Clear_Mode)
        {
            if (InputMapper::GetButtonPressed(InputAction::Aim))
            {
                ChangeMode(CameraMode::ThirdPerson_Shooting);
            }
            else
            {
                ChangeMode(CameraMode::ThirdPerson_Free);
            }
        }
    }

    //モードが変わったら補間開始
    if (m_NextMode != m_Mode && !m_IsBlending)
    {
        m_IsBlending = true;
        m_BlendT = 0.0f;
        m_BlendStartPos = m_Position;
        m_BlendStartTarget = m_TargetPosition;

        Vector3 newPos, newTarget;
        //新モード側の理想位置を一時計算して保存
        if (m_Target)
        {
            GetModeCameraPos(m_NextMode, m_Target, newPos, newTarget);
        }
        else
        {
            newPos = m_Position;
			newTarget = m_TargetPosition;
        }

        m_BlendEndPos = newPos;
        m_BlendEndTarget = newTarget;
    }

    Vector3 targetPos;
    Vector3 targetForward;
    Vector3 targetRight;
    Vector3 targetUp;

    if (m_Target)
    {
        targetPos = m_Target->GetPosition() + Vector3(0.0f, 1.5f, 0.0f);
        targetForward = m_Target->GetForward().normalized();
        targetRight = m_Target->GetRight().normalized();
        targetUp = m_Target->GetUp().normalized();
    }

    switch (m_Mode)
    {
        // 通常モード（常に回転・カーソル固定）
        case CameraMode::ThirdPerson_Free:
        {
            //マウスホイールで距離調整
            UpdateDistanceByScroll();
      
            // R2 / L2 でも距離調整
            {
                const float zoomSpeed = 0.1f;

                if (DeviceInput::GetPadPress(SDL_GAMEPAD_BUTTON_DPAD_UP))
                {
                    m_Distance -= zoomSpeed;
                }

                if (DeviceInput::GetPadPress(SDL_GAMEPAD_BUTTON_DPAD_DOWN))
                {
                    m_Distance += zoomSpeed;
                }

                m_Distance = Clamp(m_Distance, 2.0f, 20.0f);
            }

            // マウス + 右スティックで回転
            float dx = GameInput::GetLookX();
            float dy = GameInput::GetLookY();

            m_Rotation.y += dx;
            m_Rotation.x += dy;
            m_Rotation.x = Clamp(m_Rotation.x, -1.5f, 1.5f);
        
            m_Velocity = (targetPos - m_TargetPosition) * 0.1f;
            m_TargetPosition += m_Velocity;
            m_Position = m_TargetPosition + Vector3(-sinf(m_Rotation.y) * cosf(m_Rotation.x), sinf(m_Rotation.x), -cosf(m_Rotation.y) * cosf(m_Rotation.x)) * m_Distance;

            m_TargetPosition = targetPos;
        
            m_Direction = (m_TargetPosition - m_Position).normalized();
            break;
        }
        
        // シューティングモード（肩越し・プレイヤーと同期）
        case CameraMode::ThirdPerson_Shooting:
        {

            // マウス + 右スティックで回転
            float dx = GameInput::GetLookX();
            float dy = GameInput::GetLookY();

            m_Rotation.y += dx;
            m_Rotation.x += dy;
            m_Rotation.x = Clamp(m_Rotation.x, -1.2f, 1.2f);

            //プレイヤーの向き（Y軸のみ反映）
            m_Target->SetRotation(Vector3(0.0f, m_Rotation.y, 0.0f));

            //向きベクトル計算
            Vector3 forward = targetForward;
            Vector3 right = targetRight;
            Vector3 up = targetUp;

            //肩位置（中心点）
            Vector3 shoulderPos = (targetPos - Vector3(0.0f, 0.5f, 0.0f)) + up * 0.5f + right * 0.5f;

            //ピッチ反映した視線方向
            XMVECTOR vForward = XMLoadFloat3((XMFLOAT3*)&forward);
            XMVECTOR vRight = XMLoadFloat3((XMFLOAT3*)&right);
            XMMATRIX rotPitch = XMMatrixRotationAxis(vRight, m_Rotation.x);
            XMVECTOR vLook = XMVector3TransformNormal(vForward, rotPitch);

            XMFLOAT3 tmp;
            XMStoreFloat3(&tmp, vLook);
            Vector3 lookDir = Vector3(tmp.x, tmp.y, tmp.z).normalized();

            //カメラ位置（肩を中心に回転）
            float camDistance = 2.0f;
            Vector3 offset = -lookDir * camDistance;
            m_Position = shoulderPos + offset;

            //カメラの注視点（視線の先）
            m_TargetPosition = shoulderPos + lookDir * 100.0f;

            //最終更新
            m_Direction = lookDir;
            break;
        }
    
        //固定モード用
        case CameraMode::Fixed_Mode:
        {
            m_TargetPosition += m_Velocity;
            m_Position = m_TargetPosition + Vector3(-sinf(m_Rotation.y) * cosf(m_Rotation.x), sinf(m_Rotation.x), -cosf(m_Rotation.y) * cosf(m_Rotation.x)) * m_Distance;
            m_Velocity *= 0.8f;

            m_Direction = (m_TargetPosition - m_Position).normalized();
            break;
        }

        //クリア用
        case CameraMode::Clear_Mode:
        {
            m_Distance = 5.0f;

            m_TargetPosition = targetPos - targetRight * 2.0f;

            m_Position = m_TargetPosition + targetForward * m_Distance;

            m_Direction = (m_TargetPosition - m_Position).normalized();
            break;
        }

        //クリエーターモード
        case CameraMode::Creater_Mode:
        {
            if (!Manager::GetGui()->IsSceneFocused()) break;

            //マウスホイールで距離調整
            UpdateDistanceByScroll();

            //マウス回転（右クリック中のみ）
            if (Mouse_IsPressed(MouseButton::Right))
            {
                float dx = Mouse_GetDelta().x;
                float dy = Mouse_GetDelta().y;
                const float sensitivity = 0.002f;

                m_Rotation.y += dx * sensitivity;
                m_Rotation.x += dy * sensitivity;
                m_Rotation.x = Clamp(m_Rotation.x, -1.5f, 1.5f);
            }
            
            float lx = InputMapper::GetAxis(InputAction::MoveX);
            float ly = InputMapper::GetAxis(InputAction::MoveY);

            // カメラ基準方向
            Vector3 forward = GetForward();
            if (forward.lengthSq() > 0.0f)
            {
                forward.normalize();
            }

            Vector3 right = GetRight();
            if (right.lengthSq() > 0.0f)
            {
                right.normalize();
            }

            // 加速度として加算（今の挙動を維持）
            m_Velocity += forward * ly * 0.05f;
            m_Velocity += right * lx * 0.05f;

            m_TargetPosition += m_Velocity;
            m_Position = m_TargetPosition + Vector3(-sinf(m_Rotation.y) * cosf(m_Rotation.x), sinf(m_Rotation.x), -cosf(m_Rotation.y) * cosf(m_Rotation.x)) * m_Distance;
            m_Velocity *= 0.8f;

            m_Direction = (m_TargetPosition - m_Position).normalized();
            break;
        }

    }

    if (m_IsBlending)
    {
        m_BlendT += 0.05f;
        if (m_BlendT >= 1.0f)
        {
            m_BlendT = 1.0f;
            m_IsBlending = false;
            m_Mode = m_NextMode;
        }

        float t = m_BlendT;
        m_Position = Lerp(m_BlendStartPos, m_BlendEndPos, t);
        m_TargetPosition = Lerp(m_BlendStartTarget, m_BlendEndTarget, t);
        m_Direction = (m_TargetPosition - m_Position).normalized();
        return;
    }

    if (!Manager::GetCreaterMode())
    {
        IdealPos(m_Mode, m_Target);
    }
}

void Camera::DrawSelf()
{
    //プロジェクションマトリクス(見方)
    m_Projection = XMMatrixPerspectiveFovLH(m_Fov, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_Nearclip, m_Farclip);

    Renderer::SetProjectionMatrix(m_Projection);

    //ビューマトリクス(カメラ)
    m_View = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position), XMLoadFloat3((XMFLOAT3*)&m_TargetPosition), XMLoadFloat3((XMFLOAT3*)&m_Up));

    Renderer::SetViewMatrix(m_View);    

    Renderer::SetCameraPosition(XMFLOAT3(m_Position.x, m_Position.y, m_Position.z));
}

//視錐台カリング
bool Camera::CheckView(Vector3 pos, Vector3 size, float dis)
{
    XMMATRIX vp;
    vp = m_View * m_Projection;

    XMMATRIX invVp;
    invVp = XMMatrixInverse(nullptr, vp);

    XMFLOAT3 vpos[4];
    vpos[0] = XMFLOAT3(-1.0f, 1.0f, 1.0f);
    vpos[1] = XMFLOAT3(1.0f, 1.0f, 1.0f);
    vpos[2] = XMFLOAT3(-1.0f, -1.0f, 1.0f);
    vpos[3] = XMFLOAT3(1.0f, -1.0f, 1.0f);

    XMVECTOR vposv[4];
    vposv[0] = XMLoadFloat3(&vpos[0]);
    vposv[1] = XMLoadFloat3(&vpos[1]);
    vposv[2] = XMLoadFloat3(&vpos[2]);
    vposv[3] = XMLoadFloat3(&vpos[3]);

    XMVECTOR wposv[4];
    wposv[0] = XMVector3TransformCoord(vposv[0], invVp);
    wposv[1] = XMVector3TransformCoord(vposv[1], invVp);
    wposv[2] = XMVector3TransformCoord(vposv[2], invVp);
    wposv[3] = XMVector3TransformCoord(vposv[3], invVp);

    XMFLOAT3 wpos[4];
    XMStoreFloat3(&wpos[0], wposv[0]);
    XMStoreFloat3(&wpos[1], wposv[1]);
    XMStoreFloat3(&wpos[2], wposv[2]);
    XMStoreFloat3(&wpos[3], wposv[3]);

    Vector3 v;
    v = pos - m_Position;

    Vector3 wp[4];
    wp[0] = XMFLOAT3ToVector3(wpos[0]);
    wp[1] = XMFLOAT3ToVector3(wpos[1]);
    wp[2] = XMFLOAT3ToVector3(wpos[2]);
    wp[3] = XMFLOAT3ToVector3(wpos[3]);

    //左面
    {
        Vector3 v1, v2;
        v1 = wp[0] - m_Position;
        v2 = wp[2] - m_Position;

        Vector3 n;
        n = crossProduct(v1, v2);
        n.normalize(); //正規化

        float d;
        d = n.dot(v); //面からの距離

        if (d < -size.x)
        {
            return false;
        }
    }

    //右面
    {
        Vector3 v1, v2;
        v1 = wp[1] - m_Position;
        v2 = wp[3] - m_Position;

        Vector3 n;
        n = crossProduct(v1, v2);
        n.normalize(); //正規化

        float d;
        d = n.dot(v);

        if (d > size.x)
        {
            return false;
        }
    }

    //上面
    {
        Vector3 v1, v2;
        v1 = wp[0] - m_Position;
        v2 = wp[1] - m_Position;

        Vector3 n;
        n = crossProduct(v1, v2);
        n.normalize(); //正規化

        float d;
        d = n.dot(v);

        if (d > size.y)
        {
            return false;
        }
    }

    //下面
    {
        Vector3 v1, v2;
        v1 = wp[2] - m_Position;
        v2 = wp[3] - m_Position;

        Vector3 n;
        n = crossProduct(v1, v2);
        n.normalize(); //正規化

        float d;
        d = n.dot(v);

        if (d < -size.y)
        {
            return false;
        }
    }

    if (dis != 0.0f)
    {
        float length = (pos - m_Position).length();
        if (length > dis)
        {
            return false;
        }
    }

    return true;
}

//モード変更による位置変更
void Camera::GetModeCameraPos(CameraMode mode, GameObject* obj, Vector3& outPos, Vector3& outTarget)
{
    Vector3 targetPos = obj->GetPosition() + Vector3(0.0f, 1.0f, 0.0f);
    Vector3 targetForward = obj->GetForward().normalized();
    Vector3 targetRight = obj->GetRight().normalized();
    Vector3 targetUp = obj->GetUp().normalized();

    switch (mode)
    {
        case CameraMode::ThirdPerson_Free:
        case CameraMode::Clear_Mode:
        {
            outTarget = targetPos;
            outPos = outTarget + Vector3(-sinf(m_Rotation.y) * cosf(m_Rotation.x), sinf(m_Rotation.x), -cosf(m_Rotation.y) * cosf(m_Rotation.x)) * m_Distance;
            break;
        }
        case CameraMode::ThirdPerson_Shooting:
        {
            Vector3 shoulderPos = targetPos + targetUp * 0.5f + targetRight * 0.5f;
            XMVECTOR vForward = XMLoadFloat3((XMFLOAT3*)&targetForward);
            XMVECTOR vRight = XMLoadFloat3((XMFLOAT3*)&targetRight);
            XMMATRIX rotPitch = XMMatrixRotationAxis(vRight, m_Rotation.x);
            XMVECTOR vLook = XMVector3TransformNormal(vForward, rotPitch);
            XMFLOAT3 tmp;
            XMStoreFloat3(&tmp, vLook);
            Vector3 lookDir = Vector3(tmp.x, tmp.y, tmp.z).normalized();
            outPos = shoulderPos - lookDir * 2.0f;
            outTarget = shoulderPos + lookDir * 100.0f;
            break;
        }
        case CameraMode::Fixed_Mode:
        case CameraMode::Creater_Mode:
            outPos = m_Position;
            outTarget = m_TargetPosition;
            break;
    }
}

void Camera::UpdateDistanceByScroll()
{
    static int prevScroll = Mouse_GetScrollWheelValue();
    int scrollDelta = Mouse_GetScrollWheelValue() - prevScroll;
    prevScroll = Mouse_GetScrollWheelValue();

    if (scrollDelta != 0)
    {
        m_Distance -= scrollDelta * 0.01f;
        m_Distance = Clamp(m_Distance, 2.0f, 20.0f);
    }
}

void Camera::ApplyMouseMode(CameraMode mode)
{
    switch (mode)
    {
    case CameraMode::ThirdPerson_Free:
    case CameraMode::ThirdPerson_Shooting:
        Mouse_SetCursorMode(MouseCursorMode::LockedCenter);
        break;

    case CameraMode::Fixed_Mode:
    case CameraMode::Clear_Mode:
        Mouse_SetCursorMode(MouseCursorMode::VirtualCursor);
        break;

    case CameraMode::Creater_Mode:
        Mouse_SetCursorMode(MouseCursorMode::FreeCursor);
        break;
    }
}

void Camera::ReapplyMouseMode()
{
    ApplyMouseMode(m_Mode);
}

//理想位置調整
void Camera::IdealPos(CameraMode mode, GameObject* target)
{
    switch (mode)
    {
        case CameraMode::ThirdPerson_Free:
        {
            Vector3 targetPos = target->GetPosition() + Vector3(0, 1.5f, 0);
        
            // カメラの理想位置計算
            Vector3 idealPos = targetPos + Vector3(-sinf(m_Rotation.y) * cosf(m_Rotation.x), sinf(m_Rotation.x), -cosf(m_Rotation.y) * cosf(m_Rotation.x)) * m_Distance;
        
            // レイキャストによる衝突補正
            m_Position = AdjustByCollision(targetPos, idealPos, target);
        
            // 注視点
            m_TargetPosition = targetPos;
        
            m_Direction = (m_TargetPosition - m_Position).normalized();
            break;
        }
    }
}

Vector3 Camera::AdjustByCollision(const Vector3& start, const Vector3& ideal, GameObject* target)
{
    Vector3 dir = ideal - start;
    float maxDist = dir.length();
    if (maxDist < 0.001f)
    {
        return ideal;
    }
    dir /= maxDist;

    std::vector<Shape*> hits;
    Vector3 hitPos;
    float   hitDist = 0.0f;

    Shape* ignore = target->GetComponent<Capsule>();

    bool isHit = Manager::GetCollision()->Raycast(start, dir, hits, maxDist, &hitPos, &hitDist, ignore);

    for (auto hit : hits)
    {
        if (hit->GetpOwner()->GetTag() == "ENEMY")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "BULLET")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "LIGHT")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "COORDINATE")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "ANCHOR")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "TRANSFERDEVICE")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "GOAL")
        {
            return ideal;
        }
        else if (hit->GetpOwner()->GetTag() == "DISCOVERYAREA")
        {
            return ideal;
        }

    }

    if (isHit)
    {
        // オブジェクトと少し間を空ける
        float safeDist = std::max(hitDist - 0.2f, 0.5f);
        return start + dir * safeDist;
    }

    return ideal;
}

void Camera::ChangeMode(CameraMode newMode)
{
    if (m_Mode != newMode)
    {
        m_NextMode = newMode;
        ApplyMouseMode(m_NextMode);
    }
}

void Camera::Serialize(Serializer& s)
{
    GameObject::Serialize(s);

    s.WriteVector3("targetPosition", m_TargetPosition);
    s.WriteFloat("distance", m_Distance);
}

void Camera::Deserialize(Serializer& s)
{
    GameObject::Deserialize(s);

    m_TargetPosition = s.ReadVector3("targetPosition");
    m_Distance = s.ReadFloat("distance");
}
