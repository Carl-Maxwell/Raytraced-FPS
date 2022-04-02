#pragma once

#ifndef ACTOR_H
#define ACTOR_H

#include <cstddef>

#include "mvec3.h"
#include "mmath.h"
// #include "texture.h" // compile error: cstring(26,13): error C2039: 'memchr': is not a member of '`global namespace''

class Texture;

enum class PrimitiveType{
  not_set, sphere, billboard
};

class Actor{
public:
  PrimitiveType m_pType;
  m::vec3 m_position;

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