﻿#pragma once
#include "Entity/ECS_UID.hpp"

#include <Utility/Enum/kEnum.hpp>

#include <map>

namespace krakoa::scene
{
	ENUM_STRUCT_FWD_DCL(SceneRuntimeState);

	namespace ecs
	{
		class Entity;
	}
	
	class iScene
	{
	public:
		using EntityMap = std::map<std::string, ecs::Entity>;
		
	public:
		virtual ~iScene() = default;

		virtual ecs::Entity& AddEntity(const std::string& name) = 0;
		
		virtual const EntityMap& GetEntities() const = 0;
		
		virtual const ecs::Entity& GetEntity(const std::string& name) const = 0;
		virtual const ecs::Entity& GetEntity(ecs::EntityUID id) const = 0;

		virtual bool HasEntity(const std::string& name) const = 0;
		virtual bool HasEntity(const ecs::EntityUID eid) const = 0;

		virtual bool RemoveEntity(const std::string& name) = 0;
		virtual bool RemoveEntity(const ecs::Entity& entity) = 0;

		virtual void OnLoad() = 0;
		virtual void OnUpdate(float time) = 0;

		virtual SceneRuntimeState GetRuntimeState() const = 0;
		virtual void SetRuntimeState(SceneRuntimeState* state) = 0;
	};
}
