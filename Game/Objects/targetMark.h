/*==============================================================================

    ターゲットマーク[TargetMark.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/09/08
==============================================================================*/
#ifndef TARGETMARK_H
#define TARGETMARK_H

#include "Engine/Core/gameObject.h"

class TargetMark :public GameObject
{
private:
	class AnimationModel* m_Model;
	GameObject* m_Target = nullptr;

	float m_Interpolation;
	float m_Count;
public:
	void Init() override;   //初期化
	void UninitSelf() override; //終了
	void UpdateSelf() override; //更新
	void DrawSelf() override;   //描画

	void SetTarget(GameObject* obj) { m_Target = obj; }
	GameObject* GetTarget() { return m_Target; }

	std::string GetObjectClassName() const override { return "TargetMark"; }
};


#endif //TARGETMARK_H