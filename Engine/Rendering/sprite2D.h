/*==============================================================================

	2Dï`âÊ[sprite2D.h]
															 Author:äsÅ@ìNâF
															 Date  :2026/02/13
==============================================================================*/
#ifndef SPRITE2D_H
#define SPRITE2D_H

#include "Engine/Rendering/Mesh/mesh2D.h"

class Sprite2D
{
private:
	static Mesh2D m_Mesh2D;

public:
    static void Init();
    static void Uninit();
	static void Draw(Vector3 position, Vector3 size, Vector3 rotation, ID3D11ShaderResourceView* tex, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	static void Draw(Vector3 position, Vector3 size, Vector3 rotation, ID3D11ShaderResourceView* tex, XMFLOAT4 color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), const int frame = 0, const int x = 1, const int y = 1);
};

#endif //SPRITE2D_H
