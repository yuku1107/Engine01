/*==============================================================================

    JSONÉVÉäÉAÉâÉCÉYèàóù[jsonSerializer.cpp]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/03/12
==============================================================================*/
#ifndef JSONSERIALIZER_H
#define JSONSERIALIZER_H

#include "Serializer.h"
#include "thirdParty/Jason/json.hpp"

using json = nlohmann::json;

class Vector3;

class JsonSerializer : public Serializer
{
private:

    json* m_Json;

public:

    JsonSerializer(json& j);

    void WriteInt(const std::string& name, int value) override;
    void WriteFloat(const std::string& name, float value) override;
    void WriteString(const std::string& name, const std::string& value) override;
    void WriteVector3(const std::string& name, const Vector3& value) override;
	void WriteXMFLOAT4(const std::string& name, const XMFLOAT4& value) override;

    int ReadInt(const std::string& name) override;
    float ReadFloat(const std::string& name) override;
    std::string ReadString(const std::string& name) override;
	Vector3 ReadVector3(const std::string& name) override;
	XMFLOAT4 ReadXMFLOAT4(const std::string& name) override;
};

#endif //JSONSERIALIZER_H