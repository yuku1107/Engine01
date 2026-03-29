/*==============================================================================

    アンカー[anchor.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/07/14
==============================================================================*/
#ifndef ANCHOR_H
#define ANCHOR_H

#include "Engine/Core/gameObject.h"

class Anchor :public GameObject
{
private:
    class AnimationModel* m_Model;
	Vector3 m_StartPosition;
	Vector3 m_EndPosition;

	bool m_GoEnd = true;
	float m_StopDistance = 0.2f;

	bool m_WasCreaterMode = false;
public:
	void Init() override;   //初期化
	void UninitSelf() override; //終了
	void UpdateSelf() override; //更新
	void DrawSelf() override;   //描画
	void DrawInspector() override;

	const Vector3 GetStartPosition()const { return m_StartPosition; }
	void SetStartPosition(Vector3 startPosition) { m_StartPosition = startPosition; }
	
	const Vector3 GetEndPosition()const { return m_EndPosition; }
	void SetEndPosition(Vector3 endPosition) { m_EndPosition = endPosition; }

	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "Anchor"; }
};

#endif //ANCHOR_H
