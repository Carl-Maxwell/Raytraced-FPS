#pragma once

#include <string>

typedef std::string ComponentId;

class Component{
public:
  Uid m_id;
  Uid m_entity_id;

  static ComponentId name() { return "generic component"; }
};