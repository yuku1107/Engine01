/*==============================================================================

    ÉvÉåÉCÉÑÅ[[player.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :
==============================================================================*/
#ifndef PLAYER_H
#define PLAYER_H
#include "main.h"
#include <list>
#include <iostream>
#include "Engine/Components/shape.h"
#include "Engine/Core/gameObject.h"

#define BULLET_MAX (5)


class Player;
class Wire;

class PlayerState
{
public:
	virtual ~PlayerState() {}
	virtual void Enter(Player* player) = 0;
	virtual void Update(Player* player, float deltatime) = 0;
	virtual void Exit(Player* player) = 0;
};

class Player : public GameObject
{
private:
	PlayerState* m_CurrentState;
	Vector3 m_StartPosition;
	Vector3 m_StartRotation;
	Vector3 m_TargetRotation;
	Vector3 m_ReSpawnPosition;
	int m_MaxLife = 3;
	int m_Life = 0;

	class AnimationModel* m_Model;
	std::string m_AnimationName;
	std::string m_AnimationNameNext;
	float m_AnimationBlend;
	unsigned int m_Frame;
	bool m_IsAnimationLoop = true;

	bool m_IsGrounded = false;

	std::vector<Shape*> m_ground;

	bool m_IsJumpUp = false;
	bool m_IsJumping = false;
	Vector3 m_JumpPower = Vector3(0.0f, 5.0f, 0.0f);

	Vector3 m_WireBusterPower = Vector3(0.0f, 0.0f, 0.0f);
	float m_EffDistance = 7.0f;
	GameObject* m_TargetObject = nullptr;

	class Audio* m_SE;
	class Audio* m_SEjump;

	class Wire* m_Wire = nullptr;

	int m_BulletMax = BULLET_MAX;
	int m_BulletNum = 0;
public:
	void ChangeState(PlayerState* newState);
	void Init() override;   //èâä˙âª
	void UninitSelf() override; //èIóπ
	void UpdateSelf() override; //çXêV
	void DrawSelf() override;  //ï`âÊ
	void DrawInspector() override;

	void ResolveCollisions();

	const Vector3 GetStartPosition()const { return m_StartPosition; }
	void SetStartPosition(Vector3 startPosition) { m_StartPosition = startPosition; }
	const Vector3 GetStartRotation()const { return m_StartRotation; }
	void SetStartRotation(Vector3 startRotation) { m_StartRotation = startRotation; }

	const int GetMaxLife()const { return m_MaxLife; }
	const int GetLife()const { return m_Life; }
	void SetLifeMax() { m_Life = m_MaxLife; }
	void SetLifeDown(int damage) { m_Life-= damage; }
	void Heal();

	const float GetEffDistance()const{return m_EffDistance;}

	GameObject* GetTargetObject() { return m_TargetObject; }

	void SetReturn();

	const bool GetIsJumpUp()const { return m_IsJumpUp; }
	void SetIsJumpUp(bool jumpUp) { m_IsJumpUp = jumpUp; }

	const bool GetIsJump()const { return m_IsJumping; }
	void SetIsJump(bool jump);

	const Vector3 GetJumpPower()const { return m_JumpPower; }

	const bool GetIsGrounded()const { return m_IsGrounded; }

	const Vector3 GetMoveInputFromCamera();

	Wire* GetWire() { return m_Wire; }

	void WireShot();
	void WireUnSet();

	void SetWireBusterPower(Vector3 power) { m_WireBusterPower = power; }
	const Vector3 GetWireBusterPower() const { return m_WireBusterPower; }

	const int GetBulletMax() const { return m_BulletMax; }
	void SetBulletNum(int num) { m_BulletNum = num; }
	int GetBulletNum()  { return m_BulletNum; }
	void AddBulletNum() { m_BulletNum++; }
	void BulletShot();

	void ReSetFrame() { m_Frame = 0; }
	void SetIsAnimationLoop(bool loop) { m_IsAnimationLoop = loop; }
	const bool GetIsAnimationLoop()const { return m_IsAnimationLoop; }
	void AnimationBlend(std::string AnimationName);

	bool IsStateWire() const;
	void SetInvincible();
	bool IsStateInvincible() const;
	bool IsStateClear() const;
	bool IsStateDead() const;
	bool IsStateCreater() const;

	bool IsJumpTrigger() const;
	bool IsBulletShotTrigger() const;
	bool IsWirePrevTrigger() const;
	bool IsWireNextTrigger() const;
	bool IsWireShotTrigger() const;
	

	void Serialize(Serializer& s) override;
	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "Player"; }
};


#endif //PLAYER_H
