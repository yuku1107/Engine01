/*==============================================================================

	マネジャー[manager.h]
															 Author:郭　哲宇
															 Date  :2025/04/23
==============================================================================*/
#ifndef MANAGER_H
#define MANAGER_H

class Manager
{
private:
	static class Scene* m_Scene;
	static class Scene* m_SceneNext;
	static class ImGuiEngine* m_ImGuiEngine;
	static class Collision* m_Collision;

	static bool m_CreaterMode;
	static bool m_UIMode;

public:
	static void Init();   //初期化
	static void Uninit(); //終了
	static void Update(); //更新
	static void Draw();   //描画

	static Scene* GetScene() { return m_Scene; }

	static bool GetCreaterMode() { return m_CreaterMode; }
	static ImGuiEngine* GetGui() { return m_ImGuiEngine; }
	
	static Collision* GetCollision() { return m_Collision; }

	void SetUIMode(bool ui) { m_UIMode = ui; }
	static bool GetUIMode() { return m_UIMode; }

	template <typename T>
	static void SetScene() { m_SceneNext = new T; }
	static void UpdateFPSCounter();

	static bool IsTitle();
	static bool IsResult();
	static bool IsLoading();
	static bool IsGame();
};

#endif //MANAGER_H