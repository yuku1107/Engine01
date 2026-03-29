/*==============================================================================

	入力システム管理クラス[input.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/
#ifndef INPUT_H
#define INPUT_H

class Input
{
public:
    static void Init(); 
    static void Uninit();
    static void Update();
};

#endif //INPUT_H