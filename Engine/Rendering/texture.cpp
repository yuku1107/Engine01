/*==============================================================================

    テクスチャー管理[texture.cpp]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/14
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Rendering/texture.h"

std::unordered_map<std::string, ID3D11ShaderResourceView*> Texture::m_TexturePool;

void Texture::Init()
{
    Texture::Load("kk_Enter", "Assets\\texture\\kk_ENTER.png");
    Texture::Load("kk_Space", "Assets\\texture\\kk_SPACE.png");
    Texture::Load("slash", "Assets\\texture\\slash.png");
    Texture::Load("colon", "Assets\\texture\\colon.png");
    Texture::Load("particle", "Assets\\texture\\particle.png");
    Texture::Load("wasd_move", "Assets\\texture\\UI_wasd_move.png");
    Texture::Load("jump", "Assets\\texture\\UI_jump.png");
    Texture::Load("wire_set", "Assets\\texture\\UI_wire.png");
    Texture::Load("wire_anchor", "Assets\\texture\\UI_wire_anchor.png");
    Texture::Load("number", "Assets\\texture\\number.png");
    Texture::Load("sura", "Assets\\texture\\sura.jpg");
    Texture::Load("water", "Assets\\texture\\water.jpg");
    Texture::Load("coordinate_x", "Assets\\texture\\co_x.png");
    Texture::Load("coordinate_y", "Assets\\texture\\co_y.png");
    Texture::Load("sky", "Assets\\texture\\sky.jpg");
    Texture::Load("sky2", "Assets\\texture\\sky2.jpg");
    Texture::Load("cursor", "Assets\\texture\\cursor.png");
    Texture::Load("reticle", "Assets\\texture\\reticle.png");
    Texture::Load("black", "Assets\\texture\\black.png");
    Texture::Load("white", "Assets\\texture\\white.png");
    Texture::Load("tree", "Assets\\texture\\tree1.png");
    Texture::Load("title", "Assets\\texture\\title.png");
    Texture::Load("result", "Assets\\texture\\result.png");
    Texture::Load("swirl", "Assets\\texture\\swirl.png");
    Texture::Load("start_B", "Assets\\texture\\start_B.png");
    Texture::Load("start_W", "Assets\\texture\\start_W.png");
    Texture::Load("retry_B", "Assets\\texture\\retry_B.png");
    Texture::Load("retry_W", "Assets\\texture\\retry_W.png");
    Texture::Load("continue_B", "Assets\\texture\\continue_B.png");
    Texture::Load("continue_W", "Assets\\texture\\continue_W.png");
    Texture::Load("end_B", "Assets\\texture\\end_B.png");
    Texture::Load("end_W", "Assets\\texture\\end_W.png");
    Texture::Load("clear", "Assets\\texture\\clear.png");
    Texture::Load("discovery0", "Assets\\texture\\discovery0.png");
    Texture::Load("discovery1", "Assets\\texture\\discovery1.png");
    Texture::Load("discovery2", "Assets\\texture\\discovery2.png");
    Texture::Load("discovery3", "Assets\\texture\\discovery3.png");
    Texture::Load("discovery4", "Assets\\texture\\discovery4.png");
    Texture::Load("discovery5", "Assets\\texture\\discovery5.png");
    Texture::Load("discovery6", "Assets\\texture\\discovery6.png");
    Texture::Load("discovery7", "Assets\\texture\\discovery7.png");
    Texture::Load("discovery8", "Assets\\texture\\discovery8.png");

    Texture::Load("title_gost", "Assets\\texture\\title_gost.png");
    Texture::Load("over_gost", "Assets\\texture\\over_gost.png");
    Texture::Load("result_gost", "Assets\\texture\\result_gost.png");

    Texture::LoadDDS("field", "Assets\\texture\\field.dds");
    Texture::LoadDDS("soil", "Assets\\texture\\soil.dds");
    Texture::LoadDDS("grass", "Assets\\texture\\grass1.dds");
}

ID3D11ShaderResourceView* Texture::Load(const std::string& name, const char* FileName)
{
    auto it = m_TexturePool.find(name);

    if (it != m_TexturePool.end())
    {
        return it->second;
    }

    wchar_t wFileName[512];
    mbstowcs(wFileName, FileName, strlen(FileName) + 1);

    //テクスチャ読込
    TexMetadata metadata;
    ScratchImage image;
    ScratchImage mipChain;
    ID3D11ShaderResourceView* texture;

    //画像を読み込み
    LoadFromWICFile(wFileName, WIC_FLAGS_NONE, &metadata, image);

    //ミップマップ生成
    GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);

    //テクスチャ生成
    CreateShaderResourceView(Renderer::GetDevice(), mipChain.GetImages(), mipChain.GetImageCount(), mipChain.GetMetadata(), &texture);

    //テクスチャが読み込まない時、エラーの場合
    assert(texture);

    m_TexturePool[name] = texture;

    return texture;
}

ID3D11ShaderResourceView* Texture::LoadDDS(const std::string& name, const char* FileName)
{
    auto it = m_TexturePool.find(name);

    if (it != m_TexturePool.end())
    {
        return it->second;
    }

    wchar_t wFileName[512];
    mbstowcs(wFileName, FileName, strlen(FileName) + 1);

    //テクスチャ読込
    TexMetadata metadata;
    ScratchImage image;
    ID3D11ShaderResourceView* texture;

    //画像を読み込み
    LoadFromDDSFile(wFileName, DDS_FLAGS_NONE, &metadata, image);

    //テクスチャ生成
    CreateShaderResourceView(Renderer::GetDevice(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &texture);

    //テクスチャが読み込まない時、エラーの場合
    assert(texture);

    m_TexturePool[name] = texture;

    return texture;
}

ID3D11ShaderResourceView* Texture::Get(const std::string& name)
{
    auto it = m_TexturePool.find(name);

    if (it != m_TexturePool.end())
    {
        return it->second;
    }

    return nullptr;
}

void Texture::Uninit()
{
    for (auto texture : m_TexturePool)
    {
        if (texture.second)
        {
            texture.second->Release();
        }
    }

    m_TexturePool.clear();
}