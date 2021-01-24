#pragma once

#include "iScene.hpp"

#include "Entity/Entity.hpp"

#include <string>
#include <map>

namespace krakoa::scene
{
	class Scene : public iScene
	{
	public:
		Scene(const std::string_view& name, Multi_Ptr<ecs::EntityComponentSystem> ecs);
		~Scene();

		ecs::Entity& AddEntity(const std::string& entityName) override;

		const EntityMap& GetEntities() const override;
	
		[[nodiscard]] const ecs::Entity& GetEntity(const std::string& entityName) const override;
		[[nodiscard]] const ecs::Entity& GetEntity(ecs::EntityUID id) const override;

		bool HasEntity(const std::string& entityName) const override;
		bool HasEntity(const ecs::EntityUID eid) const override;
		
		bool RemoveEntity(const std::string& entityName) override;
		bool RemoveEntity(const ecs::Entity& entity) override;
		
		void OnLoad() override;
		void OnUpdate(float time) override;

		SceneRuntimeState GetRuntimeState() const override;
		void SetRuntimeState(SceneRuntimeState* state) override;

	protected:
		[[nodiscard]] ecs::Entity& GetEntity(ecs::EntityUID id);
		void UpdateScripts(float deltaTime);

	private:
		std::string name;
		EntityMap entities;
		Multi_Ptr<ecs::EntityComponentSystem> entityComponentSystem;
		SceneRuntimeState* runtimeState;
	};
}
