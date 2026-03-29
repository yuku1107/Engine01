/*==============================================================================

	新要素紹介ウィンドウ[discoveryWindow.h]
															 Author:郭　哲宇
															 Date  :2026/01/27
==============================================================================*/
#ifndef DISCOVERYWINDOW_H
#define DISCOVERYWINDOW_H

#include "Engine/Objects/UIObjects/ui.h"

enum WINDOW_STATE
{
	WINDOW_STATE_DEFAULT,
	WINDOW_STATE_UP,
	WINDOW_STATE_DOWN,
	WINDOW_STATE_SHOW
};

enum WINDOW_SHOW_STATE
{
	WINDOW_SHOW_STATE_0,
	WINDOW_SHOW_STATE_1,
	WINDOW_SHOW_STATE_2,
	WINDOW_SHOW_STATE_3,
	WINDOW_SHOW_STATE_4,
	WINDOW_SHOW_STATE_5,
	WINDOW_SHOW_STATE_6,
	WINDOW_SHOW_STATE_7,
	WINDOW_SHOW_STATE_8,
	WINDOW_SHOW_STATE_COUNT
};

static const char* WINDOW_SHOW_STATE_NAMES[] =
{
	"State_0",
	"State_1",
	"State_2",
	"State_3",
	"State_4",
	"State_5",
	"State_6",
	"State_7",
	"State_8",
	
};

class DiscoveryWindow : public UI
{
private:
	float m_Speed = 10.0f;
	WINDOW_STATE m_State;
	WINDOW_SHOW_STATE m_ShowState = WINDOW_SHOW_STATE_0;
	WINDOW_SHOW_STATE m_NextShowState = WINDOW_SHOW_STATE_0;
	bool m_RequestRefresh = false;

	std::string m_DefaultTextureName;
protected:
	std::string GetShaderName() const override { return "unlitTexture"; }

public:
	void Init() override;   //初期化
	void UpdateSelf() override; //更新

	WINDOW_SHOW_STATE GetShowState() { return m_ShowState; }
	void SetShowState(WINDOW_SHOW_STATE state) { m_ShowState = state; }

	void ChangeShowState(WINDOW_SHOW_STATE state);

	std::string GetObjectClassName() const override { return "DiscoveryWindow"; }
};

#endif //DISCOVERYWINDOW_H
