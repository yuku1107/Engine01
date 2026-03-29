/*==============================================================================

    テクスチャー管理[texture.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/06/18
==============================================================================*/
#ifndef TEXTURE_H
#define TEXTURE_H

#include <unordered_map>
#include <string>

class Texture
{
private:
    static std::unordered_map<std::string, ID3D11ShaderResourceView*> m_TexturePool;

public:
    static void Init();

    static ID3D11ShaderResourceView* Load(const std::string& name, const char* FileName);
    static ID3D11ShaderResourceView* LoadDDS(const std::string& name, const char* FileName);

    static ID3D11ShaderResourceView* Get(const std::string& name);

    static void Uninit();

    static const std::unordered_map<std::string, ID3D11ShaderResourceView*>& GetAll()
    {
        return m_TexturePool;
    }
};


#endif //TEXTURE_H
