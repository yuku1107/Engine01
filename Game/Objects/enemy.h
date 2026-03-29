/*==============================================================================

	ìG[enemy.h]
															 Author:äsÅ@ìNâF
															 Date  :2025/06/04
==============================================================================*/
#ifndef ENEMY_H
#define ENEMY_H

#include "Engine/Core/gameObject.h"

class Enemy;

class EnemyState
{
public:
	virtual ~EnemyState(){}
	virtual void Enter(Enemy* enemy) = 0;
	virtual void Update(Enemy* enemy, float deltatime) = 0;
	virtual void Exit(Enemy* enemy) = 0;
};

class Enemy : public GameObject
{
private:
	EnemyState* m_CurrentState;
	int m_Life = 3;

	class AnimationModel* m_Model;
	std::string m_AnimationName;
	std::string m_AnimationNameNext;
	float m_AnimationBlend;
	unsigned int m_Frame;

	float m_MoveMax;
	float m_ChaseDistance = 10.0f;
	float m_AttackDistance = 5.0f;
	bool m_IsAttacking = false;
	GameObject* m_Player = nullptr;

	float m_EnableDistance = 60.0f;

	Vector3 m_StartPosition;
	Vector3 m_StartRotation;
	float m_ReSpawnDistance = 30.0f;
	class ShadowVolume* m_ShadowVolume;

	int m_AttackType = 0;
public:
	void ChangeState(EnemyState* newState);
	void Init() override;   //èâä˙âª
	void UninitSelf() override; //èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;   //ï`âÊ
	void DrawInspector() override;

	void SetLife(int num) { m_Life = num; }
	void LifeDown(int num) { m_Life -= num; }

	const float GetMoveMax()const { return m_MoveMax; }
	void SetMoveMax(float move) { m_MoveMax = move; }

	const float GetChaseDistance()const { return m_ChaseDistance; }
	void SetChaseDistance(float chase) { m_ChaseDistance = chase; }

	const float GetAttackDistance()const { return m_AttackDistance; }

	bool GetIsAttacking() { return m_IsAttacking; }
	void SetIsAttacking(const bool attack) { m_IsAttacking = attack; }

	const float GetEnableDistance()const { return m_EnableDistance; }
	void SetEnableDistance(float distance) { m_EnableDistance = distance; }

	const Vector3 GetStartPosition()const { return m_StartPosition; }
	void SetStartPosition(Vector3 pos) { m_StartPosition = pos; }
	const Vector3 GetStartRotation()const { return m_StartRotation; }
	void SetStartRotation(Vector3 rot) { m_StartRotation = rot; }

	const float GetReSpawnDistance()const { return m_ReSpawnDistance; }
	void SetReSpawnDistance(float distance) { m_ReSpawnDistance = distance; }

	GameObject* GetPlayer() { return m_Player; }

	void ReSetFrame() { m_Frame = 0; }
	void AnimationBlend(std::string AnimationName);

	bool IsStateSearch() const;
	bool IsStateReturn() const;
	bool IsStateAttack() const;
	bool IsStateDead() const;
	bool IsStateCreater() const;

	void CanBeWire(bool can);

	int GetAttackType() { return m_AttackType; }
	void SetAttackType(int num) { m_AttackType = num; }
	
	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "Enemy"; }
};


#endif //ENEMY_H
