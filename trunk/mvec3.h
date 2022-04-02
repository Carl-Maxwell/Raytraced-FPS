#pragma once

// this file provides some structs (vec2, vec3, vec4)
// they're just thin wrappers around the linmath.h header functions.

namespace linmath{
  #include "linmath.h"

  // linmath has a typedef for vec3, but I'd rather have a struct with overloaded operators and whatnot
}

namespace m{

struct vec3{
  // float arr[3];
  float x;
  float y;
  float z;

  vec3() : x(0), y(0), z(0) {};
  vec3(float l_x, float l_y, float l_z) : x(l_x), y(l_y), z(l_z) {};

  inline float* to_array(int i = 0) {
    return ((float*)this+i);
  }

  //
  // Operator overloading
  //

  inline const float operator[](int const i) const{
    return *((float*)this+i);
  }
  vec3& operator=(vec3 const &other) {
    for (int i = 0; i < 3; i++) {
      *((float*)this+i) = other[i];
    }
    return *this;
  }
  vec3 const operator+(vec3 &b) {
    vec3 temp;
    linmath::vec3_add(temp.to_array(), to_array(), b.to_array());
    return temp;
  }
  vec3 operator-(vec3 &sub) {
    vec3 temp;
    linmath::vec3_sub(temp.to_array(), to_array(), sub.to_array());
    return temp;
  }
  vec3 operator*(float const &scalar) {
    vec3 temp;
    linmath::vec3_scale(temp.to_array(), to_array(), scalar);
    return temp;
  }
  vec3 operator*(vec3 const &other) {
    vec3 temp;
    for (int i = 0; i < 3; i++) {
      *temp.to_array(i) = *to_array(i) * other[i];
    }
    return temp;
  }
  vec3 operator/(float const &scalar) {
    vec3 temp;
    for (int i = 0; i < 3; i++) {
      *temp.to_array(i) = *to_array(i) / scalar;
    }
    return temp;
  }
  vec3 operator/(vec3 const &other) {
    vec3 temp;
    for (int i = 0; i < 3; i++) {
      *temp.to_array(i) = *to_array(i) / other[i];
    }
    return temp;
  }

  // vector functions

  float dot(vec3 &b) {
    return linmath::vec3_mul_inner(to_array(), b.to_array());
  }
  vec3 cross(vec3 &b) {
    vec3 temp;
    linmath::vec3_mul_cross(temp.to_array(), to_array(), b.to_array());
    return temp;
  }
  float length() {
    return linmath::vec3_len(to_array());
  }
  vec3 norm() {
    vec3 temp;
    linmath::vec3_norm(temp.to_array(), to_array());
    return temp;
  }
  vec3 min(vec3 &b) {
    vec3 temp;
    linmath::vec3_min(temp.to_array(), to_array(), b.to_array());
    return temp;
  }
  vec3 max(vec3 &b) {
    vec3 temp;
    linmath::vec3_max(temp.to_array(), to_array(), b.to_array());
    return temp;
  }

  // TODO there's a reflect function in linmath

  // TODO need a rotate vector function
  // TODO add euler?

};


struct vec4{
  float x;
  float y;
  float z;
  float w;

  // returns pointer to float[4] or float[i]

  inline float* to_array(int i = 0) {
    return ((float*)this+i);
  }
  inline vec3 to_vec3() {
    return vec3{x, y, z};
  }

  //
  // Operator overloading
  //

  inline const float operator[](int const i) const{
    return *((float*)this+i);
  }
  vec4& operator=(vec4 const &other) {
    for (int i = 0; i < 4; i++) {
      *((float*)this+i) = other[i];
    }
    return *this;
  }
  vec4& operator=(vec3 const &other) {
    for (int i = 0; i < 3; i++) {
      *((float*)this+i) = other[i];
    }
    return *this;
  }
  vec4 const operator+(vec4 &b) {
    vec4 temp;
    linmath::vec4_add(temp.to_array(), to_array(), b.to_array());
    return temp;
  }
  vec4 operator-(vec4 &sub) {
    vec4 temp;
    linmath::vec4_sub(temp.to_array(), to_array(), sub.to_array());
    return temp;
  }
  vec4 operator*(float const &scalar) {
    vec4 temp;
    linmath::vec4_scale(temp.to_array(), to_array(), scalar);
    return temp;
  }
  vec4 operator*(vec4 const &other) {
    vec4 temp;
    for (int i = 0; i < 4; i++) {
      *temp.to_array(i) = *to_array(i) * other[i];
    }
    return temp;
  }
  vec4 operator/(float const &scalar) {
    vec4 temp;
    for (int i = 0; i < 4; i++) {
      *temp.to_array(i) = *to_array(i) / scalar;
    }
    return temp;
  }
  vec4 operator/(vec4 const &other) {
    vec4 temp;
    for (int i = 0; i < 4; i++) {
      *temp.to_array(i) = *to_array(i) / other[i];
    }
    return temp;
  }

  // vector functions

  float dot(vec4 &b) {
    return linmath::vec4_mul_inner(to_array(), b.to_array());
  }
  vec4 cross(vec4 &b) {
    vec4 temp;
    linmath::vec4_mul_cross(temp.to_array(), to_array(), b.to_array());
    return temp;
  }
  float length() {
    return linmath::vec4_len(to_array());
  }
  vec4 norm() {
    vec4 temp;
    linmath::vec4_norm(temp.to_array(), to_array());
    return temp;
  }
  vec4 min(vec4 &b) {
    vec4 temp;
    linmath::vec4_min(temp.to_array(), to_array(), b.to_array());
    return temp;
  }
  vec4 max(vec4 &b) {
    vec4 temp;
    linmath::vec4_max(temp.to_array(), to_array(), b.to_array());
    return temp;
  }

  // TODO there's a reflect function in linmath

  // TODO need a rotate vector function
  // TODO add euler?

};

  // TODO vec2
  // TODO vec4
  // TODO matrices
  // TODO quaternions

}


