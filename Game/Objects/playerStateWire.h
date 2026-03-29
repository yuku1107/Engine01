/*==============================================================================

    プレイヤー_ワイヤー[playerStateWire.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/16
==============================================================================*/
#ifndef PLAYERSTATEWIRE_H
#define PLAYERSTATEWIRE_H

#include "player.h"

class PlayerStateWire : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float delaytime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEWIRE_H
