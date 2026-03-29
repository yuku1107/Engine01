/*==============================================================================

	ゲームオブジェクト[gameObject.h]
															 Author:郭　哲宇
															 Date  :2025/05/21
==============================================================================*/
#include "main.h"
#include "Engine/Core/gameObject.h"
#include "ThirdParty/ImGui/imgui.h"
#include "Engine/Serialization/serializer.h"

void GameObject::Serialize(Serializer& s)
{
    s.WriteString("tag", m_Tag);

    s.WriteVector3("position", m_Position);
    s.WriteVector3("rotation", m_Rotation);
    s.WriteVector3("scale", m_Scale);

    s.WriteXMFLOAT4("color", m_MaterialColor);

    s.WriteString("shader", m_ShaderName);
}

void GameObject::Deserialize(Serializer& s)
{
    m_Tag = s.ReadString("tag");

    m_Position = s.ReadVector3("position");
    m_Rotation = s.ReadVector3("rotation");
    m_Scale = s.ReadVector3("scale");

    m_MaterialColor = s.ReadXMFLOAT4("color");

    m_ShaderName = s.ReadString("shader");
}
