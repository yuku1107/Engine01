/*==============================================================================

    ローディング画面[loading.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/10
==============================================================================*/
#ifndef LOADING_H
#define LOADING_H

#include "Engine/Core/scene.h"

class Loading :public Scene
{
private:
    bool m_Change = false;
    bool m_LoadFinish;

public:
    void Init() override;
    void Update() override;

};

#endif //LOADING_H
