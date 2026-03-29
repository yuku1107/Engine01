/*==============================================================================

    [mousePicker3D]
                                                             Author:郭　哲宇
                                                             Date  :2026/02/27
==============================================================================*/
#ifndef MOUSEPICKER3D_H
#define MOUSEPICKER3D_H

class GameObject;
class Coordinate;
class Camera;
class Line;

class MousePicker3D
{
private:
    GameObject* m_SelectedObject = nullptr;
    Coordinate* m_Coordinate = nullptr;
    bool m_IsAxisDragging = false;

    // マウス位置からワールドレイを生成する
    Line CreateRay(Camera* camera);

    // 軸ギズモとのヒット判定
    void HandleAxisSelection(const Line& ray);

    // シーンオブジェクトとのヒット判定
    void HandleObjectSelection(const Line& ray);

    // 現在の選択を解除する
    void ClearSelection();

public:
    void Update();

};

#endif //MOUSEPICKER3D_H