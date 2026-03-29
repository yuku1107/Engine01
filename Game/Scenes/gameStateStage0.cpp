/*==============================================================================

    game_ステージ0[gameStateStage0.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/12
==============================================================================*/
#include "gameStateStage0.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/audio.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "result.h"
#include "gameStateTunnel.h"
#include "player.h"
#include "targetMark.h"
#include "discoveryWindow.h"

void GameStateStage0::Enter(Game* game)
{
    //リリース
    //Manager::GetScene()->DataLoad("save/stage0.bin");
    //開発中
    Manager::GetScene()->DataLoad("save/stage0.json");

    auto* player = Manager::GetScene()->GetGameObject<Player>();

    auto* camera = Manager::GetScene()->GetGameObject<Camera>();
    camera->SetTarget(player);
    camera->ChangeMode(Camera::CameraMode::ThirdPerson_Free);

    m_ShouldChangeStage = false;

    m_BGM = new Audio();
    m_BGM->Load("Assets\\audio\\stage0.wav");

    m_BGM->Play(true);
}

void GameStateStage0::Update(Game* game, float deltatime)
{
    auto player = Manager::GetScene()->GetGameObject<Player>();
    auto transferDevices = Manager::GetScene()->GetGameObjects<TransferDevice>();
    auto scenePolygon = Manager::GetScene()->GetGameObject<ScenePolygon2D>();

    if (m_ShouldChangeStage)
    {
        if (scenePolygon->IsFadeOutComplete())
        {
            switch (m_NextStageType)
            {
            case TransferDevice::Type_ToStage1:
                game->SetToStage(1);
                break;
            case TransferDevice::Type_ToStage2:
                game->SetToStage(2);
                break;
            case TransferDevice::Type_ToStage3:
                game->SetToStage(3);
                break;
            case TransferDevice::Type_ToResult:
                Manager::SetScene<Result>();
                return;
            }

            game->ChangeState(new GameStateTunnel());
        }
    }
    else
    {
        if (player)
        {
            for (auto transferDevice : transferDevices)
            {
                if (!transferDevice->GetCanUse())
                {
                    switch (transferDevice->GetType())
                    {
                    case TransferDevice::Type_ToStage1:
                        transferDevice->SetCanUse(true);
                        break;
                    case TransferDevice::Type_ToStage2:
                        if (game->IsStage1Cleared())
                        {
                            transferDevice->SetCanUse(true);
                        }
                        break;
                    case TransferDevice::Type_ToStage3:
                        if (game->IsStage2Cleared())
                        {
                            transferDevice->SetCanUse(true);
                        }
                        break;
                    case TransferDevice::Type_ToResult:
                        transferDevice->SetCanUse(true);
                        break;
                    }
                }

                bool hit = BoxToCapsule(*transferDevice->GetComponent<Box>(),*player->GetComponent<Capsule>());

                if (hit && !scenePolygon->IsFading() && transferDevice->GetCanUse())
                {
                    // 次のステージを覚えてフェード開始
                    m_NextStageType = transferDevice->GetType();
                    Manager::GetScene()->GetGameObject<ScenePolygon2D>()->StartFadeOut();
                    m_ShouldChangeStage = true;
                    break;
                }
            }
        }

    }

}

void GameStateStage0::Exit(Game* game)
{
    m_ShouldChangeStage = false;

    m_BGM->Uninit();
    delete m_BGM;
}
