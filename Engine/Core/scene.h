/*==============================================================================

    ÉVÅ[Éìä«óù[scene.h]
                                                             Author:äsÅ@ìNâF
                                                             Date  :2025/06/25
==============================================================================*/
#ifndef SCENE_H
#define SCENE_H

#include <list>
#include <vector>
#include "Engine/Core/gameObject.h"
#include "Engine/Core/Input/Mouse/mousePicker3D.h"
#include "Engine/Core/Input/Mouse/mousePicker2D.h"
#include "Engine/Collision/collision.h"

#define LAYER (7)

class Scene
{
private:
    std::list<GameObject*> m_GameObjects[LAYER];
	MousePicker3D m_MousePicker3D;
	MousePicker2D m_MousePicker2D;
public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	virtual void Draw2DUI();
	virtual void Draw3DUI();
	virtual void DrawLayer(int layer);
	virtual void DrawShadow();
	virtual void DrawReflected();

	void CreateDefaultObject();

	template <typename T> //ÉeÉìÉvÉåÅ[Ég
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		gameObject->Init();
		m_GameObjects[Layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER; i++)
		{
			for (auto gameObject : m_GameObjects[i])
			{
				//gameObjectÇTå^Ç…ïœä∑
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					return find;
				}
			}
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> finds;

		for (int i = 0; i < LAYER; i++)
		{
			for (auto gameObject : m_GameObjects[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					finds.push_back(find);
				}
			}
		}
		return finds;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int num)
	{
		std::vector<T*> finds;

		for (auto gameObject : m_GameObjects[num])
		{
			T* find = dynamic_cast<T*>(gameObject);
			if (find != nullptr)
			{
				finds.push_back(find);
			}
		}
		
		return finds;
	}

	template <typename T>
	std::list<T*> GetGameObjectsByList()
	{
		std::list<T*> finds;

		for (int i = 0; i < LAYER; i++)
		{
			for (auto gameObject : m_GameObjects[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
				{
					finds.push_back(find);
				}
			}
		}
		return finds;
	}

	std::list<GameObject*>& GetGameObjects(int i)
	{
		return m_GameObjects[i];
	}

	//Factoryóp
	GameObject* AddGameObject(GameObject* obj, int layer)
	{
		obj->Init();

		m_GameObjects[layer].push_back(obj);

		return obj;
	}

	void SaveAllGameObjects(const std::string& fileName);
	void LoadAllGameObjects(const std::string& fileName);
	void SaveAllGameObjectsJSON(const std::string& fileName);
	void LoadAllGameObjectsJSON(const std::string& fileName);

	void DataSave(const std::string& fileName);
	void DataLoad(const std::string& fileName);

	bool IsJson(const std::string& path)
	{
		if (path.length() < 5)
			return false;

		return path.substr(path.length() - 5) == ".json";
	}


	void ClearGameObjects(const int num);
};


#endif //SCENE_H