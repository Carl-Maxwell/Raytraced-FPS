
#include "mmath.h"

#include "actor.h"

Actor Actor::makeSphere(m::vec3 position, f32 radius) {
  Actor sphere(position, PrimitiveType::sphere);
  sphere.m_data.sphere.radius = radius;
  return sphere;
}

Actor Actor::makeBillboard(m::vec3 position, m::vec3 size, m::vec3 target) {
  Actor billboard(position, PrimitiveType::billboard);

  billboard.m_data.billboard.size   = size;
  billboard.m_data.billboard.target = target;

  return billboard;
}

