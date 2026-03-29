/*==============================================================================

    シェーダー管理[shader.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/18
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/gameObject.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/shader.h"

std::unordered_map<std::string, ShaderPool::ShaderData> ShaderPool::m_ShaderPool;

ShaderPool::ShaderData* ShaderPool::Load(const std::string& name, const char* vsFile, const char* psFile)
{
    auto it = m_ShaderPool.find(name);

    if (it != m_ShaderPool.end())
    {
        return &it->second;
    }

    ShaderData shader;

    // 読み込み
    Renderer::CreateVertexShader(&shader.vertexShader, &shader.inputLayout, vsFile);
    Renderer::CreatePixelShader(&shader.pixelShader, psFile);

    assert(shader.vertexShader && shader.pixelShader && shader.inputLayout);

    m_ShaderPool[name] = shader;

    return &m_ShaderPool[name];
}

ShaderPool::ShaderData* ShaderPool::Get(const std::string& name)
{
    auto it = m_ShaderPool.find(name);
    if (it != m_ShaderPool.end())
    {
        return &it->second;
    }
    return nullptr;
}

void ShaderPool::Init()
{
    ShaderPool::Load("unlitTexture", "Assets\\shaders\\unlitTextureVS.cso", "Assets\\shaders\\unlitTexturePS.cso");
    ShaderPool::Load("grayscale", "Assets\\shaders\\grayscaleVS.cso", "Assets\\shaders\\grayscalePS.cso");
    ShaderPool::Load("vertexLighting", "Assets\\shaders\\vertexLightingVS.cso", "Assets\\shaders\\vertexLightingPS.cso");
    ShaderPool::Load("pixelLighting", "Assets\\shaders\\pixelLightingVS.cso", "Assets\\shaders\\pixelLightingPS.cso");
    ShaderPool::Load("pixelLightingBlinnPhong", "Assets\\shaders\\pixelLightingBlinnPhongVS.cso", "Assets\\shaders\\pixelLightingBlinnPhongPS.cso");
    ShaderPool::Load("hemisphereLighting", "Assets\\shaders\\hemisphereLightingVS.cso", "Assets\\shaders\\hemisphereLightingPS.cso");
    ShaderPool::Load("pointLightingBlinnPhong", "Assets\\shaders\\pointLightingBlinnPhongVS.cso", "Assets\\shaders\\pointLightingBlinnPhongPS.cso");
    ShaderPool::Load("spotLighting", "Assets\\shaders\\spotLightingVS.cso", "Assets\\shaders\\spotLightingPS.cso");
    ShaderPool::Load("limLighting", "Assets\\shaders\\limLightingVS.cso", "Assets\\shaders\\limLightingPS.cso");
    ShaderPool::Load("bump", "Assets\\shaders\\bumpVS.cso", "Assets\\shaders\\bumpPS.cso");
    ShaderPool::Load("CookTorrance", "Assets\\shaders\\CookTorranceVS.cso", "Assets\\shaders\\CookTorrancePS.cso");
    ShaderPool::Load("scene", "Assets\\shaders\\sceneVS.cso", "Assets\\shaders\\scenePS.cso");
    ShaderPool::Load("field", "Assets\\shaders\\fieldVS.cso", "Assets\\shaders\\fieldPS.cso");
    ShaderPool::Load("wave", "Assets\\shaders\\waveVS.cso", "Assets\\shaders\\wavePS.cso");
    ShaderPool::Load("depthShadow", "Assets\\shaders\\depthShadowVS.cso", "Assets\\shaders\\depthShadowPS.cso");
    ShaderPool::Load("waveRef", "Assets\\shaders\\waveRefVS.cso", "Assets\\shaders\\waveRefPS.cso");
    ShaderPool::Load("grass", "Assets\\shaders\\grassVS.cso", "Assets\\shaders\\grassPS.cso");
    ShaderPool::Load("tree", "Assets\\shaders\\treeVS.cso", "Assets\\shaders\\treePS.cso");
    ShaderPool::Load("swirl", "Assets\\shaders\\swirlVS.cso", "Assets\\shaders\\swirlPS.cso");
    ShaderPool::Load("tunnel", "Assets\\shaders\\tunnelVS.cso", "Assets\\shaders\\tunnelPS.cso");
    ShaderPool::Load("unlitTextureAnimation", "Assets\\shaders\\unlitTextureAnimationVS.cso", "Assets\\shaders\\unlitTextureAnimationPS.cso");
    ShaderPool::Load("particle", "Assets\\shaders\\particleVS.cso", "Assets\\shaders\\particlePS.cso");
}

void ShaderPool::Uninit()
{
    for (auto& shader : m_ShaderPool)
    {
        if (shader.second.vertexShader)
        {
            shader.second.vertexShader->Release();
        }
        if (shader.second.pixelShader)
        {
            shader.second.pixelShader->Release();
        }
        if (shader.second.inputLayout)
        {
            shader.second.inputLayout->Release();
        }
    }

    m_ShaderPool.clear();
}

void Shader::Update()
{
    if (!m_pOwner)
    {
        return;
    }

    const std::string& currentName = m_pOwner->GetShaderName();

    if (!m_Data || currentName != m_CurrentShaderName)
    {
        m_Data = ShaderPool::Get(currentName);

        if (m_Data)
        {
            m_CurrentShaderName = currentName;
        }
        else
        {
            // 存在しない場合はログ出力（または安全な fallback）
            OutputDebugStringA(("Shader not found: " + currentName + "\n").c_str());
        }
    }
}

void Shader::Draw()
{
    if (!m_Data)
    {
        return;
    }

    auto ctx = Renderer::GetDeviceContext();
    ctx->IASetInputLayout(m_Data->inputLayout);
    ctx->VSSetShader(m_Data->vertexShader, nullptr, 0);
    ctx->PSSetShader(m_Data->pixelShader, nullptr, 0);

    if (m_pOwner->GetShaderName() == "depthShadow")
    {
        ID3D11ShaderResourceView* tex = Renderer::GetDepthStencil();
        Renderer::GetDeviceContext()->PSSetShaderResources(1, 1, &tex);//1番シャドウマップ
    }
}

void Shader::Set(GameObject* obj)
{
    m_pOwner = obj;
    m_Data = ShaderPool::Get(m_pOwner->GetShaderName());
    assert(m_Data);
    m_CurrentShaderName = m_pOwner->GetShaderName();
}
