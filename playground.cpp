#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float data_buf[] = {
  0.5f, 0.5f, .2f,
  1.f, 0.f, 0.f,  0.f,
  0.f, 0.f,

  0.5f, -0.5f, .3f,
  1.f, 1.f, 0.f,  0.f,
  1.f, 1.f,

  0.f, 0.f, 0.9f,
  0.f, 0.f, 1.f,  2.f,
  1.f, 0.f,

  -0.5f, 0.5f, 0.f,
  1.f, 0.f, 0.f,  0.f,
  1.f, 1.f,

  -0.5f, -0.5f, 0.f,
  1.f, 0.f, 0.f,  0.f,
  1.f, 1.f,
};

uint32_t index_array[] = {
  0, 1, 2,
  2, 3, 4,
};

char const* vertex_shader =
"#version 330 core\n"

"layout (location=0) in vec3 position;\n"
"layout (location=1) in vec3 color;\n"
"layout (location=2) in float flag;\n"
"layout (location=3) in vec2 tex_coord;\n"

"out vec3 in_color;\n"
"out vec2 in_tex_coord;\n"
"out float in_flag;\n"

"uniform float offset;\n"

"void main()\n"
"{\n"
"  gl_Position = vec4(offset / 2 + position.x, position.yz, 1.0);\n"
"  in_color = vec3(offset / 2 + position.x / 2 + 0.5, position.y / 2 + 0.5, position.z);\n"
"  in_flag = flag;\n"
"  in_tex_coord = tex_coord;\n"
"}\n";

char const* fragment_shader2 =
"#version 330 core\n"

"out vec4 color;\n"
"in vec2 in_tex_coord;\n"

"uniform sampler2D sampler;\n"

"void main()\n"
"{\n"
"  color = texture(sampler, in_tex_coord);\n"
"}\n";

char const* fragment_shader =
"#version 330 core\n"

"out vec3 color;\n"

"in vec3 in_color;\n"
"in float in_flag;\n"

"uniform vec3 our_color;\n"

"void main()\n"
"{\n"
"  if (in_flag > 1.0)\n"
"    color = our_color;\n"
"  else \n"
"    color = in_color;\n"
"}\n";

int main( void )
{
  // Initialise GLFW
  if( !glfwInit() ) { return -1; }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow( 1024, 768, "Playground", NULL, NULL);
  glfwMakeContextCurrent(window);

  glewExperimental = true; // Needed for core profile
  // Initialize GLEW
  glewInit();

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // Dark blue background
  glClearColor(0.f, 0.f, 0.4f, 0.f);








  int width, height, nrChannels;
  unsigned char *data = stbi_load("container.jpg", &width, &height,
      &nrChannels, 0);






  unsigned int texture = 0;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);


  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);






  stbi_image_free(data);







  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);






  // Create vertex buffer object and buffer our data into it

  // This prepares videocard for data transfer of something
  GLuint vbo;
  glGenBuffers(1, &vbo);
  // This sets global gl_array_buffer to this vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  // Buffer data for static drawing (reading many times)
  glBufferData(GL_ARRAY_BUFFER, sizeof(data_buf), data_buf, GL_STATIC_DRAW);






  // Buffer indexes : do i need this?
  uint32_t ebo = 0;
  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array,
      GL_STATIC_DRAW);






  // Set vao to point into buffer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, NULL);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(12));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(24));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(28));
  glEnableVertexAttribArray(3);





  uint32_t id = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(id, 1, &vertex_shader, NULL);
  glCompileShader(id);

  uint32_t id2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(id2, 1, &fragment_shader, NULL);
  glCompileShader(id2);

  uint32_t id3 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(id3, 1, &fragment_shader2, NULL);
  glCompileShader(id3);






  uint32_t prog_id = glCreateProgram();
  glAttachShader(prog_id, id);
  glAttachShader(prog_id, id2);
  glLinkProgram(prog_id);


  uint32_t prog_id2 = glCreateProgram();
  glAttachShader(prog_id2, id);
  glAttachShader(prog_id2, id3);
  glLinkProgram(prog_id2);








  int vertexColorLocation = glGetUniformLocation(prog_id, "our_color");
  int offset_loc = glGetUniformLocation(prog_id, "offset");








  do {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(prog_id);




    // update shader uniform
    double timeValue = glfwGetTime();
    float greenValue = static_cast<float>(sin(timeValue) / 4.0 + 0.5);
    glUniform3f(vertexColorLocation, 1.f - greenValue, greenValue, 0.f);
    glUniform1f(offset_loc, greenValue);



    glBindTexture(GL_TEXTURE_2D, texture);




    glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
        NULL, 0);







    glUseProgram(prog_id2);

    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawElementsBaseVertex(GL_TRIANGLES, 3, GL_UNSIGNED_INT,
        NULL, 2);



    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
       glfwWindowShouldClose(window) == 0 );






  // Close OpenGL window and terminate GLFW
  glfwTerminate();

  return 0;
}

