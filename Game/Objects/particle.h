/*==============================================================================

	パーティクル[particle.h]
															 Author:郭　哲宇
															 Date  :2025/07/01
==============================================================================*/
#ifndef PARTICLEN_H
#define PARTICLEN_H

#include "Engine/Core/gameObject.h"
#include "Engine/Rendering/structuredBuffer.h"

#define ColorCount (1.0f/23.0f)
#define PARTICLE_MAX (10000)

class Particle : public GameObject
{
private:

	StructuredBuffer<Vector3> m_PositionBuffer;
	StructuredBuffer<Vector3> m_ScaleBuffer;

	Vector3 m_InstancePosition[PARTICLE_MAX];
	Vector3 m_InstanceScale[PARTICLE_MAX];

	int m_InstanceCount = 0;

	class QuadMesh* m_QuadMesh = nullptr;

	ID3D11ShaderResourceView* m_Texture;

	Vector3 m_Color = { 1.0f,1.0f,1.0f };

	struct PARTICLE
	{
		bool Enable;
		bool OneTime;
		int Life;
		Vector3 Position;
		Vector3 Velocity;
	};

	enum ColorMode
	{
		Mode_NONE = 0,
		Mode_RtoG,
		Mode_GtoB,
		Mode_BtoR,
		Mode_MAX,
	};

	ColorMode m_Mode = Mode_NONE;

	int m_Type = 0;

	PARTICLE m_Particle[PARTICLE_MAX];

	Vector3 m_Dir;
	float m_Power;
	float m_DestoryCount;

public:
	void Init() override;   //初期化
	void Init(Vector3 pos, Vector3 dir);
	void UninitSelf() override; //終了
	void UpdateSelf() override; //更新
	void DrawSelf() override;   //描画
	void SetType(int Type) { m_Type = Type; }

	std::string GetObjectClassName() const override { return "Particle"; }
};

#endif //PARTICLEN_H
