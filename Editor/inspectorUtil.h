/*==============================================================================

    ã§í UIïîïi[inspectorUtil.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/26
==============================================================================*/
#ifndef INSPECORUTIL_H
#define INSPECORUTIL_H

class GameObject;

namespace InspectorUtil
{
    void DrawTransform(GameObject* obj);
    void DrawShader(GameObject* obj);
    void DrawCollision(GameObject* obj);
}

#endif //INSPECORUTIL_H
