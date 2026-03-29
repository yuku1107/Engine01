/*==============================================================================

    [mousePicker2D.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/27
==============================================================================*/
#ifndef MOUSEPICKER2D_H
#define MOUSEPICKER2D_H

class Coordinate2D;
class GameObject;

class MousePicker2D
{
private:
    Coordinate2D* m_Coordinate = nullptr;
    GameObject* m_SelectedObject = nullptr;
    bool m_IsDragging = false;

public:
    void Update();
};
#endif //MOUSEPICKER2D_H
