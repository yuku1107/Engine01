/*==============================================================================

    ÉèÉCÉÑÉçÅ[ÉvîªíË[wireComponent.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/28
==============================================================================*/
#ifndef WIRECOMPONENT_H
#define WIRECOMPONENT_H

#include "Engine/Components/component.h"

class WireComponent : public Component
{
public:
    enum class State
    {
        Idle,
        Targeted,
        Attached
    };

private:
    State m_State = State::Idle;
    bool  m_IsWireable = true;

public:

    void SetWireable(bool value) { m_IsWireable = value; }
    bool IsWireable() const { return m_IsWireable; }

    bool CanBeWired() const
    {
        return m_IsWireable;
    }

    bool IsTargeted() const { return m_State == State::Targeted; }
    void SetTargeted() { m_State = State::Targeted; }

    bool IsAttached() const { return m_State == State::Attached; }
    void SetAttached() { m_State = State::Attached; }

    void Clear() { if (m_State != State::Attached)m_State = State::Idle; }
};

#endif //WIRECOMPONENT_H
