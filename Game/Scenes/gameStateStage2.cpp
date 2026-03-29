/*==============================================================================

    game_ステージ2[gameStateStage2.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/12
==============================================================================*/
#include "gameStateStage2.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/audio.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/UIObjects/button.h"
#include "player.h"
#include "playerStateClear.h"
#include "gameStateTunnel.h"

void GameStateStage2::Enter(Game* game)
{
    //リリース
    //Manager::GetScene()->DataLoad("save/stage2.bin");
    //開発中
    Manager::GetScene()->DataLoad("save/stage2.json");

    auto* player = Manager::GetScene()->GetGameObject<Player>();

    auto* camera = Manager::GetScene()->GetGameObject<Camera>();
    camera->SetTarget(player);
    camera->ChangeMode(Camera::CameraMode::ThirdPerson_Free);

    m_ShouldChangeStage = false;
    
    Manager::GetScene()->GetGameObject<ContinueButton>()->OnClickEvent = [this]() {ContinueEvent(); };
    Manager::GetScene()->GetGameObject<RetryButton>()->OnClickEvent = [this]() {RetryEvent(); };

    m_BGM = new Audio();
    m_BGM->Load("Assets\\audio\\stage2.wav");

    m_BGM->Play(true);
}

void GameStateStage2::Update(Game* game, float deltatime)
{
    auto* player = Manager::GetScene()->GetGameObject<Player>();
    auto* scenePolygon = Manager::GetScene()->GetGameObject<ScenePolygon2D>();
    auto* camera = Manager::GetScene()->GetGameObject<Camera>();

    if (player)
    {

        if (player->GetPosition().y <= -5.0f)
        {
            player->SetReturn();
        }

        if (m_ShouldChangeStage)
        {
            if (scenePolygon->IsFadeOutComplete())
            {
                if (m_Retry)
                {
                    game->SetStage2Cleared(true);
                    game->SetToStage(2);
                }
                else
                {
                    game->SetStage2Cleared(true);
                    game->SetToStage(0);
                }

                game->ChangeState(new GameStateTunnel());
                return;
            }

        }
        else
        {
            if (player->IsStateClear() && !m_ClearReady)
            {
                if (!scenePolygon->IsFading() && !scenePolygon->IsFadeOutComplete())
                {
                    scenePolygon->StartFadeOut();
                }

                if (scenePolygon->IsFadeOutComplete())
                {
                    camera->ChangeMode(Camera::CameraMode::Clear_Mode);
                    m_ClearReady = true;
                }
            }
            else if (m_ClearReady)
            {
                if (camera->GetMode() == Camera::CameraMode::Clear_Mode)
                {
                    if (!scenePolygon->IsFadeInComplete())
                    {
                        scenePolygon->StartFadeIn();
                    }
                }
                else if (!Manager::GetCreaterMode())
                {
                    camera->ChangeMode(Camera::CameraMode::Clear_Mode);
                    player->ChangeState(new PlayerStateClear());
                }
            }
        }
    }
}

void GameStateStage2::Exit(Game* game)
{
    m_ShouldChangeStage = false;

    auto* continueBtn = Manager::GetScene()->GetGameObject<ContinueButton>();
    auto* retryBtn = Manager::GetScene()->GetGameObject<RetryButton>();

    if (continueBtn) continueBtn->OnClickEvent = nullptr;
    if (retryBtn)    retryBtn->OnClickEvent = nullptr;

    m_BGM->Uninit();
    delete m_BGM;
}

void GameStateStage2::ContinueEvent()
{
    auto* scenePolygon = Manager::GetScene()->GetGameObject<ScenePolygon2D>();

    if (!scenePolygon->IsFading())
    {
        scenePolygon->StartFadeOut();
        m_Retry = false;
        m_ShouldChangeStage = true;
    }
}

void GameStateStage2::RetryEvent()
{
    auto* scenePolygon = Manager::GetScene()->GetGameObject<ScenePolygon2D>();

    if (!scenePolygon->IsFading())
    {
        scenePolygon->StartFadeOut();
        m_Retry = true;
        m_ShouldChangeStage = true;
    }
}