/*==============================================================================

	ゲームオブジェクト[gameObject.h]
															 Author:郭　哲宇
															 Date  :2025/05/21
==============================================================================*/
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "main.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Components/component.h"
#include <list>
class Serializer;

class GameObject
{
private:
	std::list<Component*> m_Components;

protected:
	bool m_Destroy = false;
	std::string m_Tag;
	Vector3 m_Position{ 0.0f,0.0f,0.0f };            //位置
	Vector3 m_Rotation{ 0.0f,0.0f,0.0f };            //角度
	Vector3 m_Scale{ 1.0f,1.0f,1.0f };               //大きさ
	XMFLOAT4 m_MaterialColor{ 1.0f,1.0f,1.0f,1.0f }; //マテリア設定

	bool m_MouseSelection = false;  // マウス用選択されているか
	std::string m_ShaderName;

	bool m_UpdateEnable = true;
	bool m_DrawEnable = true;
	bool m_SaveEnable = true;

	virtual void UninitSelf() {}
	virtual void UpdateSelf() {}
	virtual void DrawSelf() {}
public:
	//仮想関数
	virtual void Init() {}
	void Uninit() 
	{
		for (auto comp : m_Components)
		{
			comp->Uninit();
			delete comp;
		}

		UninitSelf();
	}
	void Update()
	{
		if (m_UpdateEnable)
		{
			for (auto comp : m_Components)
			{
				comp->Update();
			}

			UpdateSelf();
		}
	}
	void Draw()
	{
		if (m_DrawEnable)
		{
			for (auto component : m_Components)
			{
				if (component->IsAutoDraw())
				{
					component->Draw();
				}
			}

			DrawSelf();
		}
	}

	virtual void DrawInspector() {}

	void SetDestroy() { m_Destroy = true; }
	bool GetDestroy() { return m_Destroy; }

	bool Destroy()
	{
		if (m_Destroy)
		{
			UninitSelf();
			delete this;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void SetTag(const std::string& tag) { m_Tag = tag; }
	const std::string& GetTag()const { return m_Tag; }

	void SetPosition(const Vector3& pos) { m_Position = pos; }
	const Vector3& GetPosition()const { return m_Position; }

	void SetRotation(const Vector3& rot) { m_Rotation = rot; }
	const Vector3& GetRotation()const { return m_Rotation; }

	void SetScale(const Vector3& scale) { m_Scale = scale; }
	const Vector3& GetScale()const { return m_Scale; }

	void SetMaterialColor(const XMFLOAT4& materialColor) { m_MaterialColor = materialColor; }
	const XMFLOAT4& GetMaterialColor()const { return m_MaterialColor; }

	void SetMouseSelection(const bool selection) { m_MouseSelection = selection; }
	const bool GetMouseSelection()const { return m_MouseSelection; }

	void SetUpdateEnable(const bool update) { m_UpdateEnable = update; }
	const bool GetUpdateEnable() { return m_UpdateEnable; }

	void SetDrawEnable(const bool draw) { m_DrawEnable = draw; }
	const bool GetDrawEnable() { return m_DrawEnable; }

	void SetSaveEnable(const bool save){m_SaveEnable = save; }
	const bool GetSaveEnable()const { return m_SaveEnable; }
	
	void SetShaderName(const std::string& shader) { m_ShaderName = shader; }
	const std::string& GetShaderName()const { return m_ShaderName; }

	Vector3 GetRight()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 right;
		XMStoreFloat3((XMFLOAT3*)&right, matrix.r[0]);

		return right;
	}

	Vector3 GetForward()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 forward;
		XMStoreFloat3((XMFLOAT3*)&forward, matrix.r[2]);

		return forward;
	}

	Vector3 GetUp()
	{
		XMMATRIX matrix;
		matrix = XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);

		Vector3 up;
		XMStoreFloat3((XMFLOAT3*)&up, matrix.r[1]);

		return up;
	}

	virtual void Serialize(Serializer& s);
	virtual void Deserialize(Serializer& s);
	
	virtual std::string GetObjectClassName() const { return "GameObject"; }

	float GetDistance(Vector3 Position)
	{
		return (m_Position - Position).length();
	}

	float GetDepth(Vector3 Position, Vector3 Direction)
	{
		Vector3 ToObject = m_Position - Position;

		float Deph = Direction.dot(ToObject);

		return Deph;
	}

	template <typename T> //テンプレート
	T* AddComponent()
	{
		T* component = new T();
		component->Init();
		m_Components.push_back(component);
		
		return component;
	}

	template <typename T>
	T* GetComponent()
	{
		for (auto component : m_Components)
		{
			//gameObjectをT型に変換
			T* find = dynamic_cast<T*>(component);

			if (find != nullptr)
			{
				return find;
			}
		}
		
		return nullptr;
	}
};

#endif //GAMEOBJECT_H
