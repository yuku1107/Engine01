/*==============================================================================

	ínñ _à⁄ìÆ[groundMove.h]
															 Author:äsÅ@ìNâF
															 Date  :2025/12/18
==============================================================================*/
#ifndef GROUNDMOVE_H
#define GROUNDMOVE_H

#include "Engine/Core/gameObject.h"

class GroundMove : public GameObject
{
private:
	class AnimationModel* m_Model;
	Vector3 m_StartPosition;
	Vector3 m_EndPosition;
	float m_Frame;
	float m_Speed;
	bool m_GoEnd = true;

	bool m_WasCreaterMode = false;
public:
	void Init() override;   //èâä˙âª
	void Init(Vector3 start, Vector3 End);
	void UninitSelf() override; //èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;   //ï`âÊ
	void DrawInspector() override;

	Vector3 GetStartPosition() { return m_StartPosition; }
	void SetStartPosition(Vector3 start) { m_StartPosition = start; }
	Vector3 GetEndPosition() { return m_EndPosition; }
	void SetEndPosition(Vector3 end) { m_EndPosition = end; }
	float GetSpeed() { return m_Speed; }
	void SetSpeed(float speed) { m_Speed = speed; }

	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "GroundMove"; }
};

#endif //GROUNDMOVE_H
