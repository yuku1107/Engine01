/*==============================================================================

    カメラ[camera.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/
#ifndef CAMERA_H
#define CAMERA_H

#include "Engine/Core/gameObject.h"

class Camera : public GameObject
{
public:
	// カメラモード定義
	enum CameraMode
	{
		ThirdPerson_Free,         // 通常
		ThirdPerson_Shooting,     // シューティング
		Fixed_Mode,
		Clear_Mode,
		Creater_Mode
	};
private:

	XMMATRIX m_View;
	XMMATRIX m_Projection;

	Vector3 m_Velocity = { 0.0f,0.0f,0.0f };      //速度
	Vector3 m_Resistance = { -0.5f,-0.5f,-0.5f }; //抵抗
	Vector3 m_Up = { 0.0f,1.0f,0.0f };            //カメラの上方向
	Vector3 m_TargetPosition = { 0.0f,0.0f,0.0f };   //ターゲット
	Vector3 m_Direction = { 0.0f,0.0f,0.0f };        //向き

	float m_Distance = 8.0f;		              //距離
	float m_Fov = 45.0f;				          //視野角
	float m_Nearclip = 1.0f;		              //どこまで近くが見えるか
	float m_Farclip = 1000.0f;		              //どこまで遠くが見えるか

	CameraMode m_Mode = CameraMode::Fixed_Mode;
	CameraMode m_NextMode = m_Mode;   // 切り替え予定のモード
	bool m_IsBlending = false;        // 補間中フラグ
	float m_BlendT = 0.0f;            // 補間率
	Vector3 m_BlendStartPos;          // 補間開始位置
	Vector3 m_BlendStartTarget;       // 補間開始注視点
	Vector3 m_BlendEndPos;            // 補間終了位置
	Vector3 m_BlendEndTarget;        // 補間終了注視点

	GameObject* m_Target = nullptr;
public:
	void Init() override;   //初期化
	void UninitSelf() override;  //終了
	void UpdateSelf() override;  //更新
	void DrawSelf() override;    //描画
	bool CheckView(Vector3 pos, Vector3 size, float dis = 0.0f);

	//モード変更による位置変更
	void GetModeCameraPos(CameraMode mode, GameObject* obj, Vector3& outPos, Vector3& outTarget);
	Vector3 GetTargetPosition() { return m_TargetPosition; }
	float GetFov() { return m_Fov; }

	// マウスホイール入力に応じてカメラとターゲット間の距離を更新する
	void UpdateDistanceByScroll();

	// カメラモードとマウスモードを同期させる
	void ApplyMouseMode(CameraMode mode);

	//マウスモードを再適用する
	void ReapplyMouseMode();

	//理想位置調整
	void IdealPos(CameraMode mode, GameObject* target);

	Vector3 AdjustByCollision(const Vector3& start, const Vector3& ideal, GameObject* target);

	XMMATRIX GetViewMatrix() { return m_View; }
	XMMATRIX GetProjectionMatrix() { return m_Projection; }

	Vector3 GetDirection() { return m_Direction; }

	CameraMode GetMode() { return m_Mode; }

	void ChangeMode(CameraMode newMode);

	bool IsBlending() const
	{
		return m_IsBlending;
	}

	void SetTarget(GameObject* target) { m_Target = target; }
	GameObject* GetTarget() const { return m_Target; }

	void Serialize(Serializer& s) override;

	void Deserialize(Serializer& s) override;

	std::string GetObjectClassName() const override { return "Camera"; }
};


#endif //CAMERA_H