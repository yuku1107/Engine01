/*==============================================================================

    プレイヤー_ワイヤーパレット[playerStateWireBullet.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/11/22
==============================================================================*/
#ifndef PLAYERSTATEWIREROTATIONBULLET_H
#define PLAYERSTATEWIREROTATIONBULLET_H

#include "player.h"

class PlayerStateWireBullet : public PlayerState
{
private:

public:
    void Enter(Player* player) override;
    void Update(Player* player, float delaytime) override;
    void Exit(Player* player) override;

};


#endif //PLAYERSTATEWIREROTATIONBULLET_H
