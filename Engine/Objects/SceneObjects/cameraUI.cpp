/*==============================================================================

    3DUI描画用カメラ[cameraUI.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "cameraUI.h"

void CameraUI::Init()
{
	m_Tag = "CAMERAUI";
}

void CameraUI::UninitSelf()
{

}

void CameraUI::UpdateSelf()
{
    Camera* mainCam = Manager::GetScene()->GetGameObject<Camera>();

    if (mainCam)
    {
        // メインカメラと同じ位置・向きをコピー
        m_Position = mainCam->GetPosition();
        m_Target = mainCam->GetTargetPosition();
        m_Up = mainCam->GetUp();
        m_Rotation = mainCam->GetRotation();
    }

    Vector3 dir = m_Target - m_Position;
    if (dir.lengthSq() < 1e-6f)
    {
        m_Target = m_Position + Vector3(0.0f, 0.0f, 1.0f); // デフォルト前方
    }
}

void CameraUI::DrawSelf()
{
    // 深度だけクリア
    Renderer::ClearDepth();

    // 通常通りビュープロジェクション行列を設定
    m_Projection = XMMatrixPerspectiveFovLH(m_Fov,(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_Nearclip, m_Farclip);
    Renderer::SetProjectionMatrix(m_Projection);

    m_View = XMMatrixLookAtLH(XMLoadFloat3((XMFLOAT3*)&m_Position),XMLoadFloat3((XMFLOAT3*)&m_Target),XMLoadFloat3((XMFLOAT3*)&m_Up));
    Renderer::SetViewMatrix(m_View);

    Renderer::SetCameraPosition(XMFLOAT3(m_Position.x, m_Position.y, m_Position.z));

    // UI専用レイヤーだけ描画
    Manager::GetScene()->DrawLayer(3);
}