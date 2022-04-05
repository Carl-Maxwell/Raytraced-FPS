
#include "mvec3.h"
#include "component.h"

class TransformComponent : public Component{
public:
  m::vec3 position;
  m::vec3 rotation;
  m::vec3 scale;

  TransformComponent(m::vec3 p) : position(p) {}

  static ComponentId name() { return "TransformComponent"; }
};