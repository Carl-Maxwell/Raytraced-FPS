#pragma once

#include <string>
#include <unordered_map>

#include "mtypedefs.h"
#include "mvec3.h"
#include "mmath.h"

// struct for returning two strings from ParseShader
struct shaderProgramSource{
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader{
public:
  u32 m_rendererId;
  std::string m_filepath;
  std::unordered_map<std::string, i32> m_uniformLocationCache;

  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  u32 CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
  u32 CompileShader(u32 type, const std::string& strSource);
  shaderProgramSource ParseShader();

  // Set uniforms
  void SetUniform1i(const std::string& name, i32  v);
  void SetUniform2i(const std::string& name, i32* v);
  void SetUniform3i(const std::string& name, i32* v);
  void SetUniform4i(const std::string& name, i32* v);

  void SetUniform1uiv(const std::string& name, i32 count, const u32* v);

  void SetUniform1ui(const std::string& name, i32* v);
  void SetUniform2ui(const std::string& name, i32* v);
  void SetUniform3ui(const std::string& name, i32* v);
  void SetUniform4ui(const std::string& name, i32* v);

  void SetUniform1f(const std::string& name, f32  v);
  void SetUniform2f(const std::string& name, f32  v0, f32 v1);

  void SetUniform1f(const std::string& name, f32* v);
  void SetUniform2f(const std::string& name, f32* v);
  void SetUniform3f(const std::string& name, m::vec3 v);
  void SetUniform4f(const std::string& name, m::vec4 v);

  void SetUniform(const std::string& name, i32 count, m::vec3* v);
  void SetUniform(const std::string& name, i32 count, m::vec4* v);

  i32 GetUniformLocation(const std::string& name);
  
};
