/*==============================================================================

    タイトル画面[title.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/25
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/UIObjects/button.h"
#include "title.h"
#include "game.h"
#include "player.h"
#include "playerStateTitle.h"
#include "enemy.h"
#include "enemyStateTitle.h"

void Title::Init()
{
    Scene::Init();

    //リリース
    //Manager::GetScene()->DataLoad("save/title.bin");
    //開発中
    Manager::GetScene()->DataLoad("save/title.json");

    auto enemies = Manager::GetScene()->GetGameObjects<Enemy>();

    for (auto enemy : enemies)
    {
        enemy->ChangeState(new EnemyStateTitle());
    }

    auto* player = Manager::GetScene()->GetGameObject<Player>();
    player->ChangeState(new PlayerStateTitle);

    auto* camera = Manager::GetScene()->GetGameObject<Camera>();
    camera->SetTarget(nullptr);
    camera->ChangeMode(Camera::CameraMode::Fixed_Mode);

    Manager::GetScene()->GetGameObject<StartButton>()->OnClickEvent = [this]() {StartEvent(); };
}

void Title::Update()
{
    Scene::Update();
    
    float deltaTime = 1.0f / 60.0f;
    
    auto* startButton = Manager::GetScene()->GetGameObject<StartButton>();
    
    if (!startButton->IsHover() && !Manager::GetCreaterMode())
    {
        Vector3 buttonPos = startButton->GetPosition();
    
        buttonPos.y += std::sin(m_Time * m_Speed) * m_Amplitude;
    
        m_Time += deltaTime;
    
        startButton->SetPosition(buttonPos);
    }

    if (m_Change)
    {
        if (Manager::GetScene()->GetGameObject<ScenePolygon2D>()->IsFadeOutComplete())
        {
            Manager::SetScene<Game>();
        }
    }
}

void Title::StartEvent()
{
    Manager::GetScene()->GetGameObject<ScenePolygon2D>()->StartFadeOut();  
    m_Change = true;
}
