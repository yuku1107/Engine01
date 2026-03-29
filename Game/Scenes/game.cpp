/*==============================================================================

    ƒQ[ƒ€‰æ–Ê[game.cpp]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/06/25
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "game.h"
#include "gameStateStage0.h"
#include "gameStateStage1.h"
#include "gameStateStage2.h"
#include "gameStateStage3.h"
#include "gameStateTunnel.h"

void Game::Init()
{
	Scene::Init();

	ChangeState(new GameStateStage0());
}

void Game::Update()
{
	Scene::Update();

	float deltaTime = 1.0f / 60.0f;

	if (m_CurrentState)
	{
		m_CurrentState->Update(this, deltaTime);
	}

}

void Game::ChangeState(GameState* newState)
{
	if (m_CurrentState)
	{
		m_CurrentState->Exit(this);
		delete m_CurrentState;
	}

	m_CurrentState = newState;

	if (m_CurrentState)
	{
		for (int layer = 0; layer < LAYER; layer++)
		{
			Manager::GetScene()->ClearGameObjects(layer);
		}

		Manager::GetCollision()->Clear();

		m_CurrentState->Enter(this);
	}
}
