/*==============================================================================

	座標系[coordinate2D.h]
															 Author:郭　哲宇
															 Date  :2025/09/26
==============================================================================*/
#ifndef COORDINATE2D_H
#define COORDINATE2D_H

#include "Engine/Objects/UIObjects/ui.h"

class Coordinate2D : public UI
{
private:
	GameObject* m_Target = nullptr;

	ID3D11ShaderResourceView* m_TextureX;
	ID3D11ShaderResourceView* m_TextureY;

	Vector3 m_Pos[2];
	Vector3 m_Scl[2];
	Vector3 m_Rot[2];

	int m_SelectionNum = -1;
	bool m_SelectionSwitch = false;

	Vector3 m_ClickOffset;
protected:
	virtual std::string GetDefaultTexture() const { return "result"; }
public:
	void Init() override;   //初期化
	void UninitSelf() override;  //終了
	void UpdateSelf() override;  //更新
	void DrawSelf() override;    //描画

	// 軸ドラッグ開始
	void StartAxisDrag(int axisIndex, const Vector3& mousePos);

	// 軸ドラッグ終了
	void EndAxisDrag();

	// マウス移動に応じてターゲットを更新
	void ApplyAxisMovement(const Vector3& mousePos);

	void SetTarget(GameObject* obj) { m_Target = obj; }
	GameObject* GetTarget() { return m_Target; }
	bool GetCollision(Vector3 pos);

	int* GetNum() { return &m_SelectionNum; }

	std::string GetObjectClassName() const override { return "Coordinate2D"; }
};

#endif //COORDINATE2D_H
