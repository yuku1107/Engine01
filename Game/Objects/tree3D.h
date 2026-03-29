/*==============================================================================

	–Ø_3D[tree3D.h]
															 Author:Šs@“N‰F
															 Date  :2025/12/11
==============================================================================*/
#ifndef TREE3D_H
#define TREE3D_H

#include "Engine/Core/gameObject.h"

class Tree3D :public GameObject
{
private:
	class AnimationModel* m_Model;

public:
	void Init() override;   //‰Šú‰»
	void UninitSelf() override; //I—¹
	void UpdateSelf() override; //XV
	void DrawSelf() override;   //•`‰æ

	std::string GetObjectClassName() const override { return "Tree3D"; }
};

#endif //TREE3D_H