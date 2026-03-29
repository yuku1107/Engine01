/*==============================================================================

    ワイヤロープ[wire.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/09
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/Mesh/lineMesh.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/rigidbody.h"
#include "wire.h"
#include "player.h"
#include "Game/Input/gameInput.h"

void Wire::Init()
{
    m_LineMesh = new LineMesh;
    m_LineMesh->Init();

    m_Texture = Texture::Get("particle");
    m_ShaderName = "unlitTexture";
    AddComponent<Shader>()->Set(this);

    m_Tag = "WIRE";

    for (int i = 0; i < WIRE_TOTAL; i++)
    {
        m_Spring[i].position = Vector3(0.0f, 0.0f, 0.0f);
        m_Spring[i].velocity = {};
        m_Spring[i].acceleration = {};
    }

    m_Scale = { 0.2f, 1.0f, 1.0f };

    m_SaveEnable = false;
}

void Wire::UninitSelf()
{
    if (m_LineMesh) 
    { 
        m_LineMesh->Uninit(); 
        delete m_LineMesh; 
    }
}

void Wire::Set(const Vector3& start, const Vector3& end, GameObject* startObj, GameObject* endObj, float width)
{
    m_Start = start;
    m_End = start;
    m_Target = end;
    m_StartObject = startObj;
    m_EndObject = endObj;
    m_Scale.x = width;
    m_IsExtending = true;
    m_Buster = false;
    m_MaxLength = (m_Start - m_End).length() * 2.0f;
    float restLen = m_MaxLength / (WIRE_TOTAL - 1);
    Vector3 dir = (end - start).normalized();
    for (int i = 0; i < WIRE_TOTAL; i++)
    {
        m_Spring[i].position = start + dir * restLen * (float)i;
        m_Spring[i].velocity = {};
        m_Spring[i].acceleration = {};
    }
}

void Wire::UnSet()
{
    m_EndObject = nullptr;
}

void Wire::AddMaxLength(float delta)
{
    m_MaxLength = Clamp(m_MaxLength + delta, 0.0f, WIRE_MAX_LENGTH);
}

void Wire::DrawSelf()
{
    Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

    Renderer::SetDepthEnable(false);
    Renderer::SetAddBlendEnable(true);

    for (int i = 0; i < WIRE_TOTAL - 1; i++)
    {
        m_LineMesh->Draw(m_Spring[i].position, m_Spring[i + 1].position, m_Scale.x, m_MaterialColor);
    }

    Renderer::SetAddBlendEnable(false);
    Renderer::SetDepthEnable(true);
}

void Wire::UpdateSelf()
{
    if (GameInput::GetWireNextPressed())
    {
        AddMaxLength(0.1f);
    }
    if (GameInput::GetWirePrevPressed())
    {
        AddMaxLength(-0.1f);
    }

    if (m_Buster)
    {
        AddMaxLength(-1.0f);
    }

    const float dt = 1.0f / 60.0f;
    bool hasStart = (m_StartObject != nullptr);
    bool hasEnd = (m_EndObject != nullptr);

    //端点更新
    if (hasStart)
    {
        m_Start = m_StartObject->GetPosition();
        m_Spring[0].position = m_Start;
        m_Spring[0].velocity = Vector3(0, 0, 0);
    }


    if (hasEnd)
    {
        m_DrawEnable = true;

        if (m_IsExtending)
        {
            Vector3 toTarget = m_Target - m_End;
            float step = m_ExtendSpeed * dt;
            float dist = toTarget.length();
            m_End += toTarget.normalized() * std::min(step, dist);
            
            if (step >= dist)
            {
                m_IsExtending = false;
                m_MaxLength = (m_Start - m_End).length() + 1.0f;
            }
        }
        else
        {
            m_End = m_EndObject->GetPosition();
        }
        m_Spring[WIRE_TOTAL - 1].position = m_End;
        m_Spring[WIRE_TOTAL - 1].velocity = Vector3(0, 0, 0);
    }
    else
    {
        if (m_MaxLength > 0.0f)
        {
            AddMaxLength(-0.5f);
        }
        else
        {
            m_DrawEnable = false;
        }
    }

    //力計算（中間点のみ）
    for (int i = 0; i < WIRE_TOTAL; i++)
    {
        m_Spring[i].acceleration = Vector3(0, -WIRE_GRAVITY_LINKED, 0);
        m_Spring[i].acceleration += -m_Spring[i].velocity * WIRE_ATTENUATION / WIRE_MASS;
        m_Spring[i].velocity += m_Spring[i].acceleration * dt;
        m_Spring[i].position += m_Spring[i].velocity * dt;
    }


    //拘束ループ
    const int iterCount = 50;
    for (int iter = 0; iter < iterCount; iter++)
    {
        for (int i = 0; i < WIRE_TOTAL - 1; i++)
        {
            Vector3 delta = m_Spring[i + 1].position - m_Spring[i].position;
            float len = delta.length();
            if (len < 1e-6f) continue;

            Vector3 n = delta / len;
            float diff = len - (m_MaxLength / (WIRE_TOTAL + 1));
            Vector3 correction = n * diff;

            // 端点補正は Rigidbody に任せる
            if (i == 0 && hasStart)
            {
                m_Spring[i + 1].position -= correction; // Start固定
            }
            else if (i == WIRE_TOTAL - 2 && hasEnd)
            {
                m_Spring[i].position += correction;   // End固定
            }
            else
            {
                m_Spring[i].position += correction;
                m_Spring[i + 1].position -= correction;
            }
        }
    }

    //総長更新
    m_Length = 0;
    for (int i = 1; i < WIRE_TOTAL; i++)
    {
        m_Length += (m_Spring[i].position - m_Spring[i - 1].position).length();
    }
    //張力判定
    float endDist = (m_End - m_Start).length();

    // ヒステリシス幅を設けて連続反転を防ぐ
    const float linkOn = 0.995f;   // 張るしきい値（MaxLengthの99.5%）
    const float linkOff = 0.985f;  // 緩むしきい値（MaxLengthの98.5%）

    bool wasLinked = m_Linked;

    if (m_Linked)
    {
        if (endDist < m_MaxLength * linkOff)
        {
            m_Linked = false;
        }
    }
    else
    {
        if (endDist > m_MaxLength * linkOn)
        {
            m_Linked = true;
        }
    }

    //色変更
    m_MaterialColor = m_Linked ? XMFLOAT4(1, 0, 0, 1) : XMFLOAT4(0, 1, 0, 1);
}

void Wire::ApplyPositionConstraint()
{
    if (!m_StartObject || !m_EndObject) return;
    if(m_IsExtending) return;

    const float dt = 1.0f / 60.0f;         // 物理更新間隔
    const int constraintIter = 20;         // 拘束反復（多すぎるとブレやすい）
    const float eps = 1e-6f;
    const float maxImpulseSpeed = 80.0f;   // 補正による速度上限
    const float correctionFactor = 0.65f;  // 補正の割合（0.5〜0.8）
    const float velocityBlend = 0.05f;     // 速度補正ブレンド（0.03〜0.08）
    const float radialDamping = 1.0f;      // 張力方向の速度除去率
    const float minStretchForVel = 0.002f; // 伸びが少ない場合は速度注入スキップ

    Rigidbody* rbStart = m_StartObject->GetComponent<Rigidbody>();
    Rigidbody* rbEnd = m_EndObject->GetComponent<Rigidbody>();

    // 位置拘束ループ
    for (int it = 0; it < constraintIter; ++it)
    {
        Vector3 posStart = m_StartObject->GetPosition();
        Vector3 posEnd = m_EndObject->GetPosition();

        Vector3 delta = posEnd - posStart;
        float dist = delta.length();
        if (dist < eps)
        {
            break;
        }

        // 張っていない（限界長以下）なら終了
        if (dist <= m_MaxLength + 1e-5f)
        {
            break;
        }

        Vector3 dir = delta / dist;
        float stretch = dist - m_MaxLength;

        //移動対象の決定
        bool moveStart = false, moveEnd = false;
        if (!m_Buster)
        {
            if (rbStart && rbEnd)
            {
                // 質量が小さい方を優先的に動かす
                if (rbStart->GetMass() < rbEnd->GetMass())
                {
                    moveStart = true;
                }
                else
                {
                    moveEnd = true;
                }
            }
            else if (rbStart && !rbEnd) moveStart = true;
            else if (!rbStart && rbEnd) moveEnd = true;
            else
            {
                // 両方固定なら中間で分配
                Vector3 corr = dir * (stretch * 0.5f);
                m_StartObject->SetPosition(posStart + corr);
                m_EndObject->SetPosition(posEnd - corr);
                continue;
            }
        }
        else
        {
            if (m_EndObject->GetTag() == "BULLET")
            {
                moveEnd = true;
            }
            else
            {
                moveStart = true;
            }
        }


        //位置補正
        if (moveStart && rbStart)
        {
            Vector3 target = posEnd - dir * m_MaxLength;
            Vector3 appliedPos = posStart + (target - posStart) * correctionFactor;
            rbStart->SetPositionNext(appliedPos);
            m_StartObject->SetPosition(appliedPos);
        }
        else if (moveEnd && rbEnd)
        {
            Vector3 target = posStart + dir * m_MaxLength;
            Vector3 appliedPos = posEnd + (target - posEnd) * correctionFactor;
            rbEnd->SetPositionNext(appliedPos);
            m_EndObject->SetPosition(appliedPos);
        }
        else
        {
            //一方固定のみ
            if (moveStart && rbStart)
            {
                Vector3 appliedPos = posStart + dir * stretch * correctionFactor;
                rbStart->SetPositionNext(appliedPos);
                m_StartObject->SetPosition(appliedPos);
            }
            else if (moveEnd && rbEnd)
            {
                Vector3 appliedPos = posEnd - dir * stretch * correctionFactor;
                rbEnd->SetPositionNext(appliedPos);
                m_EndObject->SetPosition(appliedPos);
            }
        }
    }


    // 拘束後に速度補正
    auto applyVelocityCorrection = [&](Rigidbody* rb, GameObject* obj, GameObject* other)
    {
        if (!rb) return;

        Vector3 pos = obj->GetPosition();
        Vector3 otherPos = other->GetPosition();

        Vector3 dir = (otherPos - pos).normalized();
        float dist = (otherPos - pos).length();
        float stretch = dist - m_MaxLength;

        Vector3 vel = rb->GetVelocity();

        //径方向成分除去
        float vRad = vel.dot(dir);
        Vector3 vRadVec = dir * vRad;
        Vector3 vTan = vel - vRadVec;

        //仮想速度生成
        Vector3 desiredVel = vTan;
        if (fabsf(stretch) >= minStretchForVel)
        {
            Vector3 posVel = (obj->GetPosition() - pos) / dt;
            Vector3 posVelRadComp = dir * (posVel.dot(dir));
            Vector3 posVelTan = posVel - posVelRadComp;
            desiredVel += posVelTan;
        }

        //ブレンド＆上限
        float speed = desiredVel.length();
        if (speed > maxImpulseSpeed)
        {
            desiredVel *= (maxImpulseSpeed / speed);
        }

        Vector3 finalVel = vel * (1.0f - velocityBlend) + desiredVel * velocityBlend;

        //張力方向の速度除去
        float finalVRad = finalVel.dot(dir);
        if (finalVRad > 0.0f)
        {
            finalVel -= dir * finalVRad * radialDamping;
        }
        rb->SetVelocity(finalVel);
    };

    if (rbStart && rbEnd)
    {
        if (rbStart->GetMass() < rbEnd->GetMass())
        {
            applyVelocityCorrection(rbStart, m_StartObject, m_EndObject);
        }
        else if (rbStart->GetMass() > rbEnd->GetMass())
        {
            applyVelocityCorrection(rbEnd, m_StartObject, m_EndObject);
        }
    }
    else if (!rbEnd)
    {
        applyVelocityCorrection(rbStart, m_StartObject, m_EndObject);
    }

}
