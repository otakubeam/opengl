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
  -0.500406, -0.286516, 0.286516,
  -0.500406, 0.286516, 0.286516,
  -0.500406, -0.286516, -0.286516,
  -0.500406, 0.286516, -0.286516,
  0.500406, -0.286516, 0.286516,
  0.500406, 0.286516, 0.286516,
  0.500406, -0.286516, -0.286516,
  0.500406, 0.286516, -0.286516,
};

// One color for each vertex. They were generated randomly.
static const GLfloat g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

uint32_t index_array[] = {
  0, 4, 6,
  3, 2, 6,
  7, 6, 4,
  5, 1, 3,
  1, 0, 2,
  5, 4, 0,

  6, 2, 4,
  6, 7, 2,
  4, 5, 6,
  3, 7, 1,
  2, 3, 0,
  0, 1, 4,
};

char const* vertex_shader =
"#version 330 core\n"
"layout (location=0) in vec3 position;\n"
//"layout (location=1) in vec3 color_in;\n"
//"out vec3 color_in_pass;\n"
"uniform mat4 MVP;\n"
"void main()\n"
"{\n"
//"  color_in_pass = color_in;\n"
"  gl_Position = MVP * vec4(position.xyz, 1.0);\n"
"}\n";

char const* fragment_shader =
"#version 330 core\n"
//"in vec3 color_in_pass;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"  color = vec4(1, 0, 0, 0.9);\n"
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
  window = glfwCreateWindow(900, 900, "Playground", NULL, NULL);
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


  // Enable depth test
  glEnable(GL_DEPTH_TEST);
  // Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    double timeValue = glfwGetTime()/2;
    float greenValue = static_cast<float>(sin(timeValue)) ;
    float redValue = static_cast<float>(cos(timeValue)) ;





    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(greenValue, 2, redValue), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,+1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 mvp = Projection * View * Model; // Remember, matrix multiplication is the other way around




    glUseProgram(prog_id);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glDrawElementsBaseVertex(GL_TRIANGLES, 100, GL_UNSIGNED_INT,
        NULL, 0);








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

