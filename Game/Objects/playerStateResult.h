/*==============================================================================

    プレイヤー_リザルト画面[playerStateResult.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/11
==============================================================================*/
#ifndef PLAYERSTATERESULT_H
#define PLAYERSTATERESULT_H

#include "player.h"

class PlayerStateResult : public PlayerState
{
private:

public:
    void Enter(Player* player) override;
    void Update(Player* player, float deltatime) override;
    void Exit(Player* player) override;
};


#endif //PLAYERSTATERESULT_H
