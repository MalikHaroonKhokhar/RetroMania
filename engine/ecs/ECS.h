#pragma once

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace Forge {

using Entity = uint32_t;
const Entity NULL_ENTITY = 0;

/**
 * @brief Base class for component arrays to allow storing them generically.
 */
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

/**
 * @brief Manages components of a specific type.
 */
template<typename T>
class ComponentArray : public IComponentArray {
public:
    void InsertData(Entity entity, T component) {
        if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) {
            throw std::runtime_error("Component added to same entity more than once.");
        }

        size_t newIndex = m_Size;
        m_EntityToIndexMap[entity] = newIndex;
        m_IndexToEntityMap[newIndex] = entity;

        if (newIndex >= m_ComponentArray.size()) {
            m_ComponentArray.push_back(component);
        } else {
            m_ComponentArray[newIndex] = component;
        }
        m_Size++;
    }

    void RemoveData(Entity entity) {
        if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end()) {
            return; // Entity doesn't have this component
        }

        size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
        size_t indexOfLastElement = m_Size - 1;

        if (indexOfRemovedEntity != indexOfLastElement) {
            // Swap with the last element
            m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement];
            Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
            m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;
        }

        m_EntityToIndexMap.erase(entity);
        m_IndexToEntityMap.erase(indexOfLastElement);
        m_Size--;
    }

    T& GetData(Entity entity) {
        if (m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end()) {
            throw std::runtime_error("Retrieving non-existent component.");
        }
        return m_ComponentArray[m_EntityToIndexMap[entity]];
    }

    bool HasData(Entity entity) const {
        return m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end();
    }

    void EntityDestroyed(Entity entity) override {
        if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end()) {
            RemoveData(entity);
        }
    }

    // Iteration support
    auto begin() { return m_ComponentArray.begin(); }
    auto end() { return m_ComponentArray.begin() + m_Size; }

    std::vector<Entity> GetEntities() const {
        std::vector<Entity> entities;
        for(size_t i = 0; i < m_Size; ++i) {
            entities.push_back(m_IndexToEntityMap.at(i));
        }
        return entities;
    }

private:
    std::vector<T> m_ComponentArray;
    std::unordered_map<Entity, size_t> m_EntityToIndexMap;
    std::unordered_map<size_t, Entity> m_IndexToEntityMap;
    size_t m_Size = 0;
};

/**
 * @brief Registry for managing entities and components.
 */
class Registry {
public:
    Entity CreateEntity() {
        return ++m_NextEntity;
    }

    void DestroyEntity(Entity entity) {
        for (auto const& pair : m_ComponentArrays) {
            pair.second->EntityDestroyed(entity);
        }
    }

    template<typename T>
    void RegisterComponent() {
        std::type_index typeName = typeid(T);
        if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end()) {
            m_ComponentArrays[typeName] = std::make_shared<ComponentArray<T>>();
        }
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    template<typename T>
    bool HasComponent(Entity entity) {
        return GetComponentArray<T>()->HasData(entity);
    }

    /**
     * @brief Simple view for iterating over entities with a specific component.
     */
    template<typename T>
    std::vector<Entity> View() {
        return GetComponentArray<T>()->GetEntities();
    }

    /**
     * @brief Simple view for iterating over entities with multiple specific components.
     */
    template<typename T1, typename T2>
    std::vector<Entity> View() {
        std::vector<Entity> result;
        auto entities = GetComponentArray<T1>()->GetEntities();
        auto array2 = GetComponentArray<T2>();

        for (Entity e : entities) {
            if (array2->HasData(e)) {
                result.push_back(e);
            }
        }
        return result;
    }

private:
    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        std::type_index typeName = typeid(T);
        if (m_ComponentArrays.find(typeName) == m_ComponentArrays.end()) {
            RegisterComponent<T>();
        }
        return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
    }

    Entity m_NextEntity = NULL_ENTITY;
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> m_ComponentArrays;
};

// Built-in components

struct Tag {
    std::string Name;
    Tag() = default;
    Tag(const std::string& name) : Name(name) {}
};

struct Transform {
    float x = 0.0f;
    float y = 0.0f;
    float rotation = 0.0f;
    float scaleX = 1.0f;
    float scaleY = 1.0f;
};

} // namespace Forge