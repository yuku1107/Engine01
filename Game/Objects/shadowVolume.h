/*==============================================================================

	シャドウ[shadowVolume.h]
															 Author:郭　哲宇
															 Date  :2025/11/14
==============================================================================*/
#ifndef SHADOWVOLUME_H
#define SHADOWVOLUME_H

#include "Engine/Core/gameObject.h"

class ShadowVolume : public GameObject
{
private:
	class AnimationModel* m_Model;
	ID3D11ShaderResourceView* m_Texture;
	std::string m_TextureName;
	class Mesh2D* m_Mesh2D;

public:
	void Init() override;   //初期化
	void UninitSelf() override; //終了
	void UpdateSelf() override; //更新
	void DrawSelf() override;   //描画

	std::string GetObjectClassName() const override { return "ShadowVolume"; }
};


#endif //SHADOWVOLUME_H
