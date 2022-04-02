
#include <iostream>
#include <string>

#include "texture.h"


#ifndef __glew_h__
#include <GL/glew.h>
#endif

#include "stb_image_namespace.h"


Texture::Texture(const std::string& path)
  : rendererId(0),
  filePath(path),
  pixels(nullptr),
  width(0),
  height(0),
  bytesPerPixel(0)
{
  stb::stbi_set_flip_vertically_on_load(true);
  pixels = stb::stbi_load(path.c_str(), &width, &height, &bytesPerPixel, 4);

  std::cout << path << " ,"
    << " x: " << width
    << " y: " << height
    << " n: " << bytesPerPixel
    << "\n";

  glGenTextures(1, &rendererId);
  glBindTexture(GL_TEXTURE_2D, rendererId);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (pixels)
    stb::stbi_image_free(pixels);
}

Texture::~Texture() {
  glDeleteTextures(1, &rendererId);
}

void Texture::bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, rendererId);
}

void Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
}
