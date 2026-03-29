/*==============================================================================

    ƒvƒŒƒCƒ„[_ƒNƒŠƒA[playerStateClear.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/09/15
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "playerStateClear.h"
#include "goal.h"

void PlayerStateClear::Enter(Player* player)
{
    player->AnimationBlend("Clear");
    player->SetIsAnimationLoop(true);
    if (player->GetIsJump())
    {
        player->SetIsJump(false);
    }

}

void PlayerStateClear::Update(Player* player, float deltatime)
{
    auto* rb = player->GetComponent<Rigidbody>();
    Vector3 golaPosition = Manager::GetScene()->GetGameObject<Goal>()->GetPosition();
    ScenePolygon2D* scenePolygon = Manager::GetScene()->GetGameObject<ScenePolygon2D>();
    
    if (!m_ClearReady)
    {
        if (scenePolygon->IsFadeOutComplete())
        {
            rb->SetPositionNext(golaPosition + Vector3(-3.0f, 0.0f, 0.0f));
            player->SetRotation(Vector3(0.0f, XM_PI, 0.0f));
            m_ClearReady = true;
        }
    }
    else
    {
        player->SetRotation(Vector3(0.0f, XM_PI, 0.0f));
    }
}

void PlayerStateClear::Exit(Player* player)
{
    
}
