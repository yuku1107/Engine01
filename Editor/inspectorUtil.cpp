/*==============================================================================

	‹¤’ÊUI•”•i[inspectorUtil.cpp]
															 Author:Šs@“N‰F
															 Date  :2026/02/26
==============================================================================*/
#include "main.h"
#include "Engine/Core/gameObject.h"
#include "Engine/Components/shape.h"
#include "Engine/Components/shader.h"
#include "Engine/Components/rigidbody.h"
#include "ThirdParty/ImGui/imgui.h"
#include "inspectorUtil.h"

void InspectorUtil::DrawTransform(GameObject* obj)
{
	if (ImGui::TreeNode("Transform"))
	{
		float Object_pos[4] = { obj->GetPosition().x, obj->GetPosition().y, obj->GetPosition().z, 0.0f };
		ImGui::Text("Position:");
		ImGui::InputFloat3("pos", Object_pos);
		Vector3 pos = { Object_pos[0],Object_pos[1],Object_pos[2] };
		obj->SetPosition(pos);

		float Object_scl[4] = { obj->GetScale().x, obj->GetScale().y, obj->GetScale().z, 0.0f };
		ImGui::Text("Scale:");
		ImGui::InputFloat3("scl", Object_scl);
		Vector3 scl = { Object_scl[0],Object_scl[1],Object_scl[2] };
		obj->SetScale(scl);

		float Object_rot[4] = { obj->GetRotation().x, obj->GetRotation().y, obj->GetRotation().z, 0.0f };
		ImGui::Text("Rotation:");
		ImGui::InputFloat3("rot", Object_rot);
		Vector3 rot = { Object_rot[0],Object_rot[1],Object_rot[2] };
		obj->SetRotation(rot);

		ImGui::TreePop();
		ImGui::Spacing();
	}
}

void InspectorUtil::DrawShader(GameObject* obj)
{
	if (ImGui::TreeNode("Shader"))
	{
		std::string shaderName = obj->GetShaderName();

		if (ImGui::BeginCombo("Shader", shaderName.c_str()))
		{
			for (auto& kv : ShaderPool::GetAll())
			{
				const std::string& name = kv.first;
				bool is_selected = (shaderName == name);

				if (ImGui::Selectable(name.c_str(), is_selected))
				{
					obj->SetShaderName(name);
				}

				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		ImGui::TreePop();
		ImGui::Spacing();
	}

}

void InspectorUtil::DrawCollision(GameObject* obj)
{
	if (auto shape = obj->GetComponent<Shape>())
	{
		if (ImGui::TreeNode("Collision"))
		{
			ImGui::Text(shape->GetShapeClassName().c_str());

			float Object_pos[4] = { shape->GetRelativePosition().x, shape->GetRelativePosition().y, shape->GetRelativePosition().z, 0.0f };
			ImGui::Text("Position:");
			ImGui::InputFloat3("pos", Object_pos);
			Vector3 pos = { Object_pos[0],Object_pos[1],Object_pos[2] };
			shape->SetRelativePosition(pos);

			float Object_scl[4] = { shape->GetRelativeScale().x, shape->GetRelativeScale().y, shape->GetRelativeScale().z, 0.0f };
			ImGui::Text("Scale:");
			ImGui::InputFloat3("scl", Object_scl);
			Vector3 scl = { Object_scl[0],Object_scl[1],Object_scl[2] };
			shape->SetRelativeScale(scl);

			float Object_rot[4] = { shape->GetRelativeRotation().x, shape->GetRelativeRotation().y, shape->GetRelativeRotation().z, 0.0f };
			ImGui::Text("Rotation:");
			ImGui::InputFloat3("rot", Object_rot);
			Vector3 rot = { Object_rot[0],Object_rot[1],Object_rot[2] };
			shape->SetRelativeRotation(rot);

			ImGui::TreePop();
			ImGui::Spacing();
		}
	}

}
