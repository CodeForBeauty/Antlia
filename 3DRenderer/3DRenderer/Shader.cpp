#include <fstream>
#include <string>
#include "Shader.h"


using namespace std;

Shader::Shader(std::string filepath, unsigned int type) : type(type)
{
	loadFromFile(filepath);
	compileShader();
}

Shader::~Shader()
{
	glDeleteShader(id);
}

void Shader::loadFromFile(std::string filepath)
{
	std::string content;
	std::string line;

	std::ifstream file(filepath);

	while (getline(file, line))
	{
		content += line + "\n";
	}

	file.close();
	setShader(content);
}

std::string Shader::getShader()
{
	return shader_text;
}

void Shader::setShader(std::string shader)
{
	shader_text = shader;
}

void Shader::compileShader()
{
	id = glCreateShader(type);
	const char* src = shader_text.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* msg = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, msg);
		std::cout << "Error: " << msg;
	}
}