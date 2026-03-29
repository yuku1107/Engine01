/*==============================================================================

	シーン管理[scene.h]
															 Author:郭　哲宇
															 Date  :2025/06/25
==============================================================================*/
#include "main.h"
#include "Engine/Core/manager.h"
#include "Engine/Core/scene.h"
#include "Engine/Core/objectFactory.h"
#include "Engine/Rendering/Renderer/renderer.h"
#include "Engine/Collision/collision.h"
#include "Engine/Objects/SceneObjects/scenePolygon.h"
#include "Engine/Objects/SceneObjects/coordinate2D.h"
#include "Engine/Objects/SceneObjects/coordinate.h"
#include "Engine/Objects/SceneObjects/cameraUI.h"
#include "Engine/Objects/SceneObjects/camera.h"
#include "Engine/Objects/SceneObjects/light.h"
#include "Engine/Objects/UIObjects/ui.h"
#include "Engine/Serialization/binarySerializer.h"
#include "Engine/Serialization/jsonSerializer.h"

# include <iostream>
# include <fstream>

#include "ThirdParty/Jason/json.hpp"
using json = nlohmann::json;

void Scene::Init()
{

}

void Scene::Uninit()
{
	for (int i = 0; i < LAYER; i++)
	{
		for (auto gameObject : m_GameObjects[i])
		{
			gameObject->Uninit();
			delete gameObject;
		}

		m_GameObjects[i].clear();
	}

	Manager::GetCollision()->Clear();
}

void Scene::Update()
{
	m_MousePicker3D.Update();
	m_MousePicker2D.Update();

	for (int i = 0; i < LAYER; i++)
	{
		for (auto gameObject : m_GameObjects[i])//範囲forループ
		{
			gameObject->Update(); //ポリモフィズム
		}
	}

	//当たり判定を消す
	Manager::GetCollision()->Remove();

	//順番↓

	//オブジェクトを消す
	for (int i = 0; i < LAYER; i++)
	{
		m_GameObjects[i].remove_if([](GameObject* object) {return object->Destroy(); });
	}
}

void Scene::Draw()
{
	//Zソート
	Camera* camera = GetGameObject<Camera>();

	if (camera != nullptr)
	{
		Vector3 cameraPosition = camera->GetPosition();
		Vector3 cmaeraDirection = camera->GetDirection();

		m_GameObjects[5].sort([&](GameObject* object1, GameObject* object2) {return object1->GetDepth(cameraPosition, cmaeraDirection) > object2->GetDepth(cameraPosition, cmaeraDirection); });
	}

	for (int i = 4; i < LAYER; i++)
	{
		for (auto gameObject : m_GameObjects[i])
		{
			if (gameObject->GetTag() == "REFLECTIONCAMERA")
			{
				continue;
			}

			gameObject->Draw();
		}
	}


}

void Scene::Draw2DUI()
{
	auto& uiObjects = m_GameObjects[1];

	uiObjects.sort([](GameObject* a, GameObject* b){return a->GetPosition().z < b->GetPosition().z;});

	for (auto gameObject : m_GameObjects[1])
	{
		gameObject->Draw();
	}
}

void Scene::Draw3DUI()
{
	for (auto gameObject : m_GameObjects[2])
	{
		gameObject->Draw();
	}
}

void Scene::DrawLayer(int layer)
{
	//Zソート
	Camera* camera = GetGameObject<Camera>();
	
	if (camera != nullptr)
	{
		Vector3 cameraPosition = camera->GetPosition();
		Vector3 cmaeraDirection = camera->GetDirection();
	
		m_GameObjects[layer].sort([&](GameObject* object1, GameObject* object2) {return object1->GetDepth(cameraPosition, cmaeraDirection) > object2->GetDepth(cameraPosition, cmaeraDirection); });
	}

	for (auto gameObject : m_GameObjects[layer])
	{
		gameObject->Draw();
	}
	
}

void Scene::DrawShadow()
{
	auto* lightObj = GetGameObject<Light>();
	if (lightObj)
	{
		Renderer::SetLight(*lightObj->GetLight());
	}
	Renderer::BeginDepth();

	for (int i = 4; i < LAYER; i++)
	{
		for (auto gameObject : m_GameObjects[i])
		{
			if (gameObject->GetTag() == "CAMERA")
			{
				continue;
			}

			if (gameObject->GetTag() == "SHADOWVOLUME")
			{
				continue;
			}

			if (gameObject->GetTag() == "GRASS")
			{
				continue;
			}

			gameObject->Draw();
		}
	}
}

