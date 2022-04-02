#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include <cstddef>

#include "mvec3.h"
#include "mmath.h"
// #include "texture.h"

class Texture;

enum class PrimitiveType{
  not_set, sphere, billboard
};

// struct Primitive{
//   PrimitiveType pType;
// };

// TODO don't put struct Data into global namespace
  // either call it ActorData or ActorModule::Data or something

// struct Data{
//   std::byte block_of_memory[512];
//   inline void* memory(i32 offset) {
//     return ( (void*)( (std::byte*)(&block_of_memory) + offset ) );
//   };
//   inline f32* to_f32(i32 offset) {
//     return (f32*)( memory(offset) );
//   };
//   inline m::vec3* to_vec3(i32 offset) {
//     return (m::vec3*)( memory(offset) );
//   };
// };

class Actor{
public:
  PrimitiveType m_pType;
  m::vec3 m_position;
  // Data m_data;

  union ActorData{
    struct Generic{

    };
    struct Sphere{
      f32 radius;
    } sphere;
    struct Billboard {
      Texture* sprite;
      m::vec3 size;
      m::vec3 target;
    } billboard;
  } m_data;

  Actor() : m_pType(PrimitiveType::not_set), m_data{} {};
  Actor(PrimitiveType pType) : m_pType(pType), m_data{} {};

  Actor(m::vec3 pos, PrimitiveType pType)
    : m_position(pos), m_pType(pType), m_data{} {};

  static Actor makeSphere(m::vec3 position, f32 radius);
  static Actor makeBillboard(m::vec3 position, m::vec3 size, m::vec3 target);

  // accessing member data
  ActorData::Sphere*    sphere()    { return &m_data.sphere; };
  ActorData::Billboard* billboard() { return &m_data.billboard; };
};

#endif