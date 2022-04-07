#pragma once

#include <vector>
#include <string>
#include <map>

#include "Uid.h"

#include "component.h"
#include "components/transform_component.h"
#include "components/sphere_component.h"

// class Component;
// class TransformComponent;
// class SphereComponent;

// I need to figure out how I'm going to pack the component data and transfer it to the fragment shader

// GL_MAX_TEXTURE_SIZE on my computer is 32'768
// log base 2 (32'768 x 32'768) = 30
// GL_MAX_TEXTURE_SIZE is a 'rough estimate.' No idea if I get 4 components at 32'768x32'768 or what.
// I think one 32'768*32'768 texture with 4 components would be about 4 gigabytes if my math is right 

// I guess there are 1 dimensional texture buffers? would that be more suitable for getting this memory into the fragment shader?

typedef std::string ComponentId;

class Entity{
public:
  Uid m_entity_id;
  std::map<ComponentId, Component*> m_components; // TODO should this just be a vector?

  TransformComponent* transform;
  SphereComponent* sphere;

  //
  // getComponent()
  //

  template<typename T>
  T& getComponent(ComponentId id) const {
    /*if (m_components.count(id) == 0) {
      Print::now("Error! cannot find component " + id + " in entity " + m_entity_id.to_english() + ".");
      auto badT = T();
      // T* realBadT = nullptr;
      return badT;
      // TODO crash?
    } /*else if (m_components[id].size() == 0) {
      Print::now("Error! cannot find component " + id + " in entity " + m_entity_id.to_english() + ".");
      T* realBadT = nullptr;
      return *realBadT;
      // TODO crash?
    } */

    return *(T*)m_components.find(id)->second;
  }

  template<>
  TransformComponent& getComponent<TransformComponent>(ComponentId id) const {
    return *transform;
  }

  template<>
  SphereComponent& getComponent<SphereComponent>(ComponentId id) const {
    return *sphere;
  }

  //
  // pushComponent()
  //

  template<typename T>
  void pushComponent(T* thatComponent) {
    m_components[T::name()] = thatComponent;
    thatComponent->m_entity_id = this->m_entity_id;
  }

  template<>
  void pushComponent<TransformComponent>(TransformComponent* thatComponent) {
    m_components["TransformComponent"] = (Component*)thatComponent;
    thatComponent->m_entity_id = this->m_entity_id;

    transform = thatComponent;
  }

  template<>
  void pushComponent<SphereComponent>(SphereComponent* thatComponent) {
    m_components["SphereComponent"] = (Component*)thatComponent;
    thatComponent->m_entity_id = this->m_entity_id;

    sphere = thatComponent;
  }
};



