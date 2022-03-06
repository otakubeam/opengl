#include <stdio.h>
#include <linux/input.h>
#include <stdlib.h>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
GLFWwindow* window;

#include <glm/glm.hpp>
using namespace glm;

float data_buf[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,

	0.0f, 0.0f, -1.2f,

	-0.5f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
};

uint32_t index_array[] = {
	1, 2, 3,
	3, 4, 5,
};

char const* vertex_shader =
"#version 330 core\n"
"layout (location=0) in vec3 position;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(position.xyz, 1.0);\n"
"}\n";

char const* fragment_shader =
"#version 330 core\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"	color = vec3(1.0, 1.0, 0.0);\n"
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
	// Initialize GLEW
	
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);




	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


  printf("%d", glGetError());




	// Create vertex buffer object and buffer our data into it

	// This prepares videocard for data transfer of something
	GLuint vbo;
	glGenBuffers(1, &vbo);
	// This sets global gl_array_buffer to this vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Buffer data for static drawing (reading many times)
	glBufferData(GL_ARRAY_BUFFER, sizeof (data_buf), data_buf, GL_STATIC_DRAW);



	// Set vao to point into buffer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, NULL);
	glEnableVertexAttribArray(0);


  printf("%d", glGetError());



	// Buffer indexes : do i need this?
	uint32_t ebo = 0;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_array), index_array, GL_STATIC_DRAW);




  printf("%d", glGetError());





  printf("%d", glGetError());




	uint32_t id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(id, 1, &vertex_shader, NULL);
	glCompileShader(id);

	uint32_t id2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(id2, 1, &fragment_shader, NULL);
	glCompileShader(id2);
	

	

  printf("%d", glGetError());



	uint32_t prog_id = glCreateProgram();
	glAttachShader(prog_id, id);
	glAttachShader(prog_id, id2);
	glLinkProgram(prog_id);




  printf("%d", glGetError());


	do {
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(prog_id);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    printf("%d", glGetError());

		glBindVertexArray(0);

    printf("%d", glGetError());

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	} 
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );



  printf("%d", glGetError());




	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

