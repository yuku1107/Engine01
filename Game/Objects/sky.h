/*==============================================================================

	“V‹ó[sky.h]
															 Author:Šs@“N‰F
															 Date  :2025/06/11
==============================================================================*/
#ifndef SKY_H
#define SKY_H

#include "Engine/Core/gameObject.h"

class Sky : public GameObject
{
private:
	class AnimationModel* m_Model;
	int m_Type;
	int m_TypeNext;
public:
	void Init() override;   //‰Šú‰»
	void UninitSelf() override; //I—¹
	void UpdateSelf() override; //XV
	void DrawSelf() override;   //•`‰æ
	void DrawInspector() override;

	int GetType() { return m_Type; }
	void SetTypeNext(int type) { m_TypeNext = type; }

	void Serialize(Serializer& s) override;
	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "Sky"; }
};


#endif //SKY_H
