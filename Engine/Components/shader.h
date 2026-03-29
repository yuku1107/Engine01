/*==============================================================================

    シェーダー管理[shader.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/18
==============================================================================*/
#ifndef SHADER_H
#define SHADER_H

#include <unordered_map>
#include <string>
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/component.h"

class ShaderPool
{
public:
    struct ShaderData
    {
        ID3D11VertexShader* vertexShader = nullptr;
        ID3D11PixelShader* pixelShader = nullptr;
        ID3D11InputLayout* inputLayout = nullptr;
    };

private:
    static std::unordered_map<std::string, ShaderData> m_ShaderPool;
public:
    static void Init();

    // 頂点＋ピクセルシェーダーをまとめてロード
    static ShaderData* Load(const std::string& name, const char* vsFile, const char* psFile);

    // 名前で取得（存在しない場合 nullptr）
    static ShaderData* Get(const std::string& name);

    // 全解放
    static void Uninit();

    static const std::unordered_map<std::string, ShaderData>& GetAll()
    {
        return m_ShaderPool;
    }
};

class Shader:public Component
{
private:
    ShaderPool::ShaderData* m_Data = nullptr;
    std::string m_CurrentShaderName;

public:
    Shader() = default;
    void Uninit() override
    {
        m_Data = nullptr;
        m_CurrentShaderName.clear();
    }
    void Update() override;
    void Draw() override;

    // ShaderPool から ShaderData をセット
    void Load(const std::string& name)
    {
        m_Data = ShaderPool::Get(name);
        assert(m_Data);
        m_CurrentShaderName = name;
    }

    void Set(GameObject* obj);

    void Set(GameObject* obj, const std::string& name)
    {
        m_pOwner = obj;
        m_Data = ShaderPool::Get(name);
        assert(m_Data);
        m_CurrentShaderName = name;
    }

    void SetShader()
    {
        if (!m_Data)
        {
            return;
        }

        auto ctx = Renderer::GetDeviceContext();
        ctx->IASetInputLayout(m_Data->inputLayout);
        ctx->VSSetShader(m_Data->vertexShader, nullptr, 0);
        ctx->PSSetShader(m_Data->pixelShader, nullptr, 0);
    }

};

#endif //SHADER_H
