/*==============================================================================

    game_トンネル[gameStateTunnel.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/18
==============================================================================*/
#include <thread>
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "Engine/Components/audio.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "tunnel.h"
#include "player.h"
#include "playerStateTunnel.h"
#include "game.h"
#include "gameStateTunnel.h"
#include "gameStateStage0.h"
#include "gameStateStage1.h"
#include "gameStateStage2.h"
#include "gameStateStage3.h"

void GameStateTunnel::Enter(Game* game)
{
	//リリース
    //Manager::GetScene()->DataLoad("save/gameTunnel.bin");
	//開発中
    Manager::GetScene()->DataLoad("save/gameTunnel.json");

    auto* player = Manager::GetScene()->GetGameObject<Player>();
    player->ChangeState(new PlayerStateTunnel);
    player->GetComponent<Rigidbody>()->SetUseGravity(false);

    auto* camera = Manager::GetScene()->GetGameObject<Camera>();
    camera->SetTarget(nullptr);
    camera->ChangeMode(Camera::CameraMode::Fixed_Mode);

    m_Time = 0.0f;
    m_LoadFinish = false;

    m_BGM = new Audio();
    m_BGM->Load("Assets\\audio\\tunnel.wav");

    m_BGM->Play(true);

    //スレッド
    std::thread th([=]
        {
            m_LoadFinish = true;
        });
    th.detach();

}

void GameStateTunnel::Update(Game* game, float deltatime)
{
    auto scenePolygon = Manager::GetScene()->GetGameObject<ScenePolygon2D>();

    if (m_ShouldChangeStage)
    {
        if (scenePolygon->IsFadeOutComplete())
        {
            switch (game->GetToStage())
            {
            case 0:
                game->ChangeState(new GameStateStage0());
                break;
            case 1:
                game->ChangeState(new GameStateStage1());
                break;
            case 2:
                game->ChangeState(new GameStateStage2());
                break;
            case 3:
                game->ChangeState(new GameStateStage3());
                break;
            }
        }
    }
    else
    {
        if (m_LoadFinish && m_Time >= 120.0f)
        {
            if (!scenePolygon->IsFading())
            {
                Manager::GetScene()->GetGameObject<ScenePolygon2D>()->StartFadeOut();
                m_ShouldChangeStage = true;
            }
        }
        else
        {
            m_Time++;
        }
    }
}

void GameStateTunnel::Exit(Game* game)
{
    m_ShouldChangeStage = false;

    m_BGM->Uninit();
    delete m_BGM;
}