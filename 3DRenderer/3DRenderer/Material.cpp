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
	if (texture)
		delete texture;
}

void Material::CompileShaders()
{
	program = glCreateProgram();
	glAttachShader(program, vs->compileShader());
	glAttachShader(program, fs->compileShader());
	glLinkProgram(program);
	glValidateProgram(program);
	glUseProgram(program);

	camPos = glGetUniformLocation(program, "u_CamPos");
	view = glGetUniformLocation(program, "u_View");
	proj = glGetUniformLocation(program, "u_Proj");
	color = glGetUniformLocation(program, "u_Color");

	glUniform4f(color, albedo[0], albedo[1], albedo[2], albedo[3]);
	//glUniform1i(glGetUniformLocation(program, "u_Texture"), 0);

	glUniform1i(glGetUniformLocation(program, "u_Metalic"), metalic);
	glUniform1i(glGetUniformLocation(program, "u_Roughness"), roughness);
	glUniform1i(glGetUniformLocation(program, "u_Specular"), specular);
}

void Material::SetCamPos(float x, float y, float z)
{
	glUniform3f(camPos, -x, -y, -z);
}
void Material::SetView(float* value)
{
	glUniformMatrix4fv(view, 1, GL_TRUE, value);
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
	glUniform1i(glGetUniformLocation(program, "u_Metalic"), value);
}
float Material::GetMetalic()
{
	return metalic;
}
void Material::SetRoughness(float value)
{
	roughness = value;
	glUniform1i(glGetUniformLocation(program, "u_Roughness"), value);
}
float Material::GetRoughness()
{
	return roughness;
}
void Material::SetSpecular(float value) 
{
	specular = value;
	glUniform1i(glGetUniformLocation(program, "u_Specular"), value);
}
float Material::GetSpecular()
{
	return specular;
}

void Material::LoadTexture(const char* path, int slot)
{
	Use();
	if (texture)
		delete texture;
	switch (slot)
	{
	case TEXTURE_ALBEDO: if (texture) delete texture; texture = new Texture(path); break;
	case TEXTURE_SPECULAR: if (specTex) delete specTex; specTex = new Texture(path); break;
	case TEXTURE_METALIC: if (metalTex) delete metalTex; metalTex = new Texture(path); break;
	case TEXTURE_ROUGHNESS: if (roughTex) delete roughTex; roughTex = new Texture(path); break;
	case TEXTURE_NORMAL: if (normal) delete normal; normal = new Texture(path); break;
	}
}

void Material::DeleteTexture(int slot) 
{
	switch (slot)
	{
	case TEXTURE_ALBEDO: if (texture) delete texture; break;
	case TEXTURE_SPECULAR: if (specTex) delete specTex; break;
	case TEXTURE_METALIC: if (metalTex) delete metalTex; break;
	case TEXTURE_ROUGHNESS: if (roughTex) delete roughTex; break;
	case TEXTURE_NORMAL: if (normal) delete normal; break;
	}
}

void Material::Use()
{
	glUseProgram(program);
}
void Material:: Bind()
{
	Use();
	if (texture)
	{
		texture->Bind(TEXTURE_ALBEDO);
		glUniform1i(glGetUniformLocation(program, "u_Texture"), TEXTURE_ALBEDO);
		glUniform1i(glGetUniformLocation(program, "u_UseTex"), 1);
	} else glUniform1i(glGetUniformLocation(program, "u_UseTex"), 0);
	if (specTex)
	{
		specTex->Bind(TEXTURE_SPECULAR);
		glUniform1i(glGetUniformLocation(program, "u_SpecularTexture"), TEXTURE_SPECULAR);
		glUniform1i(glGetUniformLocation(program, "u_UseSpecTex"), 1);
	} else glUniform1i(glGetUniformLocation(program, "u_UseSpecTex"), 0);
	if (roughTex)
	{
		roughTex->Bind(TEXTURE_ROUGHNESS);
		glUniform1i(glGetUniformLocation(program, "u_RoughnessTexture"), TEXTURE_ROUGHNESS);
		glUniform1i(glGetUniformLocation(program, "u_UseRoughTex"), 1);
	} else glUniform1i(glGetUniformLocation(program, "u_UseRoughTex"), 0);
	if (normal)
	{
		normal->Bind(TEXTURE_NORMAL);
		glUniform1i(glGetUniformLocation(program, "u_NormalTexture"), TEXTURE_NORMAL);
		glUniform1i(glGetUniformLocation(program, "u_UseNormTex"), 1);
	} else glUniform1i(glGetUniformLocation(program, "u_UseNormTex"), 0);
	if (metalTex)
	{
		normal->Bind(TEXTURE_METALIC);
		glUniform1i(glGetUniformLocation(program, "u_MetalicTexture"), TEXTURE_METALIC);
		glUniform1i(glGetUniformLocation(program, "u_UseMetalTex"), 1);
	} else glUniform1i(glGetUniformLocation(program, "u_UseMetalTex"), 0);
}