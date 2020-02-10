//
//  EntitySystem.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "EntitySystem.hpp"

namespace ns
{
    Component::~Component() { }
    void Component::Init() { }
    void Component::Update(const sf::Time&) { }
    void Component::Draw(sf::RenderWindow*) { }
    void Component::Resize(const unsigned int&, const unsigned int&) { }
    void Component::PollEvent(sf::Event&) { }
    void Component::ReceiveMessage(MessageHolder&) { }
    void Component::Destroy() { }
    void Component::SendMessage(MessageHolder message) { ReceiveMessage(message); }
    void Component::SetEntity(Entity* entity) { this->entity = entity; }
    Entity* Component::GetEntity() { return entity; }
    void Component::SetPriority(int priority) { this->priority = priority; }
    
    
    Entity::Entity() { }
    Entity::Entity(EntitySystem* system) { this->system = system; }
    void Entity::Update(const sf::Time& elapsedTime)
    {
        list<Component*>::iterator it = components.begin();
        while (it != components.end())
        {
            if ((*it)->offline) { if ((*it)->sleep) (*it)->sleep = (*it)->offline = false; else { delete (*it); it = components.erase(it); } }
            else { (*it)->Update(elapsedTime); ++it; }
        }
    }
    void Entity::Draw(sf::RenderWindow* window)
    {
        if (components.size())
            for (auto c : components)
                if (!c->offline) c->Draw(window);
    }
    void Entity::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (components.size())
            for (auto c : components)
                if (!c->offline) c->Resize(width, height);
    }
    void Entity::PollEvent(sf::Event& event)
    {
        if (components.size())
        {
            auto next = components.rbegin();
            for (auto it = components.rbegin(); it != components.rend(); it = next)
            {
                std::advance(next, 1);
                if (!(*it)->offline) (*it)->PollEvent(event);
            }
        }
    }
    void Entity::PopComponent(Component* component)
    {
        if (component->offline) return;
        component->offline = true;
        component->Destroy();
    }
    void Entity::Destroy()
    {
        list<Component*>::iterator it = components.begin();
        while (it != components.end()) { (*it)->Destroy(); delete (*it); it = components.erase(it); }
    }
    void Entity::SendMessage(MessageHolder message) { Entity::ReceiveMessage(message); }
    void Entity::ReceiveMessage(MessageHolder& message)
    {
        if (components.size())
        {
            list<Component*>::iterator next;
            for (auto e = components.begin(); e != components.end(); e = next)
            {
                next = e; std::advance(next, 1);
                if (!(*e)->offline) (*e)->ReceiveMessage(message);
            }
        }
    }
    void Entity::SetEntitySystem(EntitySystem* system) { this->system = system; }
    void Entity::SortAbove(Component* component)
    {
        if (components.back() != component)
        {
            component->sleep = component->offline = true;
            components.erase(std::find(components.begin(), components.end(), component));
            components.push_back(component);
        }
    }
    
    
    
    EntitySystem::EntitySystem() { }
    void EntitySystem::Update(const sf::Time& elapsedTime)
    {
        list<Entity*>::iterator it = entities.begin();
        while (it != entities.end())
        {
            if ((*it)->offline) { delete (*it); it = entities.erase(it); }
            else { (*it)->Update(elapsedTime); ++it; }
        }
    }
    void EntitySystem::Draw(sf::RenderWindow* window)
    {
        if (entities.size())
            for (auto e : entities)
                if (!e->offline) e->Draw(window);
    }
    void EntitySystem::Resize(const unsigned int& width, const unsigned int& height)
    {
        if (entities.size())
            for (auto e : entities)
                if (!e->offline) e->Resize(width, height);
    }
    void EntitySystem::PollEvent(sf::Event& event)
    {
        if (entities.size())
            for (auto it = entities.rbegin(); it != entities.rend(); ++it)
                if (!(*it)->offline) (*it)->PollEvent(event);
    }
    Entity* EntitySystem::AddEntity()
    {
        Entity* entity = new Entity(this);
        entities.push_back(std::move(entity));
        return entities.back();
    }
    void EntitySystem::PopEntity(Entity* entity)
    {
        entity->offline = true;
        entity->Destroy();
        /*std::list<Entity*>::iterator it = std::find(entities.begin(), entities.end(), entity);
         if (it != entities.end())
         {
         (*it)->Destroy();
         delete (*it);
         entities.erase(it);
         }*/
    }
    void EntitySystem::SendMessage(MessageHolder message)
    {
        if (entities.size())
            for (auto e : entities)
                if (!e->offline) e->ReceiveMessage(message);
    }
    void EntitySystem::clear()
    {
        list<Entity*>::iterator it = entities.begin();
        while (it != entities.end()) { (*it)->Destroy(); delete (*it); it = entities.erase(it); }
        entities.clear();
    }
}
