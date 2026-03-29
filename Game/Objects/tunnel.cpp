/*==============================================================================

    トンネル[tunnel.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/12/17
==============================================================================*/
#include "main.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/animationModel.h"
#include "tunnel.h"

void Tunnel::Init()
{

    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\tunnel.fbx");

    m_Model->LoadAnimation("Assets\\model\\tunnel_Up.fbx", "Up");
    m_Model->LoadAnimation("Assets\\model\\tunnel_Down.fbx", "Down");
    m_Model->LoadAnimation("Assets\\model\\tunnel_Right.fbx", "Right");
    m_Model->LoadAnimation("Assets\\model\\tunnel_Left.fbx", "Left");
    m_Model->LoadAnimation("Assets\\model\\tunnel_Forward.fbx", "Forward");
    m_AnimationName = "Forward";
    m_AnimationNameNext = "Forward";
    m_AnimationBlend = 0.0f;
    m_Frame = 0;

    m_Scale = { 50.0f,50.0f,50.0f };

    m_ShaderName = "tunnel";
    AddComponent<Shader>()->Set(this);

    {
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vector3);
        bd.StructureByteStride = sizeof(Vector3);
        bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA sd{};
        Vector3 tmie = Vector3(m_Time, 0.0f, 0.0f);
        sd.pSysMem = &tmie;

        Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_TimeBuffer);

        //シェーダーリソースビュー作成
        D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
        srvd.Format = DXGI_FORMAT_UNKNOWN;
        srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvd.Buffer.FirstElement = 0;
        srvd.Buffer.NumElements = 1;

        Renderer::GetDevice()->CreateShaderResourceView(m_TimeBuffer, &srvd, &m_TimeSRV);
    }

    m_Tag = "TUNNEL";
}

void Tunnel::UninitSelf()
{
	m_Model = nullptr; 
}

void Tunnel::UpdateSelf()
{
    m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, m_AnimationBlend, false);
    m_Parameter.x += 0.1f;

    m_Time += 0.01f;
    Vector3 timeData(m_Time, 0.0f, 0.0f);
    Renderer::GetDeviceContext()->UpdateSubresource(m_TimeBuffer, 0, nullptr, &timeData, 0, 0);

    int count = m_Frame % 45;
    
    if (count == 0)
    {
        int num = rand() % 5;

        switch (num)
        {
        case 0:
            AnimationBlend("Up");
            break;
        case 1:
            AnimationBlend("Left");
            break;
        case 2:
            AnimationBlend("Right");
            break;
        case 3:
            AnimationBlend("Down");
            break;
        case 4:
            AnimationBlend("Forward");
            break;
        }
    }

    m_Frame++;
    m_AnimationBlend += 0.1f;
    if (m_AnimationBlend > 1.0f)
    {
        m_AnimationBlend = 1.0f;
    }
}

void Tunnel::DrawSelf()
{
    Renderer::SetParameter(m_Parameter);

    //マトリクス設定
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_TimeSRV);

    Renderer::SetWorldMatrix(world);

    m_Model->Draw();
}

void Tunnel::AnimationBlend(std::string AnimationName)
{
    if (m_AnimationNameNext != AnimationName)
    {
        m_AnimationName = m_AnimationNameNext;
        m_AnimationNameNext = AnimationName;
        m_AnimationBlend = 0.0f;
        //m_Frame = 0.0f;
    }
}