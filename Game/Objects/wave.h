/*==============================================================================

    åŒ[wave.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :
==============================================================================*/
#ifndef WAVA_H
#define WAVA_H

#include "Engine/Core/gameObject.h"

class Wave : public GameObject
{

private:

	ID3D11Buffer* m_VertexBuffer{};
	ID3D11Buffer* m_IndexBuffer{};

	ID3D11ShaderResourceView* m_Texture{};

	VERTEX_3D m_Vertex[21][21];

	float m_Time{};

	float m_WaveSpeed = -0.1f;
	float m_WaveLength = 0.5f;
	float m_Amplitude = 0.1f;

public:
	void Init() override;
	void UninitSelf() override;
	void UpdateSelf() override;
	void DrawSelf() override;

	const VERTEX_3D(*GetVertices() const)[21] { return m_Vertex; }

	std::string GetObjectClassName() const override { return "Wave"; }
};

#endif //WAVA_H