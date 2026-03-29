/*==============================================================================

    ローディング画面[loading.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/10
==============================================================================*/
#include "main.h"
#include <thread>
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Core/Input/input.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/UIObjects/ui.h"
#include "Engine/Objects/engineObjectRegister.h"
#include "gameObjectRegister.h"
#include "game.h"
#include "title.h"
#include "result.h"
#include "loading.h"

void Loading::Init()
{
    RegisterEngineObjects();
    RegisterGameObjects();

    AddGameObject<WhiteUI>(1)->Init(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, "white", "Assets\\texture\\white.png");
    AddGameObject<LoadingUI>(1)->Init(SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.35f, SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, "loading", "Assets\\texture\\loading.png");
    AddGameObject<SpinnerUI>(1)->Init(SCREEN_WIDTH / 1.5f, SCREEN_HEIGHT / 1.5f, SCREEN_WIDTH / 3, SCREEN_HEIGHT / 3, "spinner", "Assets\\texture\\spinner.png");
    AddGameObject<ScenePolygon2D>(0);

    m_LoadFinish = false;

    //スレッド
    std::thread th([=]
    {
        Texture::Init();
        m_LoadFinish = true;
    });
    th.detach();

}

void Loading::Update()
{
    Scene::Update();

    if (m_Change)
    {
        if (Manager::GetScene()->GetGameObject<ScenePolygon2D>()->IsFadeOutComplete())
        {
            Manager::SetScene<Title>();
        }
    }
    else
    {
        if (m_LoadFinish)
        {
            Manager::GetScene()->GetGameObject<ScenePolygon2D>()->StartFadeOut();
            m_Change = true;
        }
    }
}