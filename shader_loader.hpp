#pragma once

#include <GL/glew.h>

#include <fstream>
#include <iostream>

namespace detail {

class ShaderLoader {

  enum class ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Framgent = GL_FRAGMENT_SHADER,
  };

  public:
    ShaderLoader(const char* vertex_shader, const char* fragment_shader) {
      auto vertex_src = LoadShaderSrc(vertex_shader);
      auto id1 = CompileShader(vertex_src, ShaderType::Vertex);

      auto frag_src = LoadShaderSrc(fragment_shader);
      auto id2 = CompileShader(frag_src, ShaderType::Framgent);

      prog_id_ = CompileProgram(id1, id2);

      // TODO: delete shaders
    }

    int GetHandle() const {
      return prog_id_;
    }

  private:
    int CompileProgram(const int id1, const int id2) {
      uint32_t prog_id = glCreateProgram();

      glAttachShader(prog_id, id1);
      glAttachShader(prog_id, id2);
      glLinkProgram(prog_id);

      // TODO: check for linking errors

      return prog_id;
    }
    
    int CompileShader(const std::string& vertex_src, ShaderType type) const {
      uint32_t shader_id = glCreateShader((uint)type);

      const char* c_str = vertex_src.c_str();
      glShaderSource(shader_id, 1, &c_str, NULL);
      glCompileShader(shader_id);

      // TODO: check for compilation errors

      return shader_id;
    }

    std::string LoadShaderSrc(const char* shader_filename) const {
      std::ifstream filestream(shader_filename);

      return std::string{
          (std::istreambuf_iterator<char>(filestream)),
          std::istreambuf_iterator<char>()};
    }

  private:
    int prog_id_ = 0;
};

}

class Shader {
  public:
    Shader(const char* vertex_shader, const char* fragment_shader) {
      detail::ShaderLoader loader(vertex_shader, fragment_shader);
      program_id_ = loader.GetHandle();
    }

    void UseProgram() {
      glUseProgram(program_id_);
    }

    void SetVariable(const char* var, int value) {
      auto loc = GetUniLocation(var);
      glUniform1i(loc, value);
    }

    void SetVariable(const char* var, float value) {
      auto loc = GetUniLocation(var);
      glUniform1f(loc, value);
    }

  private:
    int GetUniLocation(const char* var) const {
      return glGetUniformLocation(program_id_, var);
    }

  private:
    int program_id_ = 0;
};
