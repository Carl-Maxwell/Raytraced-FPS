
#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

// namespace std{
//   class string;
// }

class Texture{
public:
  unsigned int rendererId;
  std::string filePath;
  unsigned char* pixels;
  int width, height, bytesPerPixel;

  Texture(const std::string& path);
  ~Texture();

  void bind(unsigned int slot = 0) const;
  void unbind();

  inline int getWidth()  const { return width; };
  inline int getHeight() const { return height; };
};
#endif