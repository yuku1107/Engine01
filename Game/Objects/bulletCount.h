/*==============================================================================

    ’e‚Ì”[bulletCount.h]
                                                             Author:Šs@“N‰F
                                                             Date  :2026/02/13
==============================================================================*/
#ifndef BULLETCOUNT_H
#define BULLETCOUNT_H

#include "Engine/Core/gameObject.h"
#include "player.h"
#include "number.h"

class BulletCountUI : public GameObject
{
private:
    Player* m_Player = nullptr;
    NumberUI* m_Number = nullptr;

    int m_LastBullet = -1;

public:
    void Init() override
    {
        m_Number = Manager::GetScene()->AddGameObject<NumberUI>(1);
        m_Number->InitNumber(1, m_Position.x, m_Position.y, m_Scale.x, m_Scale.y);
        m_Number->SetSaveEnable(false);

        m_Tag = "BULLET_UI";
        m_DrawEnable = true;
    }

    void UninitSelf() override
    {
        m_Number->SetDestroy();
    }

    void UpdateSelf() override
    {
        if (!m_Player)
        {
            m_Player = Manager::GetScene()->GetGameObject<Player>();
            if (!m_Player) return;
        }

        if (!m_Number) return;

        int bullet = m_Player->GetBulletNum();

        m_Number->SetPosition(m_Position);
        m_Number->SetScale(m_Scale);
        m_Number->SetMaterialColor(m_MaterialColor);

        if (bullet == 0 || bullet == m_Player->GetBulletMax())
        {
            SetMaterialColor(XMFLOAT4(1, 0, 0, 1));
        }
        else
        {
            SetMaterialColor(XMFLOAT4(1, 1, 1, 1));
        }

        // •Ï‰»‚µ‚½Žž‚¾‚¯XV
        if (bullet != m_LastBullet)
        {
            m_Number->SetValue(bullet);
            m_LastBullet = bullet;
        }
    }

    std::string GetObjectClassName() const override { return "BulletCountUI"; }
};


#endif //BULLETCOUNT_H