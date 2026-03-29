/*==============================================================================

    ImGui[imGuiEngine.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/15
==============================================================================*/
#include "main.h"
#define _CRT_SECURE_NO_WARNINGS
#include "ThirdParty/ImGui/imgui.h"
#include "ThirdParty/ImGui/imgui_impl_win32.h"
#include "ThirdParty/ImGui/imgui_impl_dx11.h"
#include "ThirdParty/ImGui/imgui_internal.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Objects/UIObjects/ui.h"
#include "Engine/Objects/UIObjects/button.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/coordinate.h"
#include "Engine/Objects/SceneObjects/coordinate2D.h"
#include "Editor/ImGui/imGuiEngine.h"
#include "Editor/inspectorUtil.h"
#include <map>
#include <vector>
#include <string>

#include "Engine/Core/objectFactory.h"

void ImGuiEngine::Init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    //ImGuiスタイル
    ImGui::StyleColorsClassic();

    ImGuiStyle& style = ImGui::GetStyle();

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    ImGui_ImplWin32_Init(GetWindow());
    ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());
}

void ImGuiEngine::Uninit()
{
    ImGui::SaveIniSettingsToDisk("save.ini");
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiEngine::Draw()
{
    // DockSpace 親ウィンドウ
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);

    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // スタイル変更（DockSpaceを完全に広げる）
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // DockSpace 親ウィンドウの開始
    ImGui::Begin("MainDockSpace", nullptr, window_flags);
    ImGui::PopStyleVar(3);

    DrawMainMenu();

    // DockSpace の描画
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

    if (ImGui::Begin("Hierarchy")) 
    {
        DrawHierarchy();
    }
    ImGui::End();

    if (ImGui::Begin("Objects"))
    {
        DrawObjects();
    }
    ImGui::End();

    if (ImGui::Begin("Inspector")) 
    {
        DrawInspector();
    }
    ImGui::End();
    
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.3f, 0.5f, 1.0f));

    if (ImGui::Begin("Scene"))
    {
        m_IsSceneFocused =
            ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
            ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        DrawSceneView();
    }
    ImGui::End();

    ImGui::PopStyleColor();


    if (ImGui::Begin("Console")) 
    {
        DrawConsole();
        DrawSceneConsole();
    }
    ImGui::End();


    ImGui::End();

    UpdateMouseState();

    DrawFileDialogs(); 
}

void ImGuiEngine::BeginFrame()
{
    ImGui_ImplWin32_NewFrame();
    ImGui_ImplDX11_NewFrame();
    ImGui::NewFrame();
}

void ImGuiEngine::EndFrame()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImGuiEngine::UpdateMouseState()
{
    ImVec2 global_mouse = ImGui::GetMousePos();
    ImGuiWindow* hovered_window = GImGui->HoveredWindow;

    if (hovered_window)
    {
        if (m_IsSceneFocused)
        {
            //テクスチャに合わせて
            constexpr float LEFT = 10.0f;
            constexpr float RIGHT = 10.0f;
            constexpr float TOP = 25.0f;
            constexpr float BOTTOM = 10.0f;

            float scene_w = hovered_window->Size.x - LEFT - RIGHT;
            float scene_h = hovered_window->Size.y - TOP - BOTTOM;

            ImVec2 scale = { SCREEN_WIDTH / scene_w,SCREEN_HEIGHT / scene_h };

            ImVec2 window_pos = hovered_window->Pos;
            ImVec2 scroll = hovered_window->Scroll;

            ImVec2 local = { global_mouse.x - window_pos.x - scroll.x - LEFT,global_mouse.y - window_pos.y - scroll.y - TOP };

            if (local.x >= 0 && local.y >= 0 && local.x <= scene_w && local.y <= scene_h)
            {
                m_LocalMouse.x = local.x * scale.x;
                m_LocalMouse.y = local.y * scale.y;
            }
            else
            {
                m_LocalMouse = ImVec2(-1, -1);
            }
        }
        else
        {
            ImVec2 window_pos = hovered_window->Pos;
            ImVec2 scroll = hovered_window->Scroll;

            m_LocalMouse.x = global_mouse.x - window_pos.x - scroll.x;
            m_LocalMouse.y = global_mouse.y - window_pos.y - scroll.y;
        }

        m_HoveredWindowName = hovered_window->Name;
    }
    else
    {
        m_LocalMouse = ImVec2(-1, -1);
        m_HoveredWindowName.clear();
    }
    
}

