/*==============================================================================

    ƒŠƒUƒ‹ƒg‰æ–Ê[result.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/06/25
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/UIObjects/button.h"
#include "result.h"
#include "title.h"
#include "player.h"
#include "playerStateResult.h"
#include "enemy.h"
#include "enemyStateResult.h"

void Result::Init()
{
    Scene::Init();

    //ƒŠƒŠ[ƒX
    //Manager::GetScene()->DataLoad("save/result.bin");
    //ŠJ”­’†
    Manager::GetScene()->DataLoad("save/result.json");

    auto enemies = Manager::GetScene()->GetGameObjects<Enemy>();
    for (auto enemy : enemies)
    {
        enemy->ChangeState(new EnemyStateResult());
    }

    auto* player = Manager::GetScene()->GetGameObject<Player>();
    player->ChangeState(new PlayerStateResult());

    auto* camera = Manager::GetScene()->GetGameObject<Camera>();
    camera->SetTarget(nullptr);
    camera->ChangeMode(Camera::CameraMode::Fixed_Mode);

    Manager::GetScene()->GetGameObject<EndButton>()->OnClickEvent = [this]() {EndEvent(); };
}

void Result::Update()
{
    Scene::Update();

    float deltaTime = 1.0f / 60.0f;

    auto* endButton = Manager::GetScene()->GetGameObject<EndButton>();

    if (!endButton->IsHover() && !Manager::GetCreaterMode())
    {
        Vector3 buttonPos = endButton->GetPosition();

        buttonPos.y += std::sin(m_Time * m_Speed) * m_Amplitude;

        m_Time += deltaTime;

        endButton->SetPosition(buttonPos);
    }


    if (m_Change)
    {
        if (Manager::GetScene()->GetGameObject<ScenePolygon2D>()->IsFadeOutComplete())
        {
            Manager::SetScene<Title>();
        }
    }
}

void Result::EndEvent()
{
    Manager::GetScene()->GetGameObject<ScenePolygon2D>()->StartFadeOut();
    m_Change = true;
}