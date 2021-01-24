﻿#include "SceneHierarchyPanel.hpp"

#include <Core/Application.hpp>
#include <Debug/Instrumentor.hpp>
#include <Input/MouseButtonCode.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>
#include <Scene/Entity/Components/TransformComponent.hpp>
#include <Scene/Entity/Components/AppearanceComponent.hpp>
#include <Scene/Entity/Components/NativeScriptComponent.hpp>

#include <Util/TypeInfo.hpp>

#include <Utility/Bits/kBitTricks.hpp>

#include <ImGui/imgui.h>


namespace krakoa::panels
{
	using namespace scene::ecs::components;

	SceneHierarchyPanel::SceneHierarchyPanel()
		: selectedEntityID(0)
	{
		KRK_LOG("Editor", "Scene Hierarchy Created");
	}

	void SceneHierarchyPanel::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		const auto& sceneMan = GetApp().GetSceneManager();
		const auto& context = sceneMan.GetCurrentScene();

		{
			KRK_PROFILE_SCOPE("Entities Panel");

			ImGui::Begin("Entities");


			if (sceneMan.HasActiveScene())
			{
				std::uintptr_t index = 0;
				const auto& entities = context.GetEntities();
				for (const auto& [name, entity] : entities)
				{
					DrawEntityNode(name, entity);
				}
			}

			ImGui::End();
		}

		if (!context.HasEntity(selectedEntityID))
			return;

		{
			KRK_PROFILE_SCOPE("Properties Panel");
			ImGui::Begin("Properties");

			DrawComponentNode(context.GetEntity(selectedEntityID));

			ImGui::End();
		}
	}

	void SceneHierarchyPanel::DrawEntityNode(const std::string& name, const scene::ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		const auto eid = entity.GetID();
		const auto selected = selectedEntityID == eid ? ImGuiTreeNodeFlags_Selected : 0;
		const ImGuiTreeNodeFlags flags = selected | ImGuiTreeNodeFlags_OpenOnArrow;

		const bool opened = ImGui::TreeNodeEx((void*)eid, flags, "%s", name.data());

		if (ImGui::IsItemClicked(input::MOUSE_LEFT))
		{
			selectedEntityID = eid;
		}

		if (opened)
		{
			const ImGuiTreeNodeFlags subTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow;
			auto maskedEID = eid;
			klib::SetBit(maskedEID, (sizeof(eid) * 8) - 1);
			ImGui::Text("id: %llu", eid);
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawComponentNode(const scene::ecs::Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>();

			char buffer[1 << 8];
			std::strcpy(buffer, tag.GetData());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer) * sizeof(buffer[0])))
			{
				tag.SetTag(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)util::GetTypeHash<TransformComponent>(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>();
				const auto& position = transform.GetPosition();
				const auto& scale = transform.GetScale();
				auto rotation = kmaths::ToDegrees(transform.GetRotation());

				ImGui::DragFloat3("Position", position.GetPointerToData(), 0.05f);
				ImGui::DragFloat("Rotation", &rotation, 0.5f);
				ImGui::DragFloat3("Scale", scale.GetPointerToData(), 0.1f);

				transform.SetRotation(kmaths::ToRadians(rotation));

				ImGui::TreePop();
			}
		}
	}
}
