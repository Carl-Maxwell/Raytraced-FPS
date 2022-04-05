#pragma once

#include <string>
#include <map>

#include "Uid.h"

class Component;

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
  std::map<ComponentId, std::vector<Component*>> m_components; // TODO should this just be a vector?
  template<typename T>
  T* getComponent(ComponentId id) {
    if (m_components.count(id) == 0) {
      return nullptr;
    } else if (m_components[id].size() == 0) {
      return nullptr;
    }
    return static_cast<T*>(m_components[id][0]);
  }

  template<typename T>
  void pushComponent(T* thatComponent) {
    m_components[T::name()].push_back(thatComponent);
    thatComponent->m_entity_id = this->m_entity_id;
  }
};



