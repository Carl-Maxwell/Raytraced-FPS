#include "shader.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <random>
#include <map>

#include <fstream>
#include <sstream>

#include <chrono>

#define GLEW_STATIC
#include <GL/glew.h>

// Maxwell Libraries

#include "dlog.h"
#include "mvec3.h"
#include "mmath.h"

Shader::Shader(const std::string& filepath)
  : m_filepath(filepath), m_rendererId(0)
{
  shaderProgramSource source = ParseShader();
  m_rendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {
  glDeleteProgram(m_rendererId);
}

void Shader::Bind() const {
  glUseProgram(m_rendererId);
}

void Shader::Unbind() const {
  glUseProgram(0);
}

// Set uniforms
void Shader::SetUniform1i(const std::string& name, i32  v) {  glUniform1i(GetUniformLocation(name), v); }
void Shader::SetUniform2i(const std::string& name, i32 *v) {  glUniform2i(GetUniformLocation(name), v[0], v[1]); }
void Shader::SetUniform3i(const std::string& name, i32 *v) {  glUniform3i(GetUniformLocation(name), v[0], v[1], v[2]); }
void Shader::SetUniform4i(const std::string& name, i32 *v) {  glUniform4i(GetUniformLocation(name), v[0], v[1], v[2], v[3]); }

void Shader::SetUniform1uiv(const std::string& name, i32 count, const u32* v) { glUniform1uiv(GetUniformLocation(name), count, v); }

void Shader::SetUniform1ui(const std::string& name, i32* v) { glUniform1ui(GetUniformLocation(name), *v); }
void Shader::SetUniform2ui(const std::string& name, i32* v) { glUniform2ui(GetUniformLocation(name), v[0], v[1]); }
void Shader::SetUniform3ui(const std::string& name, i32* v) { glUniform3ui(GetUniformLocation(name), v[0], v[1], v[2]); }
void Shader::SetUniform4ui(const std::string& name, i32* v) { glUniform4ui(GetUniformLocation(name), v[0], v[1], v[2], v[3]); }

void Shader::SetUniform1f(const std::string& name, f32   v) { glUniform1f(GetUniformLocation(name),  v); }
void Shader::SetUniform2f(const std::string& name, f32  v0, f32 v1) { glUniform2f(GetUniformLocation(name), v0, v1); }

void Shader::SetUniform1f(const std::string& name, f32* v) { glUniform1f(GetUniformLocation(name), *v); }
void Shader::SetUniform2f(const std::string& name, f32* v) { glUniform2f(GetUniformLocation(name), v[0], v[1]); }
void Shader::SetUniform3f(const std::string& name, m::vec3 v) { glUniform3f(GetUniformLocation(name), v[0], v[1], v[2]); }
void Shader::SetUniform4f(const std::string& name, m::vec4 v) { glUniform4f(GetUniformLocation(name), v[0], v[1], v[2], v[3]); }

void Shader::SetUniform(const std::string& name, i32 count, m::vec3* v) { glUniform3fv(GetUniformLocation(name), count, v->to_array() ); } 
void Shader::SetUniform(const std::string& name, i32 count, m::vec4* v) { glUniform4fv(GetUniformLocation(name), count, v->to_array() ); } 

i32 Shader::GetUniformLocation(const std::string& name) {
  if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
    return m_uniformLocationCache[name];
  }
  i32 location = glGetUniformLocation(m_rendererId, name.c_str());
  if (location == -1) {
    Log::the().log_once("Urbarack warning:\n\tuniform: '" + name + "' doesn't exist in shader " + m_filepath + "!\n\tIs this uniform being used in the shader?");
    // __debugbreak();
  } else {
    m_uniformLocationCache[name] = location;
  }
  return location;
}


shaderProgramSource Shader::ParseShader() {
  std::ifstream stream(m_filepath);

  enum class ShaderType {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream outputStream[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line)) {
    if (line.find("#shader") != std::string::npos) {
      if (line.find("vertex") != std::string::npos)
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        type = ShaderType::FRAGMENT;
    } else {
      outputStream[(int)type] << line << "\n";
    }
  }

  return {outputStream[0].str(), outputStream[1].str()};
}

u32 Shader::CompileShader(unsigned int type, const std::string& strSource) {
  unsigned int id = glCreateShader(type);
  const char* src = strSource.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (!result) {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "Failed to compile " <<
      (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
      << " shader! \n";
    std::cout << message << "\n";

    glDeleteShader(id);
    return 0;
  }

  return id;
}


u32 Shader::CreateShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
  unsigned int program = glCreateProgram();
  unsigned int vertexShaderProgram = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
  unsigned int fragmentShaderProgram = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  glAttachShader(program, vertexShaderProgram);
  glAttachShader(program, fragmentShaderProgram);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vertexShaderProgram);
  glDeleteShader(fragmentShaderProgram);

  return program;
}