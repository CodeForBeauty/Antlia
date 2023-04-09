#pragma once
#include "Objects.h"
#include "linmath.h"


class Camera
{
public:
	Camera(float fov, float near, float far, int width, int height);

	void SetPosition(lm::vec3 position);
	void Move(lm::vec3 offset);
	void SetRotation(lm::vec3 rotation);
	void Rotate(lm::vec3 offset);

	lm::vec3 GetPosition() const;
	lm::vec3 GetRotation() const;

	void UpdateProjection(int windowWidth, int windowHeight);

	void Render(Material* material, int dataSize, int windowWidth, int windowHeight);

	void RenderDirectShadow(lm::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize);
	void RenderSpotShadow(lm::mat4 lightProj, lm::vec3 lightPos, unsigned int shadowMap, bool hasPrevious, int dataSize);
	void RenderPointShadow(lm::mat4 lightProj, lm::vec3 lightPos, unsigned int shadowMap, bool hasPrevious, int dataSize);

	void ClearBuffers();
	void BindBuffer();

	float fov;
	float near;
	float far;
	int width, height;

	bool update = true;

	lm::mat4 camMetricies = {};
	lm::mat4 camProjection = {};

	lm::vec3 forward = { 0, 0, 1 };
	lm::vec3 right = { 1, 0, 0 };
	lm::vec3 up = { 0, 1, 0 };

private:
	lm::vec3 position = {};
	lm::vec3 rotation = {};

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