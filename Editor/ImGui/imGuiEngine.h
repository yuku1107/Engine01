/*==============================================================================

    ImGuiゲームエンジン[imGuiEngine.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/15
==============================================================================*/
#ifndef IMGUIENGINE_H
#define IMGUIENGINE_H

#include "ThirdParty/ImGui/imgui.h"
#include <string>

class ImGuiEngine
{
private:
    XMFLOAT4 clear_color = { 0.5f,0.9f,0.5f,1.0f };

    ID3D11ShaderResourceView* m_RenderTarget = nullptr;

    ImVec2 m_LocalMouse = ImVec2(0, 0);
    std::string m_HoveredWindowName;

    void DrawMainMenu();      
    void DrawFileDialogs();
    void DrawHierarchy();     
    void DrawObjects();
    void DrawInspector();     
    void DrawSceneView();     
    void DrawConsole();       
    void DrawSceneConsole();
    bool m_SceneUI = false;
    bool m_IsSceneFocused = false;
public:
    void Init();   // ImGui初期化（Context作成、Impl初期化）
    void Uninit(); // ImGui終了処理（Impl解放、DestroyContext）

    void Draw();   

    void BeginFrame();
    void EndFrame();

    const ImVec2& GetLocalMousePos() const { return m_LocalMouse; }
    const std::string& GetHoveredWindowName() const { return m_HoveredWindowName; }
    const bool GetSceneUI()const { return m_SceneUI; }
    void SetSceneUI(bool ui) { m_SceneUI = ui; }
    void UpdateMouseState();

    bool IsSceneFocused() const { return m_IsSceneFocused; }
};
#endif //IMGUIENGINE_H
