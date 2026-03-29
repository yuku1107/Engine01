/*==============================================================================

    新要素紹介ウィンドウ[discoveryWindow.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2026/01/27
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"
#include "discoveryWindow.h"

#include "Game/Input/gameInput.h"
void DiscoveryWindow::Init()
{
    UI::Init();

    m_State = WINDOW_STATE_DEFAULT;
    m_ShowState = WINDOW_SHOW_STATE_1;
    m_Position = Vector3(0.0f, 0.0f, 0.0f);
    m_Scale = Vector3(100.0f, 100.0f, 1.0f);
    m_DefaultTextureName = "discovery0";

    m_Tag = "DISCOVERYWINDOW";
}

void DiscoveryWindow::UpdateSelf()
{
    if (Manager::GetCreaterMode())
    {
        m_DrawEnable = true;
    }


    switch (m_State)
    {
    case WINDOW_STATE_DEFAULT:
        m_Scale = Vector3(100.0f, 100.0f, 1.0f);
        SetTextureName(m_DefaultTextureName);
        break;
    case WINDOW_STATE_UP:
        m_Scale = Vector3(300.0f, 400.0f, 1.0f);

        if (m_Position.y > -m_Scale.y)
        {
            m_Position.y -= m_Speed;
        }
        else
        {
            m_Position.y = 0.0f;

            if (m_RequestRefresh)
            {
                m_Scale = Vector3(300.0f, 400.0f, 1.0f);
                // 新しい内容に変更して再表示
                m_ShowState = m_NextShowState;
                m_Position.y = -300.0f;
                m_State = WINDOW_STATE_DOWN;
                m_RequestRefresh = false;
            }
            else
            {
                m_Scale = Vector3(100.0f, 100.0f, 1.0f);
                SetTextureName(m_DefaultTextureName);
                m_State = WINDOW_STATE_DEFAULT;
            }
        }
        break;
    case WINDOW_STATE_DOWN:
        if (m_Position.y < 0.0f)
        {
            switch (m_ShowState)
            {
            case WINDOW_SHOW_STATE_0:
                SetTextureName("discovery0");
                break;
            case WINDOW_SHOW_STATE_1:
                SetTextureName("discovery1");
                break;
            case WINDOW_SHOW_STATE_2:
                SetTextureName("discovery2");
                break;
            case WINDOW_SHOW_STATE_3:
                SetTextureName("discovery3");
                break;
            case WINDOW_SHOW_STATE_4:
                SetTextureName("discovery4");
                break;
            case WINDOW_SHOW_STATE_5:
                SetTextureName("discovery5");
                break;
            case WINDOW_SHOW_STATE_6:
                SetTextureName("discovery6");
                break;
            case WINDOW_SHOW_STATE_7:
                SetTextureName("discovery7");
                break;
            case WINDOW_SHOW_STATE_8:
                SetTextureName("discovery8");
                break;
            }
            m_Position.y += m_Speed;
        }
        else
        {
            m_State = WINDOW_STATE_SHOW;
        }
        break;
    case WINDOW_STATE_SHOW:

        break;
    }

    if (GameInput::GetDiscoveryUITrigger())
    {
        switch (m_State)
        {
        case WINDOW_STATE_DEFAULT:
            m_Scale = Vector3(300.0f, 400.0f, 1.0f);
            m_Position.y = -m_Scale.y;
            m_State = WINDOW_STATE_DOWN;
            break;
        case WINDOW_STATE_SHOW:
            m_State = WINDOW_STATE_UP;
            break;
        }
    }
}

void DiscoveryWindow::ChangeShowState(WINDOW_SHOW_STATE state)
{
    // 同じ内容なら何もしない（連続発火防止）
    if (state == m_ShowState && m_State != WINDOW_STATE_DEFAULT)
        return;

    m_NextShowState = state;
    m_RequestRefresh = true;

    switch (m_State)
    {
    case WINDOW_STATE_DEFAULT:
        // 何も出ていない → そのまま出す
        m_ShowState = m_NextShowState;
        m_Scale = Vector3(300.0f, 400.0f, 1.0f);
        m_Position.y = -m_Scale.y;
        m_State = WINDOW_STATE_DOWN;
        m_RequestRefresh = false;
        break;

    case WINDOW_STATE_DOWN:
    case WINDOW_STATE_SHOW:
        // 表示中 → 一旦閉じる
        m_State = WINDOW_STATE_UP;
        break;

    case WINDOW_STATE_UP:
        // すでに閉じ中 → 閉じたあと再表示させる
        break;
    }
}
