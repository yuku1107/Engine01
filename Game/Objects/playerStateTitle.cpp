/*==============================================================================

	プレイヤー_タイトル画面[playerStateTitle.cpp]
															 Author:郭　哲宇
															 Date  :2025/12/11
==============================================================================*/
#include "main.h"
#include "Engine/Components/rigidbody.h"
#include "playerStateTitle.h"

void PlayerStateTitle::Enter(Player* player)
{
	auto* rb = player->GetComponent<Rigidbody>();

	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));

	player->AnimationBlend("Sleep");
}

void PlayerStateTitle::Update(Player* player, float deltatime)
{
	auto* rb = player->GetComponent<Rigidbody>();
	rb->SetVelocity(Vector3(0.0f, 0.0f, 0.0f));
	player->SetPosition(player->GetStartPosition());
	rb->SetPositionNext(player->GetPosition());
}

void PlayerStateTitle::Exit(Player* player)
{
	
}