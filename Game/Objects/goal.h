/*==============================================================================

	ƒS[ƒ‹[goal.h]
															 Author:Šs@“N‰F
															 Date  :2025/09/13
==============================================================================*/
#ifndef GOAL_H
#define GOAL_H

#include "Engine/Core/gameObject.h"

class Goal : public GameObject
{
private:
	int m_Frame;

	class AnimationModel* m_Model;

public:
	void Init() override;   //‰Šú‰»
	void UninitSelf() override; //I—¹
	void UpdateSelf() override; //XV
	void DrawSelf() override;   //•`‰æ


	std::string GetObjectClassName() const override { return "Goal"; }

};

#endif //GOAL_H
