/*==============================================================================

    プレイヤー_クリエーターモード[playerStateCreater.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/02/02
==============================================================================*/
#ifndef PLAYERSTATESCREATER_H
#define PLAYERSTATESCREATER_H

#include "player.h"

class PlayerStateCreater : public PlayerState
{
private:

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATESCREATER_H
