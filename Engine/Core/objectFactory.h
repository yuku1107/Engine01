/*==============================================================================

    [objectFactory.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2026/02/25
==============================================================================*/
#ifndef OBJECTFACTORY_H
#define OBJECTFACTORY_H

#include "Engine/Core/gameObject.h"
#include <unordered_map>

class ObjectFactory
{

public:
    struct ObjectInfo
    {
        std::function<GameObject* ()> Create;
        int DefaultLayer;
    };

    static void Register(const std::string& name, std::function<GameObject* ()> func, int layer)
    {
        m_Map[name] = { func, layer };
    }

    static GameObject* Create(const std::string& name)
    {
        auto it = m_Map.find(name);
        if (it != m_Map.end())
			return it->second.Create();

        return nullptr;
    }

    static int GetLayer(const std::string& name)
    {
        auto it = m_Map.find(name);
        if (it != m_Map.end())
            return it->second.DefaultLayer;

        return 0;
    }

    static const std::unordered_map<std::string, ObjectInfo>& GetMap()
    {
        return m_Map;
    }

private:
    static std::unordered_map<std::string, ObjectInfo> m_Map;

};

#endif //OBJECTFACTORY_H
