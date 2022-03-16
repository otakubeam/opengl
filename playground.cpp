#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>

float data_buf[] = {
  0.0f, 0.0f, 0.0f,
  0.0f, 0.5f, 0.0f,
  0.5f, 0.0f, 0.0f,
  0.0f, 0.0f, 0.5f,
};

uint32_t index_array[] = {
  0, 1, 2,
  0, 1, 3,
  0, 2, 3,
  1, 2, 3,
};

char const* vertex_shader =
"#version 330 core\n"
"layout (location=0) in vec3 position;\n"
"uniform mat4 MVP;\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(position.xyz, 1.0);\n"
"}\n";

char const* fragment_shader =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(1.0, 1.0, 0.0, 0.5);\n"
"}\n";

char const* fragment_shader2 =
"#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(1.0, 0.0, 0.0, 0.5);\n"
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
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);




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
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);









  // Set vao to point into buffer
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
  glEnableVertexAttribArray(0);







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








  // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
  glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1024.f / 768.f, 0.1f, 100.0f);

  // Or, for an ortho camera :
  //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

  // Camera matrix
  glm::mat4 View = glm::lookAt(
      glm::vec3(2,3,3), // Camera is at (4,3,3), in World Space
      glm::vec3(0,0,0), // and looks at the origin
      glm::vec3(0,+1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  // Model matrix : an identity matrix (model will be at the origin)
  glm::mat4 Model = glm::mat4(1.0f);
  // Our ModelViewProjection : multiplication of our 3 matrices
  glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around









  // Get a handle for our "MVP" uniform
  // Only during the initialisation
  GLuint MatrixID = glGetUniformLocation(prog_id, "MVP");


  MatrixID = glGetUniformLocation(prog_id2, "MVP");





  do {
    glClear(GL_COLOR_BUFFER_BIT);




    double timeValue = glfwGetTime();
    float greenValue = static_cast<float>(sin(timeValue)) ;
    float redValue = static_cast<float>(cos(timeValue)) ;





    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(1.5, greenValue * 1.2 + redValue, greenValue * redValue), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,+1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around




    glUseProgram(prog_id2);
    // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glDrawElementsBaseVertex(GL_TRIANGLES, 1, GL_UNSIGNED_INT,
        NULL, 0);






    glUseProgram(prog_id);
    // glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glDrawElementsBaseVertex(GL_TRIANGLES, 1, GL_UNSIGNED_INT,
        NULL, 1);




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

