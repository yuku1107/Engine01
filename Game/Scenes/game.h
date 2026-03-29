/*==============================================================================

    ÉQÅ[ÉÄâÊñ [game.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/06/25
==============================================================================*/
#ifndef GAME_H
#define GAME_H

#include "Engine/Core/scene.h"

class Game;

class GameState
{
public:
    virtual ~GameState() {}
    virtual void Enter(Game* game) = 0;
    virtual void Update(Game* game, float deltatime) = 0;
    virtual void Exit(Game* game) = 0;
};

class Game :public Scene
{
private:
    GameState* m_CurrentState;
    int m_ToStage = 0;

    bool m_Stage1Cleared = false;
    bool m_Stage2Cleared = false;
    bool m_Stage3Cleared = false;

public:
    void Init() override;
    void Update() override;

    void SetToStage(int stage) { m_ToStage = stage; }
    int GetToStage() { return m_ToStage; }

    void ChangeState(GameState* newState);

    void SetStage1Cleared(bool cleared) { m_Stage1Cleared = cleared; }
    bool IsStage1Cleared() const { return m_Stage1Cleared; }

    void SetStage2Cleared(bool cleared) { m_Stage2Cleared = cleared; }
    bool IsStage2Cleared() const { return m_Stage2Cleared; }

    void SetStage3Cleared(bool cleared) { m_Stage3Cleared = cleared; }
    bool IsStage3Cleared() const { return m_Stage3Cleared; }

};

#endif //GAME_H
