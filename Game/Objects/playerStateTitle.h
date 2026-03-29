/*==============================================================================

    プレイヤー_タイトル画面[playerStateTitle.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/11
==============================================================================*/
#ifndef PLAYERSTATETITLE_H
#define PLAYERSTATETITLE_H

#include "player.h"

class PlayerStateTitle : public PlayerState
{
private:

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATETITLE_H

