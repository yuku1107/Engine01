/*==============================================================================

	座標系[coordinate.h]
															 Author:郭　哲宇
															 Date  :2025/06/05
==============================================================================*/
#ifndef COORDINATE_H
#define COORDINATE_H

#include "Engine/Core/gameObject.h"
#include "Engine/Components/shape.h"

class Coordinate : public GameObject
{
private:
	GameObject* m_Target = nullptr;

	class AnimationModel* m_Model;

	Shape* m_Selection = nullptr;
	int m_SelectionNum = -1;
	bool m_SelectionSwitch = false;

	Box m_Box[3];
	Vector3 m_Pos[3];
	Vector3 m_Scl[3];
	XMVECTOR m_QRot[3];

public:
	void Init() override;   //初期化
	void UninitSelf() override;  //終了
	void UpdateSelf() override;  //更新
	void DrawSelf() override;    //描画

	// 選択された軸のドラッグ
	void StartAxisDrag(int axisIndex);
	
	// 軸ドラッグを終了する
	void EndAxisDrag();

	// 3軸ギズモ（X/Y/Z）の位置・向き・スケールを更新する
	void UpdateAxisBoxes();

	// 選択中の軸方向に沿ってターゲットを移動
	void ApplyAxisMovement();
	
	void SetTarget(GameObject* obj);
	GameObject* GetTarget() { return m_Target; }

	Box* IsHit(const Line& line, int* out = nullptr);
	int* GetBoxNum() { return &m_SelectionNum; }

	std::string GetObjectClassName() const override { return "Coordinate"; }
};

#endif //COORDINATE_H
