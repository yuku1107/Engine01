/*==============================================================================

	マネジャー[manager.cpp]
															 Author:郭　哲宇
															 Date  :2025/04/23
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Core/Input/input.h"
#include "Engine/Core/Input/deviceInput.h"
#include "Engine/Core/Input/Mouse/mouse.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Rendering/sprite2d.h"
#include "Engine/Components/shader.h"
#include "Engine/Collision/collision.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/light.h"
#include "Engine/Components/audio.h"
#include "Editor/ImGui/imGuiEngine.h"

//責任分離改良中
#include "game.h"
#include "loading.h"
#include "title.h"
#include "result.h"
#include "Game/Input/gameInput.h"

Scene* Manager::m_Scene = nullptr;
Scene* Manager::m_SceneNext = nullptr;
ImGuiEngine* Manager::m_ImGuiEngine = nullptr;
Collision* Manager::m_Collision = nullptr;
bool Manager::m_CreaterMode = false;
bool Manager::m_UIMode = false;

void Manager::Init()
{
	Renderer::Init();
	ShaderPool::Init();
	Input::Init();
	GameInput::Init();
	Audio::InitMaster();
	Mouse_Initialize(GetWindow());

	m_ImGuiEngine = new ImGuiEngine();    
	m_ImGuiEngine->Init();                

	m_Scene = new Loading();
	m_Scene->Init();

	m_Collision = new Collision();

	m_CreaterMode = false;
	m_UIMode = false;

	Sprite2D::Init();
}


void Manager::Uninit()
{
	Sprite2D::Uninit();

	m_Scene->Uninit();
	delete m_Scene;

	m_ImGuiEngine->Uninit();    
	delete m_ImGuiEngine;       

	delete m_Collision;

	Mouse_Finalize();

	Audio::UninitMaster();

	Texture::Uninit();
	ShaderPool::Uninit();

	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	UpdateFPSCounter();

	Input::Update();
	Mouse_Update();

	m_Scene->Update();

	if (DeviceInput::GetKeyTrigger('P'))
	{
		m_CreaterMode = !m_CreaterMode;
	}

	if (m_CreaterMode)
	{
		if (DeviceInput::GetKeyTrigger('O'))
		{
			m_UIMode = !m_UIMode;
			m_ImGuiEngine->SetSceneUI(m_UIMode);
		}
	}

	m_UIMode = m_ImGuiEngine->GetSceneUI();
}

void Manager::Draw()
{
	m_Scene->DrawShadow();
	
	m_Scene->DrawReflected();

	if (m_CreaterMode)
	{
		Renderer::BeginPE(0);

		m_Scene->Draw();
		m_Scene->Draw3DUI();
		m_Scene->Draw2DUI();

		Renderer::BeginPE(1);

		m_Scene->Draw();
		m_Scene->Draw2DUI();

		Renderer::Begin();

		m_ImGuiEngine->BeginFrame();

		m_ImGuiEngine->Draw();

		m_ImGuiEngine->EndFrame();

		Renderer::End();
	}
	else
	{
		if (m_ImGuiEngine->GetSceneUI())
		{
			m_ImGuiEngine->SetSceneUI(false);
		}

		Renderer::BeginPE(0);

		//オブジェクト
		m_Scene->Draw();
		//3D_UI
		m_Scene->Draw3DUI();
		//2D_UI
		m_Scene->Draw2DUI();

		Renderer::Begin();

		m_Scene->DrawLayer(0);

		Renderer::End();
	}


	//画面遷移
	if (m_SceneNext != nullptr)
	{	
		m_Scene->Uninit();
		delete m_Scene;

		m_Scene = m_SceneNext;
		m_Scene->Init();

		m_SceneNext = nullptr;
	}
}

void Manager::UpdateFPSCounter()
{
	double fps = 0.0;

	static LARGE_INTEGER prev = {};
	LARGE_INTEGER now, freq;
	QueryPerformanceCounter(&now);
	QueryPerformanceFrequency(&freq);

	if (prev.QuadPart != 0)
	{
		double dt = (double)(now.QuadPart - prev.QuadPart) / (double)freq.QuadPart;
		fps = 1.0 / dt;
	}

	prev = now;

	std::wstring title = L"GOST HUNTER FPS: " + std::to_wstring(fps);
	SetWindowTextW(GetWindow(), title.c_str());
}

bool Manager::IsTitle()
{
	return dynamic_cast<Title*>(m_Scene) != nullptr;
}

bool Manager::IsResult()
{
	return dynamic_cast<Result*>(m_Scene) != nullptr;
}

bool Manager::IsLoading()
{
	return dynamic_cast<Loading*>(m_Scene) != nullptr;
}

bool Manager::IsGame()
{
	return dynamic_cast<Game*>(m_Scene) != nullptr;
}
