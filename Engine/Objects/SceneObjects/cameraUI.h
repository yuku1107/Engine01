/*==============================================================================

    3DUI描画用カメラ[cameraUI.h]
                                                             Author:郭　哲宇
                                                             Date  :2025/05/14
==============================================================================*/
#ifndef CAMERAUI_H
#define CAMERAUI_H

#include "Engine/Core/gameObject.h"

class CameraUI : public GameObject
{
private:

	XMMATRIX m_View;
	XMMATRIX m_Projection;

	Vector3 m_Up = { 0.0f,1.0f,0.0f };            //カメラの上方向
	Vector3 m_Target{ 0.0f,0.0f,0.0f };           //ターゲット
	float m_Fov = 1.0f;				              //視野角
	float m_Nearclip = 1.0f;		              //どこまで近くが見えるか
	float m_Farclip = 1000.0f;		              //どこまで遠くが見えるか

	Vector3 m_Velocity = { 0.0f,0.0f,0.0f };      //速度
	Vector3 m_Resistance = { -0.5f,-0.5f,-0.5f }; //抵抗
	float m_Distance = 8.0f;

public:
	void Init() override;   //初期化
	void UninitSelf() override;  //終了
	void UpdateSelf() override;  //更新
	void DrawSelf() override;    //描画

	std::string GetObjectClassName() const override { return "CameraUI"; }
};


#endif //CAMERAUI_H