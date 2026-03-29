/*==============================================================================

    ÉvÉåÉCÉÑÅ[_ínñ [playerStateMove.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/07/16
==============================================================================*/
#ifndef PLAYERSTATEMOVE_H
#define PLAYERSTATEMOVE_H

#include "player.h"

class PlayerStateGround : public PlayerState
{
public:
    void Enter(Player* player) override;
    void Update(Player* player, float delaytime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEMOVE_H
