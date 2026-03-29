/*==============================================================================

	GPU構造化バッファ[StructuredBuffer.h]
                                                             Author:郭　哲宇
                                                             Date  :
==============================================================================*/
#ifndef STRUCTUREDBUFFER_H
#define STRUCTUREDBUFFER_H

#include "Engine/Rendering/Renderer/renderer.h"

template<class T>
class StructuredBuffer
{
private:

    ID3D11Buffer* m_Buffer = nullptr;
    ID3D11ShaderResourceView* m_SRV = nullptr;
    UINT m_Count = 0;

public:

    StructuredBuffer() = default;
    ~StructuredBuffer()
    {
		Release();
    }

    void Create(UINT count, T* initData = nullptr)
    {
        m_Count = count;

        D3D11_BUFFER_DESC bd{};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(T) * count;
        bd.StructureByteStride = sizeof(T);
        bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bd.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA sd{};
        sd.pSysMem = initData;

        Renderer::GetDevice()->CreateBuffer(&bd, initData ? &sd : nullptr, &m_Buffer);

        D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
        srvd.Format = DXGI_FORMAT_UNKNOWN;
        srvd.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        srvd.Buffer.FirstElement = 0;
        srvd.Buffer.NumElements = count;

        Renderer::GetDevice()->CreateShaderResourceView(m_Buffer, &srvd, &m_SRV);
    }

    void Release()
    {
        if (m_SRV)
        {
            m_SRV->Release();
            m_SRV = nullptr;
        }
        if (m_Buffer)
        {
            m_Buffer->Release();
            m_Buffer = nullptr;
        }
    }

    void Update(const T* data, UINT count)
    {
        if (count > m_Count)
            count = m_Count;

        D3D11_MAPPED_SUBRESOURCE msr;

        Renderer::GetDeviceContext()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

        memcpy(msr.pData, data, sizeof(T) * count);

        Renderer::GetDeviceContext()->Unmap(m_Buffer, 0);
    }

    void BindVS(UINT slot)
    {
        Renderer::GetDeviceContext()->VSSetShaderResources(slot, 1, &m_SRV);
    }

    ID3D11ShaderResourceView* GetSRV() const
    {
        return m_SRV;
    }

    ID3D11Buffer* GetBuffer() const
    {
        return m_Buffer;
    }

    UINT GetCount() const
    {
        return m_Count;
    }
};

#endif //STRUCTUREDBUFFER_H

