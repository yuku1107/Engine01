/*==============================================================================

    メイン[main.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/04/23
==============================================================================*/
#ifndef MAIN_H
#define MAIN_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "Engine/Math/vector3.h"

//テクスチャ読み込む用ライブラリ
#include "ThirdParty/DirectXTex/include/DirectXTex.h"

#if _DEBUG
#pragma comment(lib,"ThirdParty/DirectXTex/lib/DirectXTex_Debug.lib")
#else
#pragma comment(lib,"ThirdParty/DirectXTex/lib/DirectXTex_Release.lib")
#endif




#pragma comment (lib, "winmm.lib")


#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)


HWND GetWindow();

void Invoke(std::function<void()> Function, int Time);

#endif //MAIN_H