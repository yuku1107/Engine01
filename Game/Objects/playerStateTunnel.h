/*==============================================================================

    プレイヤー_トンネル[playerStateTunnel.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/01/23
==============================================================================*/
#ifndef PLAYERSTATETUNNEL_H
#define PLAYERSTATETUNNEL_H

#include "player.h"

class PlayerStateTunnel : public PlayerState
{
private:

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATETUNNEL_H
