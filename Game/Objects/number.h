/*==============================================================================

    数字[number.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/02/13
==============================================================================*/
#ifndef NUMBER_H
#define NUMBER_H

#include "Engine/Core/gameObject.h"
#include "Engine/Objects/UIObjects/ui.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"


// 任意桁数の数値表示（右詰め表示）
class NumberUI : public GameObject
{
private:
    std::vector<DigitUI*> m_Digits;  // 右から左へ並べる
    int m_Value = 0;

    int m_DigitCount = 1;
    float m_Spacing = 0.0f; // 桁間

public:
    void InitNumber(int digitCount, float x, float y, float digitW, float digitH, float spacing = 0.0f)
    {
        m_DigitCount = std::max(1, digitCount);
        m_Position = { x, y, 0.0f };
        m_Scale = { digitW, digitH, 1.0f };
        m_Spacing = spacing;

        m_Tag = "NUMBER_UI";
        m_DrawEnable = true;

        // Digitを生成（右から左へ並べる想定）
        m_Digits.clear();
        m_Digits.reserve(m_DigitCount);

        for (int i = 0; i < m_DigitCount; i++)
        {
            // i=0 が一番右の桁（1の位）
            float dx = (m_Scale.x + m_Spacing) * (m_DigitCount - 1 - i);
            float px = m_Position.x + dx;
            float py = m_Position.y;

            auto* d = Manager::GetScene()->AddGameObject<DigitUI>(/*layer*/ 1);
            d->InitDigit(px, py, m_Scale.x, m_Scale.y);
            d->SetSaveEnable(false);

            m_Digits.push_back(d);
        }

        SetValue(0);
    }

    void UninitSelf() override
    {
        m_Digits.clear();
    }

    void UpdateSelf() override
    {
        for (int i = 0; i < (int)m_Digits.size(); i++)
        {
            float x = m_Position.x + (m_Scale.x + m_Spacing) * i;
            float y = m_Position.y;
            m_Digits[i]->SetPosition({ x, y, 0.0f });
            m_Digits[i]->SetScale(m_Scale);
            m_Digits[i]->SetMaterialColor(m_MaterialColor);
        }
    }

    void CreateDigits()
    {
        m_Digits.clear();
        m_Digits.reserve(m_DigitCount);

        for (int i = 0; i < m_DigitCount; i++)
        {
            auto* d = Manager::GetScene()->AddGameObject<DigitUI>(1);

            // まだ位置は決めない
            d->InitDigit(0.0f, 0.0f, m_Scale.x, m_Scale.y);
            d->SetSaveEnable(false);

            m_Digits.push_back(d);
        }
    }

    void SetValue(int v)
    {
        m_Value = std::max(0, v);

        int value = m_Value;
        for (int i = m_DigitCount - 1; i >= 0; --i)
        {
            int num = value % 10;
            value /= 10;

            m_Digits[i]->SetDigit(num);
        }
    }

    int GetValue() const { return m_Value; }

    std::string GetObjectClassName() const override { return "NumberUI"; }
};


// タイマー "MM:SS" UI
// NumberUI を2つ（分/秒）と ":" を1つ並べる
class TimerUI : public GameObject
{
private:
    NumberUI* m_Minutes = nullptr;
    NumberUI* m_Seconds = nullptr;
    UI* m_Colon = nullptr;

    float m_TimeSec = 0.0f;
    bool m_Running = false;

    // レイアウト
    float m_Spacing = 0.0f;
    float m_BlockGap = 10.0f; // 分とコロン/秒の間隔

public:
    void InitTimer(float x, float y, float digitW, float digitH, float spacing = 0.0f)
    {
        m_Position = { x,y,0.0f };
        m_Scale.x = digitW;
        m_Scale.y = digitH;
        m_Spacing = spacing;

        m_Tag = "TIMER_UI";
        m_DrawEnable = true;

        // 分 (2桁)
        m_Minutes = Manager::GetScene()->AddGameObject<NumberUI>(1);
        m_Minutes->InitNumber(2, m_Position.x, m_Position.y, m_Scale.x, m_Scale.y, m_Spacing);
        m_Minutes->SetSaveEnable(false);

        float colonX = m_Position.x + (m_Scale.x + m_Spacing) * 2 + m_BlockGap;
        m_Colon = Manager::GetScene()->AddGameObject<UI>(1);
        m_Colon->Init(colonX, m_Position.y, m_Scale.x * 0.5f, m_Scale.y, "colon"); // サイズは好みで
        m_Colon->SetSaveEnable(false);

        // 秒 (2桁)
        float secX = colonX + (m_Scale.x * 0.5f) + m_BlockGap;
        m_Seconds = Manager::GetScene()->AddGameObject<NumberUI>(1);
        m_Seconds->InitNumber(2, secX, m_Position.y, m_Scale.x, m_Scale.y, m_Spacing);
        m_Seconds->SetSaveEnable(false);

        SetTime(0.0f);
        Stop();
    }

    void UpdateSelf() override
    {
        if (!m_Running) return;

        m_Minutes->SetPosition(m_Position);
        m_Minutes->SetScale(m_Scale);
        m_Minutes->SetMaterialColor(m_MaterialColor);

        Vector3 colonPos = m_Position;
        colonPos.x = m_Position.x + (m_Scale.x + m_Spacing) * 2 + m_BlockGap;
        m_Colon->SetPosition(colonPos);
        m_Colon->SetMaterialColor(m_MaterialColor);

        Vector3 secPos = m_Position;
        secPos.x = colonPos.x + (m_Scale.x * 0.5f) + m_BlockGap;
        m_Seconds->SetPosition(secPos);
        m_Seconds->SetScale(m_Scale);
        m_Seconds->SetMaterialColor(m_MaterialColor);

        m_TimeSec += 1.0f / 60.0f;
        ApplyTimeToDigits();
    }

    void SetTime(float sec)
    {
        m_TimeSec = std::max(0.0f, sec);
        ApplyTimeToDigits();
    }

    float GetTime() const { return m_TimeSec; }

    void Start() { m_Running = true; }
    void Stop() { m_Running = false; }
    void Reset() { SetTime(0.0f); }

private:
    void ApplyTimeToDigits()
    {
        int t = static_cast<int>(m_TimeSec);
        int mm = t / 60;
        int ss = t % 60;

        if (m_Minutes) m_Minutes->SetValue(mm);
        if (m_Seconds) m_Seconds->SetValue(ss);
    }

public:
    std::string GetObjectClassName() const override { return "TimerUI"; }
};


#endif //NUMBER_H