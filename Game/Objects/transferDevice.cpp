/*==============================================================================

	転送装置[transferDevice.cpp]
															 Author:郭　哲宇
															 Date  :2025/09/13
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Components/animationModel.h"
#include "Engine/Rendering/Mesh/quadMesh.h"
#include "Engine/Rendering/texture.h"
#include "Engine/Components/shader.h"
#include "Engine/Serialization/serializer.h"
#include "ThirdParty/ImGui/imgui.h"

#include "transferDevice.h"
#include "player.h"

void TransferDevice::Init()
{

    m_Model = AddComponent<AnimationModel>();
    m_Model->Load("Assets\\model\\gate.fbx");
    m_Model->LoadAnimation("Assets\\model\\gate_Assemble.fbx", "Assemble");
    m_Model->LoadAnimation("Assets\\model\\gate_Disassemble.fbx", "Disassemble");
    m_AnimationName = "Disassemble";
    m_AnimationNameNext = "Disassemble";
    m_Frame = 0;

    m_QuadMesh = new QuadMesh();
    m_QuadMesh->Init();

    m_Texture = Texture::Get("swirl");

    m_MaterialColor = { 1.0f,1.0f,1.0f,1.0f };

    AddComponent<Box>()->Set(this);

    m_ShaderName = "pointLightingBlinnPhong";
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

    m_Tag = "TRANSFERDEVICE";
}

void TransferDevice::UninitSelf()
{
    m_Model = nullptr;

    m_QuadMesh->Uninit();
    delete m_QuadMesh;

    m_TimeBuffer->Release();
    m_TimeSRV->Release();
}

void TransferDevice::UpdateSelf()
{
    GetComponent<Box>()->Update(m_Position + Vector3(0.0f, 1.5f, 0.0f), m_Scale * Vector3(2.0f, 2.5f, 1.0f), m_Rotation);

    switch (m_Type)
    {
    case Type_ToStage1:
        m_MaterialColor = { 0.0f,1.0f,0.0f,1.0f };
        break;
    case Type_ToStage2:
        m_MaterialColor = { 0.0f,0.0f,1.0f,1.0f };
        break;
    case Type_ToStage3:
        m_MaterialColor = { 0.0f,1.0f,1.0f,1.0f };
        break;
    case Type_ToResult:
        m_MaterialColor = { 1.0f,0.0f,0.0f,1.0f };
        break;
    }

    if (m_CanUse)
    {
        if (!m_DrawEnable)
        {
            m_DrawEnable = true;
        }

        Player* player = Manager::GetScene()->GetGameObject<Player>();
        Vector3 playerPos = player->GetPosition();
        float distance = (playerPos - m_Position).length();

        if (distance <= 10)
        {
            m_QuadMeshEnable = !m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, 1.0f, false);
            AnimationBlend("Assemble");
        }
        else
        {
            m_Model->Update(m_AnimationName.c_str(), m_Frame, m_AnimationNameNext.c_str(), m_Frame, 1.0f, false);
            m_QuadMeshEnable = false;
            AnimationBlend("Disassemble");
        }

        m_Frame++;

        //シェーダー用時間経過
        m_Time += 0.01f;
        Vector3 timeData(m_Time, 0.0f, 0.0f);
        Renderer::GetDeviceContext()->UpdateSubresource(m_TimeBuffer, 0, nullptr, &timeData, 0, 0);
    }
    else
    {
        m_DrawEnable = false;
    }
}

void TransferDevice::DrawSelf()
{
    auto* shader = GetComponent<Shader>();
    m_ShaderName = "pointLightingBlinnPhong";
    shader->Update();
    shader->Draw();

    //マテリア設定
    MATERIAL material{};
    material.Diffuse = m_MaterialColor;
    material.TextureEnable = true;
    Renderer::SetMaterial(material);

    //マトリクス設定
    XMMATRIX world, scale, rotation, trancelation;

    scale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
    rotation = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
    trancelation = XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);

    world = scale * rotation * trancelation;

    Renderer::SetWorldMatrix(world);

    m_Model->Draw(m_MaterialColor);

    if (m_QuadMeshEnable)
    {
        m_ShaderName = "swirl";
        shader->Update();
        shader->Draw();

        //シェーダー設定
        Renderer::GetDeviceContext()->VSSetShaderResources(2, 1, &m_TimeSRV);
        Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);
        m_QuadMesh->Draw(m_Position , m_Rotation, m_Scale * Vector3(2.2f, 3.2f, 1.0f), false);
    }
}

void TransferDevice::DrawInspector()
{
    int currentIndex = static_cast<int>(m_Type);

    ImGui::Text("Transfer Type:");

    ImGui::RadioButton("None", &currentIndex, 0);
    ImGui::RadioButton("ToStage1", &currentIndex, 1);
    ImGui::RadioButton("ToStage2", &currentIndex, 2);
    ImGui::RadioButton("ToStage3", &currentIndex, 3);
    ImGui::RadioButton("ToResult", &currentIndex, 4);

    m_Type = static_cast<TransferDevice::TransferType>(currentIndex);

    ImGui::Checkbox("Can Use", &m_CanUse);
}

void TransferDevice::AnimationBlend(std::string AnimationName)
{
    if (m_AnimationNameNext != AnimationName)
    {
        m_AnimationName = m_AnimationNameNext;
        m_AnimationNameNext = AnimationName;
        m_Frame = 0;
    }
}

void TransferDevice::Serialize(Serializer& s)
{
    GameObject::Serialize(s);

    s.WriteInt("type", static_cast<int>(m_Type));
}

void TransferDevice::Deserialize(Serializer& s)
{
    GameObject::Deserialize(s);

    m_Type = static_cast<TransferType>(s.ReadInt("type"));
}
