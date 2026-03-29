/*==============================================================================

	プレイヤー_クリエーターモード[playerStateCreater.cpp]
															 Author:郭　哲宇
															 Date  :2025/02/02
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateCreater.h"
#include "playerStateAir.h"
#include "playerStateTunnel.h"
#include "playerStateTitle.h"
#include "playerStateResult.h"
#include "game.h"

void PlayerStateCreater::Enter(Player* player)
{
	player->SetPosition(player->GetStartPosition());
	
	auto* rb = player->GetComponent<Rigidbody>();
	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
	rb->SetAngularVelocity(Vector3(0.0f, 0.0f, 0.0f));
	rb->SetPositionNext(player->GetPosition());

	player->AnimationBlend("Idle");

}

void PlayerStateCreater::Update(Player* player, float deltatime)
{
	if (!Manager::GetCreaterMode())
	{
		if (Manager::IsGame())
		{
			player->ChangeState(new PlayerStateAir());
		}
		else if (Manager::IsTitle())
		{
			player->ChangeState(new PlayerStateTitle());
		}
		else if(Manager::IsResult())
		{
			player->ChangeState(new PlayerStateResult());
		}

		return;
	}

	player->SetStartPosition(player->GetPosition());
	player->SetStartRotation(player->GetRotation());
}

void PlayerStateCreater::Exit(Player* player)
{

}