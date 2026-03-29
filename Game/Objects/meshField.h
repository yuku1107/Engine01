#pragma once

#include "Engine/Core/gameObject.h"

class MeshField : public GameObject
{

private:
	ID3D11Buffer*				m_VertexBuffer{};
	ID3D11Buffer*				m_IndexBuffer{};
	ID3D11ShaderResourceView*	m_Texture{};
	ID3D11ShaderResourceView*   m_TextureSoil{};

	VERTEX_3D					m_Vertex[21][21]{};
	int m_type = 1;
public:
	void Init() override;
	void Init(int type);
	void UninitSelf() override;
	void UpdateSelf() override;
	void DrawSelf() override;

	const VERTEX_3D(*GetVertices() const)[21] { return m_Vertex; }

	float GetHeight(Vector3 Position);

	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "MeshField"; }
};