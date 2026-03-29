/*==============================================================================

    プレイヤー_ワイヤーブースター[playerStateWireBuster.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/16
==============================================================================*/
#ifndef PLAYERSTATEWIREBUSTER_H
#define PLAYERSTATEWIREBUSTER_H

#include "player.h"

class PlayerStateWireBuster : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float delaytime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEWIREBUSTER_H
