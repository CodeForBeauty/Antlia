#include "Material.h"

Material::Material()
{
	vs = new Shader("shaders/BasicVertex.vert", GL_VERTEX_SHADER);
	fs = new Shader("shaders/BasicFragment.frag", GL_FRAGMENT_SHADER);
	albedo = new float[4] {1, 1, 1, 1};
	CompileShaders();
}

Material::Material(Shader* vs, Shader* fs) : vs(vs), fs(fs) { CompileShaders(); albedo = new float[4] {1, 1, 1, 1}; }

Material::Material(Shader* vs, Shader* fs, float* albedo) : vs(vs), fs(fs), albedo(albedo) { CompileShaders(); }

Material::Material(Shader* vs, Shader* fs, float* albedo, float metalic) : vs(vs), fs(fs), albedo(albedo), metalic(metalic) { CompileShaders(); }

Material::Material(float* albedo) : albedo(albedo) 
{
	vs = new Shader("shaders/BasicVertex.vert", GL_VERTEX_SHADER);
	fs = new Shader("shaders/BasicFragment.frag", GL_FRAGMENT_SHADER);
	CompileShaders();
}

Material::~Material()
{
	glDeleteProgram(program);
	glUseProgram(0);
	delete vs, fs, albedo;
}

void Material::CompileShaders()
{
	program = glCreateProgram();
	glAttachShader(program, vs->compileShader());
	glAttachShader(program, fs->compileShader());
	glLinkProgram(program);
	glValidateProgram(program);
	glUseProgram(program);

	pos = glGetUniformLocation(program, "u_Pos");
	proj = glGetUniformLocation(program, "u_Proj");
	rot = glGetUniformLocation(program, "u_Rot");
	scale = glGetUniformLocation(program, "u_Scale");
	color = glGetUniformLocation(program, "u_Color");

	glUniform4f(color, albedo[0], albedo[1], albedo[2], albedo[3]);
}

void Material::SetPos(float x, float y, float z)
{
	glUniform4f(pos, x, y, z, 1);
}
void Material::SetRot(float* value)
{
	glUniformMatrix4fv(rot, 1, GL_TRUE, value);
}
void Material::SetScale(float x, float y, float z)
{
	glUniform4f(scale, x, y, z, 1);
}
void Material::SetProj(float* value)
{
	glUniformMatrix4fv(proj, 1, GL_TRUE, value);
}


void Material::SetAlbedo(float r, float g, float b, float a)
{
	albedo[0] = r;
	albedo[1] = g;
	albedo[2] = b;
	albedo[3] = a;
	glUniform4f(color, r, g, b, a);
}
float* Material::GetAlbedo()
{
	return albedo;
}
void Material::SetMetalic(float value)
{
	metalic = value;
}
float Material::GetMetalic()
{
	return metalic;
}
void Material::SetRoughness(float value)
{
	roughness = value;
}
float Material::GetRoughness()
{
	return roughness;
}
void Material::SetSpecular(float value) 
{
	specular = value;
}
float Material::GetSpecular()
{
	return specular;
}