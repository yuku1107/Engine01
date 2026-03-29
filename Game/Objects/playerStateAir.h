/*==============================================================================

    ƒvƒŒƒCƒ„[_‹ó’†[playerStateAir.h]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/07/16
==============================================================================*/
#ifndef PLAYERSTATEAIR_H
#define PLAYERSTATEAIR_H

#include "player.h"

class PlayerStateAir : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEAIR_H