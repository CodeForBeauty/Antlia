#pragma once
#include "Objects.h"
#include "linmath.h"


class Camera
{
public:
	Camera(float fov, float near, float far, int width, int height);

	void SetPosition(ln::vec3 position);
	void Move(ln::vec3 offset);
	void SetRotation(ln::vec3 rotation);
	void Rotate(ln::vec3 offset);

	ln::vec3 GetPosition() const;
	ln::vec3 GetRotation() const;

	void UpdateProjection(int windowWidth, int windowHeight);

	void Render(Material* material, int dataSize, int windowWidth, int windowHeight);

	void RenderDirectShadow(ln::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize);
	void RenderSpotShadow(ln::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize);
	void RenderPointShadow(ln::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize);

	unsigned int GetDirectShadow();
	unsigned int GetSpotShadow();
	unsigned int GetPointShadow();

	void ClearBuffers();

	void BindBuffer();


	float fov;
	float near;
	float far;
	int width, height;

	bool update = true;

	ln::mat4 camMetricies = {};
	ln::mat4 camProjection = {};

	ln::vec3 forward = ln::vec3(0, 0, 1);
	ln::vec3 right = ln::vec3(1, 0, 0);

private:
	ln::vec3 position = {};
	ln::vec3 rotation = {};

	unsigned int rectVAO, rectVBO;

	unsigned int postProcessFBO, postProcessTexture;
	unsigned int renderStoreFBO, renderStoreTexture;
	unsigned int antialiasingFBO, antialiasingTexture;

	unsigned int shadowRendererDirectFBO, shadowRendererDirectTexture;
	unsigned int shadowRendererSpotFBO, shadowRendererSpotTexture;
	unsigned int shadowRendererPointFBO, shadowRendererPointTexture;

	unsigned int shadowStoreDirectFBO, shadowStoreDirectTexture;
	unsigned int shadowStoreSpotFBO, shadowStoreSpotTexture;
	unsigned int shadowStorePointFBO, shadowStorePointTexture;

	unsigned int shaderProgram, shadowRendererProgram, cubeShadowRendererProgram;

	Shader VertShader = Shader("shaders/FrameBuffer.vert", GL_VERTEX_SHADER);
	Shader FragShader = Shader("shaders/FrameBuffer.frag", GL_FRAGMENT_SHADER);

	Shader DirectShadowVert = Shader("shaders/ShadowRender.vert", GL_VERTEX_SHADER);
	Shader DirectShadowFrag = Shader("shaders/ShadowRender.frag", GL_FRAGMENT_SHADER);
	Shader DirectShadowGeom = Shader("shaders/ShadowRender.geom", GL_GEOMETRY_SHADER);

	Shader CubeShadowVert = Shader("shaders/ShadowRender1.vert", GL_VERTEX_SHADER);
	Shader CubeShadowFrag = Shader("shaders/ShadowRender1.frag", GL_FRAGMENT_SHADER);
	Shader CubeShadowGeom = Shader("shaders/ShadowRender1.geom", GL_GEOMETRY_SHADER);

	float rect[30] = {
					   1.0f,  -1.0f, 1.0f, 1, 0,
					  -1.0f, -1.0f, 1.0f, 0, 0,
					  -1.0f,  1.0f, 1.0f, 0, 1,

					  -1.0f,  1.0f, 1.0f, 0, 1,
					   1.0f,  1.0f, 1.0f, 1, 1,
					   1.0f, -1.0f, 1.0f, 1, 0
	};
};