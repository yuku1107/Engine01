/*==============================================================================

	ワイヤロープ[wire.h]
															 Author:郭　哲宇
															 Date  :2025/07/09
==============================================================================*/
#ifndef WIRE_H
#define WIRE_H

#include "Engine/Core/gameObject.h"

#define WIRE_MAX_LENGTH         (15.0f)
#define WIRE_TOTAL              (21)
#define WIRE_MASS               (0.5f)
#define WIRE_GRAVITY_LINKED     (50.0f)
#define WIRE_ATTENUATION        (1.0f)

class Wire : public GameObject
{
public:
    struct Spring
    {
        Vector3 position{};
        Vector3 velocity{};
        Vector3 acceleration{};
    };

private:
    ID3D11ShaderResourceView* m_Texture{};
    class LineMesh* m_LineMesh = nullptr;

    Spring m_Spring[WIRE_TOTAL];
    Vector3 m_Start{};
    Vector3 m_End{};
    Vector3 m_Target{};

    GameObject* m_StartObject = nullptr;
    GameObject* m_EndObject = nullptr;

    float m_MaxLength;
    float m_Length = 0.0f;
    float m_ExtendSpeed = 30.0f;

    //伸びる中フラグ
    bool m_IsExtending = false;

    //繋がたフラウ
    bool m_Linked = false;

    //高速回収フラグ
    bool m_Buster = false;

public:
    void Init() override;
    void UninitSelf() override;
    void UpdateSelf() override;
    void DrawSelf() override;

    void Set(const Vector3& start, const Vector3& end, GameObject* startObj = nullptr, GameObject* endObj = nullptr, float width = 0.2f);
    void UnSet();
    bool GetBuster() { return m_Buster; }
    void SetBuster(bool buster) { m_Buster = buster; }

    const Vector3& GetStartPosition() const { return m_Start; }
    const Vector3& GetEndPosition() const { return m_End; }
    float GetLength() const { return m_Length; }
    float GetMaxLength() const { return m_MaxLength; }
    void AddMaxLength(float delta);

    bool GetLinked()
    {
        if (m_IsExtending)
        {
            return false;
        }

        float endDist = (m_EndObject->GetPosition() - m_StartObject->GetPosition()).length();

        const float linkOn = 0.995f;
        const float linkOff = 0.985f;

        if (m_Linked)
        {
            if (endDist < m_MaxLength * linkOff)
                m_Linked = false;
        }
        else
        {
            if (endDist > m_MaxLength * linkOn)
                m_Linked = true;
        }
        
        return m_Linked; 
    }

    bool GetExtending() { return m_IsExtending; }

    GameObject* GetStartObject() { return m_StartObject; }
    GameObject* GetEndObject() { return m_EndObject; }

    Spring* GetSpring() { return m_Spring; }

    // 物理シミュレーション
    void ApplyPositionConstraint();

    std::string GetObjectClassName() const override { return "Wire"; }
};

#endif //WIRE_H
