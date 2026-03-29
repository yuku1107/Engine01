/*==============================================================================

	JSONÉVÉäÉAÉâÉCÉYèàóù[jsonSerializer.cpp]
															 Author:äsÅ@ìNâF
															 Date  :2026/03/12
==============================================================================*/
#include "main.h"
#include "Engine/Serialization/jsonSerializer.h"
#include "Engine/Math/vector3.h"

#include "ThirdParty/Jason/json.hpp"

using json = nlohmann::json;

JsonSerializer::JsonSerializer(json& j)
{
    m_Json = &j;
}

void JsonSerializer::WriteInt(const std::string& name, int value)
{
    (*m_Json)[name] = value;
}

void JsonSerializer::WriteFloat(const std::string& name, float value)
{
    (*m_Json)[name] = value;
}

void JsonSerializer::WriteString(const std::string& name, const std::string& value)
{
    (*m_Json)[name] = value;
}

void JsonSerializer::WriteVector3(const std::string& name, const Vector3& value)
{
    (*m_Json)[name] =
    {
        value.x,
        value.y,
        value.z
    };
}

void JsonSerializer::WriteXMFLOAT4(const std::string& name, const XMFLOAT4& value)
{
    (*m_Json)[name] =
    {
        value.x,
        value.y,
        value.z,
        value.w
	};
}

int JsonSerializer::ReadInt(const std::string& name)
{
    return (*m_Json)[name];
}

float JsonSerializer::ReadFloat(const std::string& name)
{
    return (*m_Json)[name];
}

std::string JsonSerializer::ReadString(const std::string& name)
{
    return (*m_Json)[name];
}

Vector3 JsonSerializer::ReadVector3(const std::string& name)
{
    auto& j = (*m_Json)[name];

    Vector3 v;
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];

    return v;
}

XMFLOAT4 JsonSerializer::ReadXMFLOAT4(const std::string& name)
{
    auto& j = (*m_Json)[name];

    XMFLOAT4 v;
    v.x = j[0];
    v.y = j[1];
    v.z = j[2];
    v.w = j[3];

    return v;
}

