/*==============================================================================

	íe[bullet.h]
															 Author:äsÅ@ìNâF
															 Date  :2025/06/04
==============================================================================*/
#ifndef BULLET_H
#define BULLET_H

#include "Engine/Core/gameObject.h"

class Bullet : public GameObject
{
private:
	class Shader* m_Shader;

	class AnimationModel* m_Model;

	Vector3 m_StartPosition;


public:
	void Init() override;   //èâä˙âª
	void UninitSelf() override; //èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;   //ï`âÊ

	void Shot(Vector3 start,Vector3 scale ,Vector3 velocity);

	std::string GetObjectClassName() const override { return "Bullet"; }
};

#endif //BULLET_H
