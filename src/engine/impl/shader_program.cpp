#include "../shader_program.hpp"
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, 0L, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, 0L, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
					  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}

void shader_load_glsl_from_source(const char* vertex_source, 
                    const char* fragment_source, Shader& program)
{
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_source, 0L);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_source, 0L);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	program.ID = glCreateProgram();
	glAttachShader(program.ID, vertex);
	glAttachShader(program.ID, fragment);
	glLinkProgram(program.ID);
	checkCompileErrors(program.ID, "PROGRAM");
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void shader_load_glsl(const char* vertexPath, 
                      const char* fragmentPath, 
                      Shader& program)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure &e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
	}

	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	
	shader_load_glsl_from_source(vShaderCode, fShaderCode, program);
}

void shader_use_program(const Shader &program)
{
	glUseProgram(program.ID);
}

void shader_delete_program(const Shader& program)
{
	glDeleteProgram(program.ID);
}

void shader_upload_int(const Shader& program, const char* name, int v)
{
	glUniform1i(glGetUniformLocation(program.ID, name), v);
}

void shader_upload_int_array(const Shader& program, const char* name, int* v, size_t size)
{
	glUniform1iv(glGetUniformLocation(program.ID, name), size, v);
}

void shader_upload_float(const Shader& program, const char* name, float v)
{
	glUniform1f(glGetUniformLocation(program.ID, name), v);
}

void shader_upload_vec2(const Shader& program, const char* name, float x, float y)
{
	glUniform2f(glGetUniformLocation(program.ID, name), x, y);
}


void shader_upload_vec3(const Shader& program, 
    const char* name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(program.ID, name), x, y, z);
}

void shader_upload_vec4(const Shader& program, 
	const char* name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(program.ID, name), x, y, z, w);
}

void shader_upload_float_array(const Shader& program, const char* name, 
	const float* array, size_t size)
{
	glUniform1fv(glGetUniformLocation(program.ID, name), size, array);
}

void shader_upload_mat4(const Shader& program, 
	const char* name, const float* mat)
{
	GLint location = glGetUniformLocation(program.ID, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}
