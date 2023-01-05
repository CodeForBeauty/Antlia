#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iostream>
#include <fstream>

class Shader
{
public:
	Shader(std::string filepath, unsigned int type);
	~Shader();
	void loadFromFile(std::string filepath);
	std::string getShader();
	void setShader(std::string shader);

	unsigned int compileShader();
private:
	std::string shader_text;
	unsigned int type;
	unsigned int id;
};
