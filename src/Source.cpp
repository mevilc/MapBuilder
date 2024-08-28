#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <array>

// glad: loads OpenGL functions pointers
// glfw: creates OpenGL context, handles window management
// OpenGL: >v3.3 is core

static const char* vertShaderSrc = "#version 330 core\n"
								   "layout (location = 0) in vec3 aPos;\n"
								   "void main()\n"
								   "{\n"
								   "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
								   "}\n\0";

static const char* fragShaderSrc = "#version 330 core\n"
								   "out vec4 FragColor;\n"
								   "void main()\n"
								   "{\n"
								   "  FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
								   "}\n\0";


int main()
{

	// INITIALIZE /////////////////

	glfwInit();

	// Tell glfw the gl version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// loads gl functors to use
	gladLoadGL();

	// CREATE WINDOW /////////////////

	// create window in screen coordinates (pixel coordinates)
	int winW = 800, winH = 800;
	const char* winTitle = "MapBuilder";
	GLFWwindow* window = glfwCreateWindow(winW, winH, winTitle, NULL, NULL);
	if (window == nullptr)
	{
		std::cerr << "GLFW window not created!\n";
		glfwTerminate();
		return -1;
	}

	// tell glfw to make window part of current context
	glfwMakeContextCurrent(window);

	// portion of window to render
	glViewport(0, 0,												/* bottom left of screen */
		winW, winH													/* top right of screen   */
	);

	// front buffer: default color
	// back buffer: this color
	glClearColor(0.f, 0.5f, 0.5f, 0.7f);
	glClear(GL_COLOR_BUFFER_BIT);									/* sets back buffer with this color */
	glfwSwapBuffers(window);

	// VERTICES /////////////////
	
	// create 2d vertices
	std::array<GLfloat, 9> vertices
	{
		// x            y                            z
		-0.5f, -0.5f * float(std::sqrt(3)) / 3,     0.f,			/* left corner */
		 0.5f, -0.5f * float(std::sqrt(3)) / 3,     0.f,			/* right corner */
		 0.f,   0.5f * float(std::sqrt(3)) * 2 / 3, 0.f				/* top corner  */
	};

	// SHADERS /////////////

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// binds shaders to a program
	GLuint shaderPrgm = glCreateProgram();
	glAttachShader(shaderPrgm, vertShader);
	glAttachShader(shaderPrgm, fragShader);
	glLinkProgram(shaderPrgm);

	// already bound to program
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	// BUFFERS ///////////////////
	// sending a batch of data from to cpu to gpu each time

	// VAO contains pointer to VBOs and tells gl how to interpret them
	// allows switching between multiple VBOs

	GLuint VAO{ }, VBO{};

	// generate VAO before VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), /* pass vertices to gpu buffer */
			GL_STATIC_DRAW);
												

	// dont close window right away
	while (!glfwWindowShouldClose(window))
	{
		// while window is active, poll for events
		glfwPollEvents();
	}

	
	// TERMINATE /////////////////
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;

}
