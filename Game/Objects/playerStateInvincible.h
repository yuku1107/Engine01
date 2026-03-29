/*==============================================================================

    ÉvÉåÉCÉÑÅ[_ñ≥ìG[playerStateInvincible.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/12/16
==============================================================================*/
#ifndef PLAYERSTATEINVINCIBLE_H
#define PLAYERSTATEINVINCIBLE_H

#include "player.h"

class PlayerStateInvincible : public PlayerState
{
private:
    int m_Count = 90;

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEINVINCIBLE_H
