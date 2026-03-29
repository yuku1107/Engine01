/*==============================================================================

    新要素紹介エリア[discoveryArea.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/02/09
==============================================================================*/
#ifndef DISCOVERYAREA_H
#define DISCOVERYAREA_H

#include "Engine/Core/gameObject.h"
#include "discoveryWindow.h"

class Player;

class DiscoveryArea : public GameObject
{
private:
    WINDOW_SHOW_STATE m_ShowState;

    bool m_PlayerInside = false;
    Player* m_Player = nullptr;
    DiscoveryWindow* m_Window = nullptr;
    class CubeMesh* m_CubeMesh;

public:
    void Init() override;   //初期化
    void UninitSelf() override;//終了
    void UpdateSelf() override; //更新
    void DrawSelf() override;   //描画
    void DrawInspector() override;

    bool IsInside(const Vector3& point)const;
    void SetState(WINDOW_SHOW_STATE state);
    WINDOW_SHOW_STATE GetState() { return m_ShowState; }

    void Serialize(Serializer& s) override;

    void Deserialize(Serializer& s) override;

    std::string GetObjectClassName() const override { return "DiscoveryArea"; }
};

#endif //DISCOVERYAREA_H
