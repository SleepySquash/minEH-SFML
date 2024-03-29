//
//  EntitySystem.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef EntitySystem_hpp
#define EntitySystem_hpp

#include <iostream>
#include <list>
using std::list;

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Settings.hpp"
#include "MessageHolder.hpp"

#undef SendMessage
namespace ns
{
    struct Component;
    struct Entity;
    struct EntitySystem;
    
    struct Component : MessageSender
    {
        Entity* entity{ nullptr };
        bool offline{ false }, deleteme{ true }, sleep{ false };
        int priority{ 0 };
        
        virtual ~Component();
        virtual void Init();
        virtual void Update(const sf::Time&);
        virtual void FixedUpdate(const unsigned int&);
        virtual void Draw(sf::RenderWindow*);
        virtual void Resize(const unsigned int& width, const unsigned int& height);
        virtual void PollEvent(sf::Event& event);
        virtual void ReceiveMessage(MessageHolder& message);
        virtual void Destroy();
        void SendMessage(MessageHolder message) override;
        void SetPriority(int priority);
        void SetEntity(Entity* entity);
        Entity* GetEntity();
    };
    
    struct Entity : MessageSender
    {
        EntitySystem* system{ nullptr };
        bool offline{ false };
        list<Component*> components;
        
        Entity();
        Entity(EntitySystem* system);
        void Update(const sf::Time& elapsedTime);
        void FixedUpdate(const unsigned int& elapsedTime);
        void Draw(sf::RenderWindow* window);
        void Resize(const unsigned int& width, const unsigned int& height);
        void PollEvent(sf::Event& event);
        void PopComponent(Component* component);
        void SendMessage(MessageHolder message);
        void ReceiveMessage(MessageHolder& message);
        void Destroy();
        void SetEntitySystem(EntitySystem* system);
        void SortAbove(Component* component);
        template<typename T, typename ...Args> T* AddComponent(Args... args)
        {
            T* component = new T(args...);
            components.push_back(component);
            
            component->entity = this;
            component->Init();
            component->Resize(gs::width, gs::height);
            
            return component;
        }
        template<typename T, typename ...Args> T* InsertComponent(const list<Component*>::iterator& it, Args... args)
        {
            T* component = new T(args...);
            components.insert(it, component);
            
            component->entity = this;
            component->Init();
            component->Resize(gs::width, gs::height);
            
            return component;
        }
        template<typename T, typename ...Args> T* PrioritizeComponent(int priority, Args... args)
        {
            T* component = new T(args...);
            component->priority = priority;
            
            bool done{ false };
            for (list<Component*>::iterator it = components.begin(); it != components.end() && !done; ++it)
                if ((*it)->priority > priority)
                {
                    components.insert(it, component);
                    done = true;
                }
            if (!done)
                components.push_back(component);
            
            component->entity = this;
            component->Init();
            component->Resize(gs::width, gs::height);
            
            return component;
        }
    };
    
    struct EntitySystem : MessageSender
    {
        list<Entity*> entities;
        
        EntitySystem();
        void Update(const sf::Time& elapsedTime);
        void FixedUpdate(const unsigned int& elapsedTime);
        void Draw(sf::RenderWindow* window);
        void Resize(const unsigned int& width, const unsigned int& height);
        void PollEvent(sf::Event& event);
        Entity* AddEntity();
        void PopEntity(Entity* entity);
        void SendMessage(MessageHolder message);
        void clear();
    };
}

#endif /* EntitySystem_hpp */