void Scene::DrawReflected()
{
	Renderer::BeginReflection();
	Renderer::UpdateReflectionViewProj(GetGameObject<Camera>(), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

	//Zソート
	Camera* camera = GetGameObject<Camera>();

	if (camera != nullptr)
	{
		Vector3 cameraPosition = camera->GetPosition();
		Vector3 cmaeraDirection = camera->GetDirection();

		m_GameObjects[5].sort([&](GameObject* object1, GameObject* object2) {return object1->GetDepth(cameraPosition, cmaeraDirection) > object2->GetDepth(cameraPosition, cmaeraDirection); });
	}

	for (int i = 4; i < LAYER; i++)
	{
		for (auto gameObject : m_GameObjects[i])
		{
			if (gameObject->GetTag() == "WAVE")
			{
				continue;
			}

			if (gameObject->GetTag() == "CAMERA")
			{
				continue;
			}

			if (gameObject->GetTag() == "SHADOWVOLUME")
			{
				continue;
			}

			gameObject->Draw();
		}
	}
}

void Scene::CreateDefaultObject()
{
	Manager::GetScene()->AddGameObject<ScenePolygon2D>(0);
	Manager::GetScene()->AddGameObject<Coordinate2D>(1);
	Manager::GetScene()->AddGameObject<CameraUI>(2);
	Manager::GetScene()->AddGameObject<Coordinate>(3);
	Manager::GetScene()->AddGameObject<Light>(3);
	Manager::GetScene()->AddGameObject<Camera>(4);
}

void Scene::SaveAllGameObjects(const std::string& fileName)
{
	std::ofstream ofs(fileName, std::ios::binary);
	if (!ofs)
	{
		MessageBoxA(nullptr, ("ファイルを開けません: " + fileName).c_str(), "保存エラー", MB_OK | MB_ICONERROR);
		return;
	}

	// レイヤー数を保存
	uint64_t layerCount = static_cast<uint64_t>(LAYER);
	ofs.write(reinterpret_cast<const char*>(&layerCount), sizeof(layerCount));

	BinarySerializer  serializer(ofs);

	for (int layer = 0; layer < LAYER; layer++)
	{
		//保存対象だけをカウントする
		uint64_t count = 0;
		for (const auto& obj : m_GameObjects[layer])
		{
			if (obj && obj->GetSaveEnable())
				count++;
		}

		//count を書き込む
		ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

		//保存対象だけを書き込む
		for (const auto& obj : m_GameObjects[layer])
		{
			if (!obj || !obj->GetSaveEnable()) continue;

			std::string className = obj->GetObjectClassName();

			uint64_t nameLen = className.size();

			ofs.write((char*)&nameLen, sizeof(nameLen));
			ofs.write(className.data(), nameLen);

			obj->Serialize(serializer);
		}
	}
}

void Scene::LoadAllGameObjects(const std::string& fileName)
{
	std::ifstream ifs(fileName, std::ios::binary);

	if (!ifs)
	{
		MessageBoxA(nullptr,
			("ファイルを開けません: " + fileName).c_str(),
			"読み込みエラー",
			MB_OK | MB_ICONERROR);
		return;
	}

	// 既存オブジェクト削除
	for (int layer = 0; layer < LAYER; layer++)
	{
		ClearGameObjects(layer);
	}

	Manager::GetCollision()->Clear();

	uint64_t layerCount = 0;

	ifs.read(reinterpret_cast<char*>(&layerCount), sizeof(layerCount));

	if (!ifs || layerCount > LAYER)
	{
		MessageBoxA(nullptr,
			"layerCount が異常です",
			"Binary Load Error",
			MB_OK | MB_ICONERROR);
		return;
	}

	BinarySerializer serializer(ifs);

	for (uint64_t layer = 0; layer < layerCount; ++layer)
	{
		uint64_t count = 0;

		ifs.read(reinterpret_cast<char*>(&count), sizeof(count));

		if (!ifs || count > 10000)
		{
			std::string msg =
				"count が異常です\nLayer: " + std::to_string(layer) +
				"\ncount: " + std::to_string(count);

			MessageBoxA(nullptr, msg.c_str(),
				"Binary Load Error",
				MB_OK | MB_ICONERROR);
			return;
		}

		for (uint64_t i = 0; i < count; ++i)
		{
			std::streampos pos = ifs.tellg();

			uint64_t nameLen = 0;

			ifs.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));

			if (!ifs || nameLen > 1024)
			{
				std::string msg =
					"nameLen が異常です\nLayer: " + std::to_string(layer) +
					"\nnameLen: " + std::to_string(nameLen) +
					"\nFilePos: " + std::to_string((long long)pos);

				MessageBoxA(nullptr, msg.c_str(),
					"Binary Load Error",
					MB_OK | MB_ICONERROR);
				return;
			}

			std::string className(static_cast<size_t>(nameLen), '\0');

			ifs.read(&className[0], nameLen);

			if (!ifs)
			{
				std::string msg =
					"className 読み込み失敗\nLayer: " + std::to_string(layer) +
					"\nFilePos: " + std::to_string((long long)pos);

				MessageBoxA(nullptr, msg.c_str(),
					"Binary Load Error",
					MB_OK | MB_ICONERROR);
				return;
			}

			// GameObject生成
			GameObject* obj = ObjectFactory::Create(className);

			if (!obj)
			{
				std::string msg =
					"不明なクラス名\nClass: " + className +
					"\nLayer: " + std::to_string(layer);

				MessageBoxA(nullptr, msg.c_str(),
					"Binary Load Error",
					MB_OK | MB_ICONERROR);

				continue;
			}

			// Deserialize
			try
			{
				obj->Deserialize(serializer);
			}
			catch (const std::exception& e)
			{
				std::streampos errPos = ifs.tellg();

				std::string msg =
					"Deserialize Error\n"
					"Class: " + className +
					"\nLayer: " + std::to_string(layer) +
					"\nFilePos: " + std::to_string((long long)errPos) +
					"\nError: " + std::string(e.what());

				MessageBoxA(nullptr, msg.c_str(),
					"Binary Deserialize Error",
					MB_OK | MB_ICONERROR);

				delete obj;
				return;
			}

			m_GameObjects[layer].push_back(obj);
		}
	}

	// Init
	for (int layer = 0; layer < LAYER; ++layer)
	{
		for (auto& obj : m_GameObjects[layer])
		{
			if (obj)
			{
				obj->Init();
			}
		}
	}
}

