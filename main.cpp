
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "Texture.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

// Triangle coordinates, need to be between -1 and 1
GLfloat  vertices_triangle[] =
{
   -0.5f,      -0.5f * float(sqrt(3))     / 3, 0.0f, 0.1f,  0.0f, 0.5f,   // Lower left corner
	0.5f,      -0.5f * float(sqrt(3))     / 3, 0.0f, 0.0f,  0.1f, 0.5f,   // Lower right corner
	0.0f,       0.5  * float(sqrt(3)) * 2 / 3, 0.0f, 0.2f,  0.3f, 0.3f,   // Upper corner
   -0.5f / 2,   0.5f * float(sqrt(3))     / 6, 0.0f, 0.4f,  0.4f, 0.5f,   // Inner Left
	0.5f / 2,   0.5f * float(sqrt(3))     / 6, 0.0f, 0.5f,  0.6f, 0.9f,   // Inner Right
	0.0f,      -0.5f * float(sqrt(3))     / 3, 0.0f, 1.0f,  0.0f, 0.2f,   // Inner down
};

GLuint indices_triangle[] = {
	0, 3, 5, // Lower left triangle 
	3, 2, 4, // Lower right triangle
	5, 4, 1  // upper triangle
};

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TextureCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

// Indices for vertices order
GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};



int main() 
{
	// Initialize GLFW
	glfwInit();

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

	Shader shaderProgram("default.vert", "default.frag");
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "MVP");

	int width, height, nchan;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load("pop_cat.png", &width, &height, &nchan, 0);

	std::string texPath = "popcat.png";

	// Texture
	Texture popCat("pop_cat.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));
	
	VAO1.LinkAttribute(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttribute(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttribute(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();


	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Last number 1.0 determines color transparency
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell opengl which shader we want to use 
		shaderProgram.Activate();

		glUniform1f(uniID, 0.5f);
		popCat.Bind();
		// Bind the VAO so opengl knows to use it
		VAO1.Bind();
		// Draw the triangle using the GL_TRIANGLES primitive
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		// Take care of all window alterations (resize, etc)
		glfwPollEvents();
	}

	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	// Delete window before exiting program
	glfwDestroyWindow(window);
	// Delete GLFW objects 
	glfwTerminate();
	return 0;
}