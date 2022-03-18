#include "shaderClass.h"

std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Create Vertex Shader and get reference
	GLuint verticeShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object 
	glShaderSource(verticeShader, 1, &vertexSource, NULL);

	// Compile the Vertex shader into machine code
	glCompileShader(verticeShader);
	compileErrors(verticeShader, "VERTEX");

	// Create frangment shader and get reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach fragment shader source to the fragment shader object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);

	// Compile the fragment shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT");

	// Create shader program object and get its reference
	ID = glCreateProgram();
	// Attach the vertex and fragment shaders to the shader program
	glAttachShader(ID, verticeShader);
	glAttachShader(ID, fragmentShader);
	// Wrap up/ Link all shaders together into the shader program
	glLinkProgram(ID);

	// Delete the shader objects we got the shader program don't need these anymore
	glDeleteShader(verticeShader);
	glDeleteShader(fragmentShader);

}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	GLint hascompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hascompiled);
		if (hascompiled == GL_FALSE) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader COMPILATION ERROR for:" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hascompiled);
		if (hascompiled == GL_FALSE) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader LINKING ERROR for:" << type << "\n" << std::endl;
		}
	}
}