#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	ifstream vertexFile;
	ifstream fragmentFile;
	stringstream vertexSStream;
	stringstream fragmentSStream;

	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	vertexFile.exceptions(ifstream::failbit || ifstream::badbit);
	fragmentFile.exceptions(ifstream::failbit || ifstream::badbit);

	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open())
		{
			throw exception("open file error");
		}
		
		
		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		// 2. 编译着色器
		unsigned int vertex, fragment;
		

		// 顶点着色器
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// 片段着色器也类似
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment,"FRAGMENT");

		// 着色器程序
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	catch(const std::exception& ex)
	{
		printf(ex.what());
	}
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::SetUniform3f(const char * paramNameString, glm::vec3 param)
{
	glUniform3f(glGetUniformLocation(ID, paramNameString), param.x,param.y,param.z);
}

void Shader::SetUniform1f(const char * paramNameString, float param)
{
	glUniform1f(glGetUniformLocation(ID, paramNameString), param);
}

void Shader::SetUniform1i(const char * paramNameString, int slot)
{
	glUniform1i(glGetUniformLocation(ID, paramNameString), slot);
}

void Shader::checkCompileErrors(unsigned int ID, std::string type) 
{
	int success;
	char infoLog[512];

	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 512, NULL, infoLog);
			cout << "shader compile error" << infoLog << endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			cout << "program linking error:" << infoLog << endl;
		}
	}
}