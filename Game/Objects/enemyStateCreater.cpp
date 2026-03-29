/*==============================================================================

    “G_ƒNƒŠƒG[ƒ^[ƒ‚[ƒh[enemyStateCreater.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/02/02
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/rigidbody.h"
#include "enemyStateCreater.h"
#include "enemyStateSearch.h"
#include "enemyStateTitle.h"
#include "enemyStateResult.h"
#include "game.h"

void EnemyStateCreater::Enter(Enemy* enemy)
{
    m_StartPosition = enemy->GetStartPosition();
    m_StartRotation = enemy->GetStartRotation();
    enemy->SetPosition(m_StartPosition);
    enemy->SetRotation(m_StartRotation);
    enemy->SetMaterialColor({ 1.0f,1.0f,1.0f,1.0f });
    enemy->AnimationBlend("Idle");
    enemy->CanBeWire(false);
    auto* rb = enemy->GetComponent<Rigidbody>();
    rb->SetPositionNext(m_StartPosition);
    rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	enemy->SetDrawEnable(true);
}

void EnemyStateCreater::Update(Enemy* enemy, float deltatime)
{
	if (!Manager::GetCreaterMode())
	{
		if (Manager::IsGame())
		{
			enemy->ChangeState(new EnemyStateSearch);
		}
        else if (Manager::IsTitle())
        {
            enemy->ChangeState(new EnemyStateTitle());
        }
        else if (Manager::IsResult())
        {
            enemy->ChangeState(new EnemyStateResult());
		}

		return;
	}

    enemy->SetStartPosition(enemy->GetPosition());
    enemy->SetStartRotation(enemy->GetRotation());
}

void EnemyStateCreater::Exit(Enemy* enemy)
{

}