void ImGuiEngine::DrawMainMenu()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Menu"))
        {
            if (ImGui::MenuItem("AutoSave"))
            {
                Manager::GetScene()->SaveAllGameObjects("Save/data.bin");
            }

            if (ImGui::MenuItem("AutoLoad"))
            {
                Manager::GetScene()->LoadAllGameObjects("Save/data.bin");
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void ImGuiEngine::DrawFileDialogs()
{
    ImGui::Begin("Save/Load");

    if (ImGui::Button("Open Save Dialog"))
        ImGui::OpenPopup("Save File");

    if (ImGui::Button("Open Load Dialog"))
        ImGui::OpenPopup("Load File");

    static char fileName[128] = "data1";
    static int format = 1; // 0 = Binary, 1 = JSON

    // ==========================
    // Save Popup
    // ==========================
    if (ImGui::BeginPopupModal("Save File"))
    {
        ImGui::InputText("File Name", fileName, IM_ARRAYSIZE(fileName));

        ImGui::RadioButton("Binary", &format, 0);
        ImGui::SameLine();
        ImGui::RadioButton("JSON", &format, 1);

        std::string fullPath = "Save/";
        fullPath += fileName;
        fullPath += (format == 0) ? ".bin" : ".json";

        ImGui::Text("Will save as: %s", fullPath.c_str());

        if (ImGui::Button("OK"))
        {
            Manager::GetScene()->DataSave(fullPath);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    // ==========================
    // Load Popup
    // ==========================
    if (ImGui::BeginPopupModal("Load File"))
    {
        ImGui::InputText("File Name", fileName, IM_ARRAYSIZE(fileName));

        ImGui::RadioButton("Binary", &format, 0);
        ImGui::SameLine();
        ImGui::RadioButton("JSON", &format, 1);

        std::string fullPath = "Save/";
        fullPath += fileName;
        fullPath += (format == 0) ? ".bin" : ".json";

        ImGui::Text("Will load: %s", fullPath.c_str());

        if (ImGui::Button("OK"))
        {
            Manager::GetScene()->DataLoad(fullPath);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel"))
            ImGui::CloseCurrentPopup();

        ImGui::EndPopup();
    }

    ImGui::End();
}

void ImGuiEngine::DrawHierarchy()
{
    ImGui::Text("=== Hierarchy ===");

    Coordinate* coordinate = Manager::GetScene()->GetGameObject<Coordinate>();
    Coordinate2D* coordinate2D = Manager::GetScene()->GetGameObject<Coordinate2D>();

    //ボタン
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));        // 通常
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.5f, 0.9f, 1.0f)); // ホバー時
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.3f, 0.7f, 1.0f));  // 押したとき
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);                        // 角
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 0));                 // パディング
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);                      // 枠太さ
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));        // 枠色

    if (ImGui::TreeNode("Layer0"))
    {
        auto& object0 = Manager::GetScene()->GetGameObjects(0);
        int num = 0;

        for (auto obj : object0)
        {
            std::string label = obj->GetObjectClassName() + "##Layer0_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }
            }

            num++;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Layer1"))
    {
        auto& object1 = Manager::GetScene()->GetGameObjects(1);
        int num = 0;

        for (auto obj : object1)
        {
            std::string label= obj->GetObjectClassName() + "##Layer1_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }

                if (coordinate2D)
                {
                    coordinate2D->SetTarget(obj);
                }

            }
            
            num++;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Layer2"))
    {
        auto& object2 = Manager::GetScene()->GetGameObjects(2);
        int num = 0;

        for (auto obj : object2)
        {
            std::string label = obj->GetObjectClassName() + "##Layer2_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }
            }

            num++;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Layer3"))
    {
        auto& object3 = Manager::GetScene()->GetGameObjects(3);
        int num = 0;

        for (auto obj : object3)
        {
            std::string label = obj->GetObjectClassName() + "##Layer3_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }
            }

            num++;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Layer4"))
    {
        auto& object4 = Manager::GetScene()->GetGameObjects(4);
        int num = 0;

        for (auto obj : object4)
        {
            std::string label = obj->GetObjectClassName() + "##Layer4_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }
            }

            num++;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Layer5"))
    {
        auto& object5 = Manager::GetScene()->GetGameObjects(5);
        int num = 0;

        for (auto obj : object5)
        {
            std::string label = obj->GetObjectClassName() + "##Layer5_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }
            }

            num++;
        }
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Layer6"))
    {
        auto& object6 = Manager::GetScene()->GetGameObjects(6);
        int num = 0;

        for (auto obj : object6)
        {
            std::string label = obj->GetObjectClassName() + "##Layer6_" + std::to_string(num);

            if (ImGui::Button(label.c_str()))
            {
                if (coordinate)
                {
                    coordinate->SetTarget(obj);
                }
            }

            num++;
        }
        ImGui::TreePop();
    }

    //ボタンPop
    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);
}

void ImGuiEngine::DrawObjects()
{
    auto camera = Manager::GetScene()->GetGameObject<Camera>();
    Vector3 spawnPos = { 0.0f,0.0f,0.0f };

    if (camera)
    {
        spawnPos = camera->GetPosition() + camera->GetForward() * 5.0f;
    }
    

    ImGui::Text("=== Objects ===");

    const auto& map = ObjectFactory::GetMap();

    // Layerごとに整理
    std::map<int, std::vector<std::pair<std::string, ObjectFactory::ObjectInfo>>> layers;

    for (auto& pair : map)
    {
        layers[pair.second.DefaultLayer].push_back(pair);
    }

    // Layerごと表示
    for (auto& layer : layers)
    {
        int layerIndex = layer.first;

        ImGui::Text("LAYER_%d", layerIndex);

        for (auto& obj : layer.second)
        {
            const std::string& name = obj.first;
            const auto& info = obj.second;

            ImGui::SameLine();

            if (ImGui::Button(name.c_str()))
            {
                GameObject* newObj = info.Create();

                Manager::GetScene()->AddGameObject(newObj, info.DefaultLayer);

                newObj->SetPosition(spawnPos);
            }
        }
    }
}

void ImGuiEngine::DrawInspector()
{
    ImGui::Text("=== Inspector ===");

    static bool show_data_window = false;

    if (m_SceneUI)
    {
        Coordinate2D* coordinate2D = Manager::GetScene()->GetGameObject<Coordinate2D>();

        auto* target = coordinate2D->GetTarget();
        
        ImGui::PushID(target);

        if (target)
        {
            show_data_window = true;
        }
        else
        {
            show_data_window = false;
        }

        if (show_data_window)
        {
            InspectorUtil::DrawTransform(target);

            target->DrawInspector();

            if (ImGui::Button("Delete"))
            {
                target->SetDestroy();
            }
        }

        ImGui::PopID();
        
    }
    else
    {
        Coordinate* coordinate = Manager::GetScene()->GetGameObject<Coordinate>();
        
        auto* target = coordinate->GetTarget();

        ImGui::PushID(target);

        if (target)
        {
            show_data_window = true;
        }
        else
        {
            show_data_window = false;
        }

        if (show_data_window)
        {
            InspectorUtil::DrawTransform(target);
            InspectorUtil::DrawCollision(target);
            InspectorUtil::DrawShader(target);

            target->DrawInspector();

            if (ImGui::Button("Delete"))
            {
                target->SetDestroy();
            }
        }

        ImGui::PopID();
    }
}

void ImGuiEngine::DrawSceneView()
{
    ImVec2 size = ImGui::GetContentRegionAvail();

    if (m_SceneUI)
    {
        m_RenderTarget = Renderer::GetPETexture(1);
    }
    else
    {
        m_RenderTarget = Renderer::GetPETexture(0);
    }

    
    ImGui::Image((ImTextureID)m_RenderTarget, size);
}

void ImGuiEngine::DrawConsole()
{
    ImGui::Text("=== Console ===");

    // マウスの現在位置を取得
    ImVec2 mousePos = ImGui::GetMousePos();
    ImGui::Text("Mouse Pos: (%.1f, %.1f)", mousePos.x, mousePos.y);

    ImGui::Text("Local Mouse: (%.1f, %.1f)", m_LocalMouse.x, m_LocalMouse.y);

    if (!m_HoveredWindowName.empty())
    {
        ImGui::Text("Hovered Window: %s", m_HoveredWindowName.c_str());
    }
    else
    {
        ImGui::Text("Hovered Window: (none)");
    }

}

void ImGuiEngine::DrawSceneConsole()
{
    ImGui::Text("=== SceneConsole ===");

    if (ImGui::Button("GameScene"))
    {
        m_SceneUI = false;
    }

    ImGui::SameLine();
    if (ImGui::Button("UIScene"))
    {
        m_SceneUI = true;
    }

}
