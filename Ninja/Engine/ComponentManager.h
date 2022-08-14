/*--------------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: ComponentManager.h
Project: PROJECT
Author: Kevin Wright
Creation date: 2/16/2021
-----------------------------------------------------------------*/
#pragma once
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <memory>
#include "Component.h"
#include <iostream>


namespace PROJECT {
    class ComponentManager {
    public:
        ~ComponentManager() {
            Clear();
        }
        void UpdateAll(double dt) {
            for (Component* component : components) {
                component->Update(dt);
            }
        }

        void AddComponent(Component* component) {
            components.push_back(component);
        }

        template<typename T>
        T* GetComponent() {
            //if (components.size() > 20) {
            //    components.resize(20);
            //    std::cout << "deleting" << std::endl;
            //    // throw std::runtime_error("Wait! there is something in components.. It has " + std::to_string(components.size()) + " components..!");
            //}

            for (Component* component : components) {
                T* ptr = dynamic_cast<T*>(component); 
                if (ptr != nullptr) {
                    return ptr;
                }
            }
            return nullptr;
        }

        template<typename T>
        void RemoveComponent() {
            auto it = std::find_if(components.begin(), components.end(), [](Component* element) {
                return (dynamic_cast<T*>(element) != nullptr);
                });
            delete* it;
            components.erase(it);
        }
        void Clear() {
            for (Component* component : components) {
                    delete component;
            }
            components.clear();
        }
    private:
        std::vector<Component*> components;
    };
}