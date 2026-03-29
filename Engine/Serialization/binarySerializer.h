/*==============================================================================

    バイナリシリアライズ処理[birnarySerializer.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/03/12
==============================================================================*/
#ifndef BINARYSERIALIZER_H
#define BINARYSERIALIZER_H

#include "Serializer.h"
#include <ostream>
#include <istream>

class BinarySerializer : public Serializer
{
private:

    std::ostream* m_Out;
    std::istream* m_In;

public:

    BinarySerializer(std::ostream& os);
    BinarySerializer(std::istream& is);

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

#endif //BINARYSERIALIZER_H