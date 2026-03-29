/*==============================================================================

	バイナリシリアライズ処理[birnarySerializer.cpp]
															 Author:郭　哲宇
															 Date  :2026/03/12
==============================================================================*/
#include "main.h"
#include "Engine/Serialization/binarySerializer.h"
#include "Engine/Math/vector3.h"

BinarySerializer::BinarySerializer(std::ostream& os)
{
    m_Out = &os;
    m_In = nullptr;
}

BinarySerializer::BinarySerializer(std::istream& is)
{
    m_In = &is;
    m_Out = nullptr;
}

void BinarySerializer::WriteInt(const std::string& name, int value)
{
    m_Out->write(reinterpret_cast<char*>(&value), sizeof(value));
}

void BinarySerializer::WriteFloat(const std::string& name, float value)
{
    m_Out->write(reinterpret_cast<char*>(&value), sizeof(value));
}

void BinarySerializer::WriteString(const std::string& name, const std::string& value)
{
    uint32_t len = static_cast<uint32_t>(value.size());

    m_Out->write(reinterpret_cast<const char*>(&len), sizeof(len));
    m_Out->write(value.data(), len);
}

void BinarySerializer::WriteVector3(const std::string&, const Vector3& value)
{
    m_Out->write(reinterpret_cast<const char*>(&value), sizeof(Vector3));
}

void BinarySerializer::WriteXMFLOAT4(const std::string& name, const XMFLOAT4& value)
{
    m_Out->write(reinterpret_cast<const char*>(&value), sizeof(XMFLOAT4));
}

int BinarySerializer::ReadInt(const std::string& name)
{
    int value;
    m_In->read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

float BinarySerializer::ReadFloat(const std::string& name)
{
    float value;
    m_In->read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

std::string BinarySerializer::ReadString(const std::string& name)
{
    uint32_t len = 0;
    m_In->read(reinterpret_cast<char*>(&len), sizeof(len));

    if (len > 1024)
    {
        throw std::runtime_error("Deserialize: string length too big");
    }

    std::string value;
    value.resize(len);

    m_In->read(&value[0], len);

    return value;
}

Vector3 BinarySerializer::ReadVector3(const std::string& name)
{
    Vector3 value;
    m_In->read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}

XMFLOAT4 BinarySerializer::ReadXMFLOAT4(const std::string& name)
{
    XMFLOAT4 value;
    m_In->read(reinterpret_cast<char*>(&value), sizeof(value));
    return value;
}
