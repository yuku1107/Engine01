/*==============================================================================

	ƒgƒ“ƒlƒ‹[tunnel.h]
															 Author:Šs@“N‰F
															 Date  :2025/12/17
==============================================================================*/
#ifndef TUNNEL_H
#define TUNNEL_H

#include "Engine/Core/gameObject.h"
#include "Engine/Rendering/Mesh/mesh2D.h"

class Tunnel : public GameObject
{
private:
	class AnimationModel* m_Model;
	std::string m_AnimationName;
	std::string m_AnimationNameNext;
	float m_AnimationBlend;
	unsigned int m_Frame;
	bool m_IsAnimationLoop = true;

	ID3D11ShaderResourceView* m_Texture;

	ID3D11Buffer* m_TimeBuffer;
	ID3D11ShaderResourceView* m_TimeSRV;
	float m_Time;

	XMFLOAT4 m_Parameter = { 0.0f,0.0f,0.0f,0.0f };
public:
	void Init() override;   //‰Šú‰»
	void UninitSelf() override; //I—¹
	void UpdateSelf() override; //XV
	void DrawSelf() override;   //•`‰æ

	void AnimationBlend(std::string AnimationName);

	std::string GetObjectClassName() const override { return "Tunnel"; }

};

#endif //TUNNEL_H
