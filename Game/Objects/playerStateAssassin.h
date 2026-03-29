/*==============================================================================

    ÉvÉåÉCÉÑÅ[_à√éE[playerStateAssassin.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/11/26
==============================================================================*/
#ifndef PLAYERSTATEASSASSIN_H
#define PLAYERSTATEASSASSIN_H

#include "player.h"

class PlayerStateAssassin : public PlayerState
{
private:
    float m_length = 0.0f;
public:
    void Enter(Player* player) override;
    void Update(Player* player, float delaytime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATEASSASSIN_H
