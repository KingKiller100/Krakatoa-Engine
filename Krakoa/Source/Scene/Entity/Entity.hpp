#pragma once

#include "ECS_UID.hpp"
#include "EntityComponentSystem.hpp"

// #include "../../Patterns/MemPooler.hpp"
#include "../../Debug/Debug.hpp"

#include <HelperMacros.hpp>
#include <Utility/String/kToString.hpp>

namespace krakoa
{	
	class Entity //: public patterns::MemPooler<Entity, 10000>
	{
		using UID = EntityUID;
		
	public:
		Entity();

		Entity(const Entity& other);
		Entity& operator=(const Entity& other);

		Entity(Entity&& other) noexcept;
		Entity& operator=(Entity&& other) noexcept;

		~Entity() noexcept;

		USE_RESULT const bool& IsSelected() const noexcept;
		void Select() noexcept;
		void Unselect() noexcept;

		USE_RESULT bool IsActive() const noexcept;
		void Activate();
		void Deactivate();

		[[nodiscard]] EntityUID GetID() const;

		void RemoveAllComponents() noexcept;

		template<typename Component, typename ...Args>
		Component& AddComponent(Args&& ...params)
		{
			KRK_ASSERT(
				!HasComponent<Component>(), // Assert a brand new component being added
				klib::kString::ToString("Attempt to add a component already a part of this entity - {0}", typeid(Component).name())
			);

			Multi_Ptr<ComponentWrapper> comp = manager->RegisterComponent<Component, Args...>(id, std::forward<Args>(params)...);
			components.insert(std::make_pair(comp->GetUID(), comp));
			return klib::ToImpl<InternalComponent<Component>>(comp).Ref();
		}

		template<typename Component>
		bool RemoveComponent() noexcept
		{
			const ComponentUID uid = manager->GetUniqueID<Component>();
			components.erase(uid);
			return manager->RemoveComponent<Component>(id);
		}

		template<typename Component>
		USE_RESULT Component& GetComponent() const
		{
			KRK_ASSERT(
				HasComponent<Component>(), // Assert component already a part of entity
				klib::ToString("Attempt to get a component not a part of this entity - {0}", typeid(Component).name())
			);

			const ComponentUID uid = manager->GetUniqueID<Component>();
			auto& comp = components.at(uid);
			return klib::ToImpl<InternalComponent<Component>>(comp).Ref();
		}

		template<typename Component>
		USE_RESULT bool HasComponent() const noexcept
		{
			return manager->HasComponent<Component>(id);
		}

		bool operator==(const Entity& other) const noexcept;
		bool operator!=(const Entity& other) const noexcept;

	private:

		std::unordered_map<ComponentUID, Multi_Ptr<ComponentWrapper>> components;
		
		UID id;
		bool selected;
		bool active;
		EntityComponentSystem* manager;
	};
}
