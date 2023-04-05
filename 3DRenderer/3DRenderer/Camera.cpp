#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "functions.h"


Camera::Camera(float fov, float near, float far, int width, int height)
	: fov(ln::radians(fov)), near(near), far(far), width(width), height(height)
{
	int AAsamples = 4;

	camProjection = ln::perspective(fov, near, far, (float)height/width);
	camMetricies = ln::eulerRotation(rotation);
	camMetricies.w.w = 1.0f;

	// Creating framebuffers for post processing and antialiasing.
	glGenFramebuffers(1, &postProcessFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);

	postProcessTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessTexture, 0);


	glGenFramebuffers(1, &antialiasingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, antialiasingFBO);

	glGenTextures(1, &antialiasingTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, antialiasingTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AAsamples, GL_RGB, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, antialiasingTexture, 0);
	
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, AAsamples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


	glGenVertexArrays(1, &rectVAO);
	glBindVertexArray(rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 30, rect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, VertShader.id);
	glAttachShader(shaderProgram, FragShader.id);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	glGenFramebuffers(1, &renderStoreFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, renderStoreFBO);
	renderStoreTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderStoreTexture, 0);
	

	// Creating shadow renderers to render shadows from shadow maps.
	glGenFramebuffers(1, &shadowRendererDirectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererDirectFBO);
	shadowRendererDirectTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererDirectTexture, 0);

	unsigned int shadowRBO;
	glGenRenderbuffers(1, &shadowRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, shadowRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shadowRBO);

	glGenFramebuffers(1, &shadowRendererSpotFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererSpotFBO);
	shadowRendererSpotTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererSpotTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shadowRBO);

	glGenFramebuffers(1, &shadowRendererPointFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererPointFBO);
	shadowRendererPointTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererPointTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, shadowRBO);

	shadowRendererProgram = glCreateProgram();
	glAttachShader(shadowRendererProgram, DirectShadowVert.id);
	glAttachShader(shadowRendererProgram, DirectShadowGeom.id);
	glAttachShader(shadowRendererProgram, DirectShadowFrag.id);
	glLinkProgram(shadowRendererProgram);
	glValidateProgram(shadowRendererProgram);
	glUseProgram(shadowRendererProgram);
	glUniform1f(glGetUniformLocation(shadowRendererProgram, "u_Bias"), 0.05f);

	cubeShadowRendererProgram = glCreateProgram();
	glAttachShader(cubeShadowRendererProgram, CubeShadowVert.id);
	glAttachShader(cubeShadowRendererProgram, CubeShadowGeom.id);
	glAttachShader(cubeShadowRendererProgram, CubeShadowFrag.id);
	glLinkProgram(cubeShadowRendererProgram);
	glValidateProgram(cubeShadowRendererProgram);
	glUseProgram(cubeShadowRendererProgram);

	glUniform1f(glGetUniformLocation(cubeShadowRendererProgram, "u_FarPlane"), 100);
	glUniform1f(glGetUniformLocation(cubeShadowRendererProgram, "u_Bias"), 0.001f);


	// Creating storages for shadows.
	glGenFramebuffers(1, &shadowStoreDirectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowStoreDirectFBO);
	shadowStoreDirectTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowStoreDirectTexture, 0);

	glGenFramebuffers(1, &shadowStoreSpotFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowStoreSpotFBO);
	shadowStoreSpotTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowStoreSpotTexture, 0);

	glGenFramebuffers(1, &shadowStorePointFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowStorePointFBO);
	shadowStorePointTexture = CreateNewTexture(width, height, GL_RGB, GL_UNSIGNED_BYTE, GL_NEAREST, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowStorePointTexture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::SetPosition(ln::vec3 pos)
{
	position = pos;
	update = true;
	glUseProgram(shadowRendererProgram);
	glUniform3fv(glGetUniformLocation(shadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
	glUseProgram(cubeShadowRendererProgram);
	glUniform3fv(glGetUniformLocation(cubeShadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(cubeShadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
}
void Camera::Move(ln::vec3 offset)
{
	position += offset;
	update = true;
	glUseProgram(shadowRendererProgram);
	glUniform3fv(glGetUniformLocation(shadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
	glUseProgram(cubeShadowRendererProgram);
	glUniform3fv(glGetUniformLocation(cubeShadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(cubeShadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
}
void Camera::SetRotation(ln::vec3 rot)
{
	rotation = rot;
	update = true;
	ln::mat3 tmpMat = ln::eulerRotation(rotation);
	forward = tmpMat * ln::vec3(0, 0, 1);
	right = tmpMat * ln::vec3(1, 0, 0);
	std::cout << forward << std::endl;
	camMetricies = tmpMat;
	glUseProgram(shadowRendererProgram);
	glUniform3fv(glGetUniformLocation(shadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
	glUseProgram(cubeShadowRendererProgram);
	glUniform3fv(glGetUniformLocation(cubeShadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(cubeShadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
}
void Camera::Rotate(ln::vec3 offset)
{
	rotation += offset;
	update = true;
	ln::mat3 tmpMat = ln::eulerRotation(rotation);
	forward = tmpMat * ln::vec3(0, 0, 1);
	right = tmpMat * ln::vec3(1, 0, 0);
	std::cout << right << std::endl;
	camMetricies = tmpMat;
	glUseProgram(shadowRendererProgram);
	glUniform3fv(glGetUniformLocation(shadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
	glUseProgram(cubeShadowRendererProgram);
	glUniform3fv(glGetUniformLocation(cubeShadowRendererProgram, "u_CamPos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(cubeShadowRendererProgram, "u_View"), 1, GL_TRUE, camMetricies);
}

ln::vec3 Camera::GetPosition() const
{
	return position;
}
ln::vec3 Camera::GetRotation() const
{
	return rotation;
}

void Camera::UpdateProjection(int windowWidth, int windowHeight)
{
	camProjection = ln::perspective(fov, near, far, (float)windowHeight / windowWidth);
	glUseProgram(cubeShadowRendererProgram);
	glUniformMatrix4fv(glGetUniformLocation(cubeShadowRendererProgram, "u_Proj"), 1, GL_TRUE, camProjection);
	glUseProgram(shadowRendererProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "u_Proj"), 1, GL_TRUE, camProjection);
}

void Camera::Render(Material* material, int dataSize, int windowWidth, int windowHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, antialiasingFBO);
	material->Bind();

	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, shadowRendererDirectTexture);
	glUniform1i(glGetUniformLocation(material->program, "u_ShadowDir"), 6);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, shadowRendererSpotTexture);
	glUniform1i(glGetUniformLocation(material->program, "u_ShadowSpot"), 7);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, shadowRendererPointTexture);
	glUniform1i(glGetUniformLocation(material->program, "u_ShadowPoint"), 8);

	glViewport(0, 0, width, height);
	glDrawElements(GL_TRIANGLES, dataSize, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, antialiasingFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderStoreFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, GL_NONE);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);

	glUseProgram(shaderProgram);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glViewport(0, 0, windowWidth, windowHeight);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowRendererPointTexture);//renderStoreTexture
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Camera::RenderDirectShadow(ln::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize)
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererDirectFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowRendererProgram);
	glUniform3fv(glGetUniformLocation(shadowRendererProgram, "u_Pos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "lightProj"), 1, GL_TRUE, lightProj);

	glViewport(0, 0, width, height);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(shadowRendererProgram, "u_ShadowMap"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowStoreDirectTexture);
	glUniform1i(glGetUniformLocation(shadowRendererProgram, "u_ShadowRender"), 1);
	glUniform1i(glGetUniformLocation(shadowRendererProgram, "u_HasPrevious"), hasPrevious);

	glDrawElements(GL_TRIANGLES, dataSize, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, shadowRendererDirectFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowStoreDirectFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::RenderSpotShadow(ln::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize)
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererSpotFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowRendererProgram);
	glUniform3fv(glGetUniformLocation(shadowRendererProgram, "u_Pos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(shadowRendererProgram, "lightProj"), 1, GL_TRUE, lightProj);

	glViewport(0, 0, width, height);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(shadowRendererProgram, "u_ShadowMap"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowStoreDirectTexture);
	glUniform1i(glGetUniformLocation(shadowRendererProgram, "u_ShadowRender"), 1);
	glUniform1i(glGetUniformLocation(shadowRendererProgram, "u_HasPrevious"), hasPrevious);

	glDrawElements(GL_TRIANGLES, dataSize, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, shadowRendererSpotFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowStoreSpotFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Camera::RenderPointShadow(ln::mat4 lightProj, unsigned int shadowMap, bool hasPrevious, int dataSize)
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererPointFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(cubeShadowRendererProgram);
	glUniform3fv(glGetUniformLocation(cubeShadowRendererProgram, "u_Pos"), 1, -position);
	glUniformMatrix4fv(glGetUniformLocation(cubeShadowRendererProgram, "lightProj"), 1, GL_TRUE, lightProj);

	glViewport(0, 0, width, height);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glUniform1i(glGetUniformLocation(cubeShadowRendererProgram, "u_ShadowMap"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowStorePointTexture);
	glUniform1i(glGetUniformLocation(cubeShadowRendererProgram, "u_ShadowRender"), 1);
	glUniform1i(glGetUniformLocation(cubeShadowRendererProgram, "u_HasPrevious"), hasPrevious);

	glDrawElements(GL_TRIANGLES, dataSize, GL_UNSIGNED_INT, nullptr);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, shadowRendererPointFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowStorePointFBO);
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Camera::ClearBuffers()
{
	glBindFramebuffer(GL_FRAMEBUFFER, antialiasingFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererDirectFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererPointFBO);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererSpotFBO);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Camera::BindBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, antialiasingFBO);
}
