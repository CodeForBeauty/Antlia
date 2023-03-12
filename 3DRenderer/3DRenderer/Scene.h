#pragma once

#include <vector>
#include "Objects.h"
#include "Lighting.h"
#include "Camera.h"
#include "Shader.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(Mesh* object);
	void AddLight(Light* light);
	void AddEntity(Entity* entity);
	void AddMaterial(Material* material);
	void SetObjectMaterial(Mesh* object, Material* material);

	void DeleteMaterial(Material* material);
	void DeleteLight(Light* light);
	void DeleteObject(Mesh* object);

	void Render(float* proj, int width, int height);

	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	unsigned int fbo;
	unsigned int fbt;
	unsigned int fboAA;
	unsigned int fbtAA;
	unsigned int rbo, rboShadow, rboShadowColor;
	int AAsamples = 4;

	Camera* preview = new Camera(90, 0.5, 100);
	Camera* renderCamera = new Camera(90, 0.1, 100);

	std::string savePath = "";

	std::vector <Mesh*> objects;
	std::vector <Light*> lights;
	std::vector <Entity*> entities;
	std::vector <Material*> materials;
private:
	std::vector <std::vector <Mesh*>> materialGroup;

	bool updateLight = true;
	unsigned int program;
	unsigned int rectVAO, rectVBO;
	unsigned int shadowProgram;
	unsigned int shadowFBO, shadowMap;
	unsigned int shadowWidth = 2048, shadowHeight = 2048;
	unsigned int shadowRProgram;
	unsigned int shadowRenderer, shadowRendererT;
	float rect[30] = {
					   1.0f,  -1.0f, 1.0f, 1, 0,
					  -1.0f, -1.0f, 1.0f, 0, 0,
					  -1.0f,  1.0f, 1.0f, 0, 1,

					  -1.0f,  1.0f, 1.0f, 0, 1,
					   1.0f,  1.0f, 1.0f, 1, 1,
					   1.0f, -1.0f, 1.0f, 1, 0
	};

	Shader FrameBufferVert = Shader("shaders/FrameBuffer.vert", GL_VERTEX_SHADER);
	Shader FrameBufferFrag = Shader("shaders/FrameBuffer.frag", GL_FRAGMENT_SHADER);
	Shader ShadowVert = Shader("shaders/ShadowMap.vert", GL_VERTEX_SHADER);
	Shader ShadowFrag = Shader("shaders/ShadowMap.frag", GL_FRAGMENT_SHADER);
	Shader ShadowRVert = Shader("shaders/ShadowRender.vert", GL_VERTEX_SHADER);
	Shader ShadowRFrag = Shader("shaders/ShadowRender.frag", GL_FRAGMENT_SHADER);
	Shader ShadowRGeom = Shader("shaders/ShadowRender.geom", GL_GEOMETRY_SHADER);
};