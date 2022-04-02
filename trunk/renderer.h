#pragma once

// #include <>

#include "application.h"
#include "texture.h"
#include "shader.h"
#include "renderer.h"
#include "dlog.h"

class Renderer{
public:
  static Renderer& the() {
    static Renderer* my_renderer = nullptr;
    if (!my_renderer) {
      my_renderer = new Renderer;
    }
    return *my_renderer;
  };
  void Clear() const;
  void Draw(const Shader& shader) const;
};