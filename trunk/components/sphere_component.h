
#include "component.h"
#include "mtypedefs.h"

class SphereComponent : public Component{
public:
  f32 radius;

  SphereComponent(f32 thatRadius = 1.0f) : radius(thatRadius) {}

  static ComponentId name() { return "SphereComponent"; }
};