void Scene::SaveAllGameObjectsJSON(const std::string& fileName)
{
	json root;

	root["layerCount"] = LAYER;
	root["layers"] = json::array();

	for (int layer = 0; layer < LAYER; layer++)
	{
		json layerJson;
		layerJson["objects"] = json::array();

		for (const auto& obj : m_GameObjects[layer])
		{
			if (!obj || !obj->GetSaveEnable()) continue;

			json objJson;

			objJson["class"] = obj->GetObjectClassName();

			json data;

			JsonSerializer serializer(data);

			obj->Serialize(serializer);

			objJson["data"] = data;

			layerJson["objects"].push_back(objJson);
		}

		root["layers"].push_back(layerJson);
	}

	std::ofstream ofs(fileName);
	ofs << root.dump(4); // 4はインデント
}

void Scene::LoadAllGameObjectsJSON(const std::string& fileName)
{
	std::ifstream ifs(fileName);
	if (!ifs) return;

	json root;
	ifs >> root;

	// 既存をクリア
	for (int layer = 0; layer < LAYER; layer++)
	{
		ClearGameObjects(layer);
	}

	Manager::GetCollision()->Clear();

	if (!root.contains("layers"))
	{
		MessageBoxA(nullptr, "layers がありません", "読み込みエラー", MB_OK);
		return;
	}

	auto& layers = root["layers"];

	for (size_t layer = 0; layer < layers.size() && layer < LAYER; ++layer)
	{
		for (auto& objJson : layers[layer]["objects"])
		{
			if (!objJson.contains("class"))
			{
				MessageBoxA(nullptr, "class がありません", "JSON Error", MB_OK);
				continue;
			}

			std::string className = objJson["class"];

			GameObject* obj = ObjectFactory::Create(className);
			if (!obj)
			{
				std::string msg = "Unknown class: " + className;
				MessageBoxA(nullptr, msg.c_str(), "JSON Error", MB_OK);
				continue;
			}

			if (!objJson.contains("data"))
			{
				MessageBoxA(nullptr, "data がありません", "JSON Error", MB_OK);
				delete obj;
				continue;
			}

			try
			{
				json& data = objJson["data"];

				JsonSerializer serializer(data);

				obj->Deserialize(serializer);
			}
			catch (const std::exception& e)
			{
				std::string msg = "Deserialize Error\n";
				msg += "Class: " + className + "\n";
				msg += e.what();

				MessageBoxA(nullptr, msg.c_str(), "JSON Error", MB_OK);

				delete obj;
				continue;
			}

			m_GameObjects[layer].push_back(obj);
		}
	}

	// Init
	for (auto& layer : m_GameObjects)
	{
		for (auto& obj : layer)
		{
			obj->Init();
		}
	}
}

void Scene::DataSave(const std::string& fileName)
{
	if (IsJson(fileName))
		SaveAllGameObjectsJSON(fileName);
	else
		SaveAllGameObjects(fileName);
}

void Scene::DataLoad(const std::string& fileName)
{
	if (IsJson(fileName))
		LoadAllGameObjectsJSON(fileName);
	else
		LoadAllGameObjects(fileName);
}

void Scene::ClearGameObjects(const int num)
{

	for (auto gameObject : m_GameObjects[num])
	{
		gameObject->Uninit();
		delete gameObject;
	}

	m_GameObjects[num].clear();

}
