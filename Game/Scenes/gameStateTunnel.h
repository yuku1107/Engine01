/*==============================================================================

    game_ÉgÉìÉlÉã[gameStateTunnel.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/12/18
==============================================================================*/
#ifndef GAMESTATETUNNEL_H
#define GAMESTATETUNNEL_H

#include "game.h"
#include "transferDevice.h"

class GameStateTunnel : public GameState
{
private:
    TransferDevice::TransferType m_NextStageType;
    bool m_ShouldChangeStage = false;
    class Audio* m_BGM;
    bool m_LoadFinish;
    float m_Time;

public:
    void Enter(Game* game) override;
    void Update(Game* game, float deltatime) override;
    void Exit(Game* game) override;
};


#endif //GAMESTATETUNNEL_H
