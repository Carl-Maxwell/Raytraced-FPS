
#include <GL/glew.h>

#include "renderer.h"

void Renderer::Clear() const {
  glClear(GL_COLOR_BUFFER_BIT);
}


void Renderer::Draw(const Shader& shader) const {
  shader.Bind();
  glDrawArrays(GL_TRIANGLES, 0, 6);
}