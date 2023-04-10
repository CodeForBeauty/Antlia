#include "Material.h"

Material::Material()
	:vs(Shader("shaders/BasicVertex.vert", GL_VERTEX_SHADER)), 
	fs(Shader("shaders/BasicFragment.frag", GL_FRAGMENT_SHADER)),
	gs(Shader("shaders/BasicGeometry.geom", GL_GEOMETRY_SHADER))
{
	CompileShaders();
}

Material::Material(Shader vs, Shader fs, Shader gs)
	: vs(vs), fs(fs), gs(gs)
{
	CompileShaders(); 
}

Material::Material(Shader vs, Shader fs, Shader gs, lm::vec4 albedo) 
	: vs(vs), fs(fs), gs(gs), albedo(albedo)
{
	CompileShaders();
}

Material::Material(lm::vec4 albedo) 
	: albedo(albedo),
	vs(Shader("shaders/BasicVertex.vert", GL_VERTEX_SHADER)),
	fs(Shader("shaders/BasicFragment.frag", GL_FRAGMENT_SHADER)),
	gs(Shader("shaders/BasicGeometry.geom", GL_GEOMETRY_SHADER))
{
	vs = Shader("shaders/BasicVertex.vert", GL_VERTEX_SHADER);
	fs = Shader("shaders/BasicFragment.frag", GL_FRAGMENT_SHADER);
	gs = Shader("shaders/BasicGeometry.geom", GL_GEOMETRY_SHADER);
	CompileShaders();
}

Material::~Material()
{
	glDeleteProgram(program);
	if (texture) delete texture;
	if (specTex) delete specTex;
	if (metalTex) delete metalTex;
	if (roughTex) delete roughTex;
	if (normal) delete normal;
}

void Material::CompileShaders()
{
	program = glCreateProgram();
	glAttachShader(program, vs.id);
	glAttachShader(program, fs.id);
	glAttachShader(program, gs.id);
	glLinkProgram(program);
	glValidateProgram(program);
	glUseProgram(program);

	camPos = glGetUniformLocation(program, "u_CamPos");
	view = glGetUniformLocation(program, "u_View");
	proj = glGetUniformLocation(program, "u_Proj");
	color = glGetUniformLocation(program, "u_Color");

	glUniform4f(color, albedo.x, albedo.y, albedo.z, albedo.w);

	glUniform1f(glGetUniformLocation(program, "u_Metalic"), metalic);
	glUniform1f(glGetUniformLocation(program, "u_Roughness"), roughness);
	glUniform1f(glGetUniformLocation(program, "u_Specular"), specular);
}

void Material::SetCamPos(float x, float y, float z)
{
	glUniform3f(camPos, -x, -y, -z);
}
void Material::SetCamPos(lm::vec3 position)
{
	glUniform3fv(camPos, 1, -position);
}
void Material::SetView(lm::mat4 value)
{
	glUniformMatrix4fv(view, 1, GL_TRUE, value);
}
void Material::SetProj(lm::mat4 value)
{
	glUniformMatrix4fv(proj, 1, GL_TRUE, value);
}


void Material::SetAlbedo(float r, float g, float b, float a)
{
	albedo.x = r;
	albedo.y = g;
	albedo.z = b;
	albedo.w = a;
	glUniform4f(color, r, g, b, a);
}
lm::vec4 Material::GetAlbedo()
{
	return albedo;
}
void Material::SetMetalic(float value)
{
	metalic = value;
	glUniform1f(glGetUniformLocation(program, "u_Metalic"), value);
}
float Material::GetMetalic()
{
	return metalic;
}
void Material::SetRoughness(float value)
{
	roughness = value;
	glUniform1f(glGetUniformLocation(program, "u_Roughness"), value);
}
float Material::GetRoughness()
{
	return roughness;
}
void Material::SetSpecular(float value) 
{
	specular = value;
	glUniform1f(glGetUniformLocation(program, "u_Specular"), value);
}
float Material::GetSpecular()
{
	return specular;
}

void Material::SetName(std::string na)
{
	name = na;
}
std::string Material::GetName()
{
	return name;
}

void Material::LoadTexture(const char* path, int slot)
{
	Use();
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

void Material::ClearLights()
{
	for (int i = 0; i < 4; i++)
	{
		std::string buff = "u_DirectLightColor[" + std::to_string(i) + "]";
		glUniform4f(glGetUniformLocation(program, buff.c_str()), 0, 0, 0, 0);
		buff = "u_PointLightColor[" + std::to_string(i) + "]";
		glUniform4f(glGetUniformLocation(program, buff.c_str()), 0, 0, 0, 0);
		buff = "u_SpotLightColor[" + std::to_string(i) + "]";
		glUniform4f(glGetUniformLocation(program, buff.c_str()), 0, 0, 0, 0);
	}
	for (int i = 4; i < 8; i++)
	{
		std::string buff = "u_PointLightColor[" + std::to_string(i) + "]";
		glUniform4f(glGetUniformLocation(program, buff.c_str()), 0, 0, 0, 0);
		buff = "u_SpotLightColor[" + std::to_string(i) + "]";
		glUniform4f(glGetUniformLocation(program, buff.c_str()), 0, 0, 0, 0);
	}
}
