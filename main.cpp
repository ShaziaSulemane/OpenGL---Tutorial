#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

int main() 
{
	// Initialize GLFW
	glfwInit();

	// Triangle coordinates, need to be between -1 and 1
	GLfloat  vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5 * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	// Tell GLFW what version of OpenGL we are using
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of size 800x800 and names OpenGL Tutorial
	GLFWwindow* window = glfwCreateWindow(800, 800, "OpenGL Tutorial", NULL, NULL);

	// Error message for if window isn't created
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce window into current context
	glfwMakeContextCurrent(window);

	// Load GLAD so it configures OpenGL
	gladLoadGL();

	// Specify the viewport of OpenGL in the window
	// In this case the viewport goes from x = 0 y = 0 to x = 800 y = 800
	glViewport(0, 0, 800, 800);

	// Create Vertex Shader and get reference
	GLuint verticeShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object 
	glShaderSource(verticeShader, 1, &vertexShaderSource, NULL);
	// Compile the Vertex shader into machine code
	glCompileShader(verticeShader);

	// Create frangment shader and get reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source to the fragment shader object
	glShaderSource(fragmentShader, 1, &vertexShaderSource, NULL);
	// Compile the fragment shader into machine code
	glCompileShader(fragmentShader);

	// Create shader program object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the vertex and fragment shaders to the shader program
	glAttachShader(shaderProgram, verticeShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap up/ Link all shaders together into the shader program
	glLinkProgram(shaderProgram);

	// Delete the shader objects we got the shader program don't need these anymore
	glDeleteShader(verticeShader);
	glDeleteShader(fragmentShader);

	// Create reference containers for the vertex array object and the vertex buffer object
	GLuint VAO, VBO;

	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Make the VAO the current Vertex array object by binding it 
	glBindVertexArray(VAO);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Introduce the vertices into de VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the vertex attribute so that opengl knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the vertex attribute so that opengl knows to use it
	glEnableVertexAttribArray(0);

	//Bind both the VAO and VBO to 0 so that we dont accidently modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Specify the color of the background
	// Last number 1.0 determines color transparency
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell opengl which shader we want to use 
		glUseProgram(shaderProgram);
		// Bind the VAO so opengl knows to use it
		glBindVertexArray(VAO);
		// Draw the triangle using the GL_TRIANGLES primitive
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);
		// Take care of all window alterations (resize, etc)
		glfwPollEvents();
	}

	// Delete objects we created 
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Delete window before exiting program
	glfwDestroyWindow(window);
	// Delete GLFW objects 
	glfwTerminate();
	return 0;
}