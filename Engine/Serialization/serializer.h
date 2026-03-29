/*==============================================================================

    シリアライズ基底クラス[serializer.h]
                                                             Author:郭　哲宇
                                                             Date  :2026/03/12
==============================================================================*/
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>

class Vector3;

class Serializer
{
public:

    virtual void WriteInt(const std::string& name, int value) = 0;
    virtual void WriteFloat(const std::string& name, float value) = 0;
    virtual void WriteString(const std::string& name, const std::string& value) = 0;
    virtual void WriteVector3(const std::string& name, const Vector3& value) = 0;
    virtual void WriteXMFLOAT4(const std::string& name, const XMFLOAT4& value) = 0;

    virtual int ReadInt(const std::string& name) = 0;
    virtual float ReadFloat(const std::string& name) = 0;
    virtual std::string ReadString(const std::string& name) = 0;
    virtual Vector3 ReadVector3(const std::string& name) = 0;
	virtual XMFLOAT4 ReadXMFLOAT4(const std::string& name) = 0;
};

#endif //SERIALIZER_H