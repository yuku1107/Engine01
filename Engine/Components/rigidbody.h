/*==============================================================================

    RIGIDBODY
                                                             Author:郭　哲宇
                                                             Date  :
==============================================================================*/
#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "main.h"
#include "Engine/Core/gameObject.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/shape.h"

class Rigidbody : public Component
{
private:
    Vector3 m_PositionNext = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_Velocity = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_Acceleration = Vector3(0.0f, 0.0f, 0.0f);
    Vector3 m_AngularVelocity = Vector3(0, 0, 0);

    float m_Mass = 0.0f;

    Vector3 m_Resistance = Vector3(0.0f, 0.0f, 0.0f);
    float m_MovePower = 0.0f;

    std::unordered_map<std::string, Vector3> m_ResistanceMap;
    std::unordered_map<std::string, float> m_MovePowerMap;

    bool m_IsGrounded = false;
    bool m_UseGravity = true;
    float m_DeltaTime = 1.0f / 60.0f;

public:
    void Set(GameObject* obj)
    {
        m_pOwner = obj;

        m_ResistanceMap["Default"] = Vector3(0.0f, 0.0f, 0.0f);
        m_MovePowerMap["Default"] = 10.0f;

        SetMovementMode("Default");
    }

    void Uninit() override
    {
        m_ResistanceMap.clear();
        m_MovePowerMap.clear();
    }

    void Update() override
    {
        if (m_UseGravity)
        {
            if (m_Velocity.y < 0)
            {
                // 落下中は重力を強くする
                m_Acceleration += Vector3(0, -9.8f * 2.0f, 0);
            }
            else
            {
                // 上昇中は通常の重力
                m_Acceleration += Vector3(0, -9.8f, 0);
            }
        }

        Vector3 resistance = m_Resistance; 

        if (m_Velocity.y < 0)
        {
            // 落下中は空気抵抗を小さくする
            resistance.y *= 0.3f;
        }

        // 抵抗
        m_Velocity *= (Vector3(1.0f, 1.0f, 1.0f) - resistance);

        // 加速度適用
        m_Velocity += m_Acceleration * m_DeltaTime;

        // 移動
        m_PositionNext = GetpOwner()->GetPosition() + m_Velocity * m_DeltaTime;

        GetpOwner()->SetRotation(GetpOwner()->GetRotation() + m_AngularVelocity);

        // 加速度をリセット
        m_Acceleration = Vector3(0, 0, 0);
    }

    const Vector3 GetPositionNext()const { return m_PositionNext; }
    void SetPositionNext(const Vector3 pos) { m_PositionNext = pos; }

    void SetVelocity(const Vector3& v) { m_Velocity = v; }
    Vector3 GetVelocity() const { return m_Velocity; }

    void AddVelocity(const Vector3& velocity)
    {
        m_Velocity += velocity * m_MovePower;
    }

    void AddAcceleration(const Vector3& acceleration)
    {
        m_Acceleration += acceleration;
    }
    
    void AddForce(const Vector3& force)
    {
        m_Acceleration += force * m_MovePower;
    }

    void SetAngularVelocity(const Vector3 av) { m_AngularVelocity = av; }

    void SetMass(const float& mass) { m_Mass = mass; }
    float GetMass() const { return m_Mass; }

    void SetResistance(const Vector3& res) { m_Resistance = res; }
    const Vector3& GetResistance() const { return m_Resistance; }

    void SetMovePower(float power) { m_MovePower = power; }
    float GetMovePower() const { return m_MovePower; }

    // 状態別パラメータ管理
    void SetResistanceMap(const std::string& type, const Vector3& value) { m_ResistanceMap[type] = value; }
    void SetMovePowerMap(const std::string& type, const float& value) { m_MovePowerMap[type] = value; }

    void SetCurrentResistance(const std::string& type)
    {
        auto it = m_ResistanceMap.find(type);
        if (it != m_ResistanceMap.end())
        {
            m_Resistance = it->second;
        }
    }

    void SetCurrentMovePower(const std::string& type)
    {
        auto it = m_MovePowerMap.find(type);
        if (it != m_MovePowerMap.end())
        {
            m_MovePower = it->second;
        }
    }

    void SetMovementMode(const std::string& type)
    {
        SetCurrentResistance(type);
        SetCurrentMovePower(type);
    }

    bool IsGrounded() const { return m_IsGrounded; }
    void SetGrounded(bool grounded) { m_IsGrounded = grounded; }

    void SetUseGravity(bool gravity) { m_UseGravity = gravity; }
};

#endif //RIGIDBODY_H

