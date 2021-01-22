﻿#include "Precompile.hpp"
#include "SceneManager.hpp"

#include "Scene.hpp"

#include "../Graphics/2D/Renderer2D.hpp"

#include "Entity/Components/CameraComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"

namespace krakoa::scene
{
	using namespace ecs::components;

	SceneManager::SceneManager()
		: entityComponentSystem(new ecs::EntityComponentSystem())
		, runtimeState(SceneRuntimeState::STOP)
	{}

	SceneManager::~SceneManager()
	{
		RemoveAll();
		entityComponentSystem->RemoveAllEntities();
	}

	void SceneManager::Add(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		auto* scene = new Scene(name, entityComponentSystem);
		scenes[name.data()].reset(scene);
		scene->SetRuntimeState(std::addressof(runtimeState));
		currentScene = name;
		scene->OnLoad();
	}

	bool SceneManager::Remove(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = scenes.find(name.data());
		if (iter == scenes.end())
			return false;

		scenes.erase(iter);
		return true;
	}

	void SceneManager::RemoveAll()
	{
		KRK_PROFILE_FUNCTION();
		scenes.clear();
	}

	SceneRuntimeState SceneManager::GetRuntimeState() const
	{
		return runtimeState;
	}

	void SceneManager::SetRuntimeState(SceneRuntimeState state)
	{
		runtimeState = state;
	}

	iScene& SceneManager::GetCurrentScene()
	{
		KRK_PROFILE_FUNCTION();
		return *scenes.at(currentScene);
	}

	void SceneManager::LoadFromFile(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
	}

	void SceneManager::OnUpdate(const float deltaTime)
	{
		KRK_PROFILE_FUNCTION();
		auto& scene = GetCurrentScene();

		scene.OnUpdate(deltaTime);

		RenderEntities(scene);
	}

	void SceneManager::RenderEntities(const iScene& scene) const
	{
		KRK_PROFILE_FUNCTION();

		iCamera* camera = nullptr;
		kmaths::TransformMatrix<float> cameraTransform = kmaths::GetTransformIdentity<float>();

		const auto cameraEntities
			= entityComponentSystem->GetEntitiesWithComponents<CameraComponent, TransformComponent>();

		for (const auto id : cameraEntities)
		{
			const auto& entity = scene.GetEntity(id);

			const auto& cam = entity.GetComponent<CameraComponent>();
			auto& tfm = entity.GetComponent<TransformComponent>();

			if (!cam.IsPrimary())
				continue;

			camera = std::addressof(cam.GetCamera());
			cameraTransform = tfm.GetTransformationMatrix2D();
			break;
		}

		if (!camera)
			return;

		const auto drawables
			= entityComponentSystem->GetEntitiesWithComponents<Appearance2DComponent, TransformComponent>();

		graphics::Renderer2D::BeginScene(*camera, cameraTransform);

		for (const auto id : drawables)
		{
			const auto& entity = scene.GetEntity(id);

			const auto& appearance = entity.GetComponent<Appearance2DComponent>();
			const auto& transform = entity.GetComponent<TransformComponent>();

			switch (appearance.GetGeometryType()) {
			case graphics::GeometryType::QUAD:
				graphics::Renderer2D::DrawQuad(appearance, transform);
				break;

			case graphics::GeometryType::TRIANGLE:
				graphics::Renderer2D::DrawTriangle(appearance, transform);
				break;

			case graphics::GeometryType::CIRCLE:
				//	graphics::Renderer2D::DrawCircle(appearance, transform);
				break;

			default: // case of an unknown geometry type
				KRK_FATAL("Failed to draw unknown geometry type");
				break;
			}
		}

		graphics::Renderer2D::EndScene();
	}
}
