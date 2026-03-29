/*==============================================================================

	’n–Ê[ground.h]
															 Author:Šs@“N‰F
															 Date  :2025/09/11
==============================================================================*/
#ifndef GROUND_H
#define GROUND_H

#include "Engine/Core/gameObject.h"

class Ground :public GameObject
{
private:
	class AnimationModel* m_Model;

public:
	void Init() override;   //‰Šú‰»
	void UninitSelf() override; //I—¹
	void UpdateSelf() override; //XV
	void DrawSelf() override;   //•`‰æ


	std::string GetObjectClassName() const override { return "Ground"; }

};


#endif //GROUND_H
