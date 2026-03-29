/*==============================================================================

    –Ø[tree.h]
                                                             Author:Šs@“N‰F
                                                             Date  :2025/06/18
==============================================================================*/
#ifndef TREE_H
#define TREE_H

#include "Engine/Core/gameObject.h"
#include "Engine/Rendering/structuredBuffer.h"

#define TREE_MAX (100)

class Tree : public GameObject
{
private:
	StructuredBuffer<Vector3> m_PositionBuffer;
	StructuredBuffer<Vector3> m_ScaleBuffer;
	StructuredBuffer<Vector3> m_TimeBuffer;
	float m_Time;

	class QuadMesh* m_QuadMesh;

	ID3D11ShaderResourceView* m_Texture;

public:
	void Init() override;   //‰Šú‰»
	void UninitSelf() override; //I—¹
	void UpdateSelf() override; //XV
	void DrawSelf() override;   //•`‰æ

	std::string GetObjectClassName() const override { return "Tree"; }

};

#endif //TREE_H
