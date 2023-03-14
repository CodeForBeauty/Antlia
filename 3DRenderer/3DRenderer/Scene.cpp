#include "Scene.h"
#include <array>
#include <iostream>
#include <string>

Scene::Scene()
{
	materials.push_back(new Material());
	//materials[0]->SetAlbedo
	materials[0]->SetMetalic(0.0f);
	materials[0]->SetRoughness(0.5f);
	materials[0]->SetSpecular(0.5f);
	materials[0]->sceneSlot = 0;
	materialGroup.push_back({});

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 10000, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 40000, nullptr, GL_DYNAMIC_DRAW);

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &fbt);
	glBindTexture(GL_TEXTURE_2D, fbt);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 850, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbt, 0);
	
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << fboStatus << std::endl;

	glGenFramebuffers(1, &fboAA);
	glBindFramebuffer(GL_FRAMEBUFFER, fboAA);

	glGenTextures(1, &fbtAA);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbtAA);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, AAsamples, GL_RGB, 850, 600, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbtAA, 0);


	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, AAsamples, GL_DEPTH24_STENCIL8, 850, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << fboStatus << std::endl;

	program = glCreateProgram();

	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), &rect, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glAttachShader(program, FrameBufferVert.compileShader());
	glAttachShader(program, FrameBufferFrag.compileShader());
	glLinkProgram(program);
	glValidateProgram(program);
	glUseProgram(program);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(program, "u_ScreenTex"), 0);
	glUseProgram(0);


	glGenFramebuffers(1, &shadowFBO);
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	shadowProgram = glCreateProgram();
	glAttachShader(shadowProgram, ShadowVert.compileShader());
	glAttachShader(shadowProgram, ShadowFrag.compileShader());
	glLinkProgram(shadowProgram);
	glValidateProgram(shadowProgram);
	glUseProgram(shadowProgram);
	glUseProgram(0);


	glGenFramebuffers(1, &shadowRenderer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRenderer);

	glGenTextures(1, &shadowRendererT);
	glBindTexture(GL_TEXTURE_2D, shadowRendererT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 850, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererT, 0);

	
	glGenRenderbuffers(1, &rboShadowColor);
	glBindRenderbuffer(GL_RENDERBUFFER, rboShadowColor);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 850, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboShadowColor);


	glGenFramebuffers(1, &shadowRenderer1);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRenderer1);

	glGenTextures(1, &shadowRendererT1);
	glBindTexture(GL_TEXTURE_2D, shadowRendererT1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 850, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererT1, 0);


	glGenRenderbuffers(1, &rboShadowColor1);
	glBindRenderbuffer(GL_RENDERBUFFER, rboShadowColor1);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 850, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboShadowColor1);
	
	shadowRProgram = glCreateProgram();
	glAttachShader(shadowRProgram, ShadowRVert.compileShader());
	glAttachShader(shadowRProgram, ShadowRGeom.compileShader());
	glAttachShader(shadowRProgram, ShadowRFrag.compileShader());
	glLinkProgram(shadowRProgram);
	glValidateProgram(shadowRProgram);
	glUseProgram(shadowRProgram);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glGenFramebuffers(1, &shadowRendererDir);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererDir);

	glGenTextures(1, &shadowRendererTDir);
	glBindTexture(GL_TEXTURE_2D, shadowRendererTDir);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 850, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererTDir, 0);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << fboStatus << std::endl;

	
	glGenFramebuffers(1, &shadowRendererSpot);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRendererSpot);

	glGenTextures(1, &shadowRendererTSpot);
	glBindTexture(GL_TEXTURE_2D, shadowRendererTSpot);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 850, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowRendererTSpot, 0);

	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << fboStatus << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	
}

Scene::~Scene()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	std::cout << "object" << std::endl;
	for (int i = 0; i < entities.size(); i++)
		delete entities[i];
	for (int i = 0; i < lights.size(); i++)
		delete lights[i];
	for (int i = 0; i < materials.size(); i++)
		delete materials[i];
	delete preview;
	delete renderCamera;
}

void Scene::AddObject(Mesh* object)
{
	int offset = 0;
	for (int i = 0; i < materialGroup[0].size(); i++)
	{
		offset += materialGroup[0][i]->geometry->verticiesCount;
	}
	
	for (int i = 0; i < object->geometry->indeciesCount; i++)
	{
		object->geometry->transformedIndecies[i] = object->geometry->indecies[i] + offset;
	}
	objects.push_back(object);
	materialGroup[0].push_back(object);
	object->material = materials[0];
}

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}

void Scene::AddLight(Light* light)
{
	lights.push_back(light);
	updateLight = true;
}

void Scene::AddMaterial(Material* material)
{
	materials.push_back(material);
	materialGroup.push_back({});
	material->sceneSlot = materialGroup.size()-1;
	glUseProgram(material->program);
	Vector3D camPos = preview->GetPosition();
	material->SetCamPos(camPos.x, camPos.y, camPos.z);
	material->SetView(preview->rotMetricies);
}

void Scene::SetObjectMaterial(Mesh* object, Material* material)
{
	bool start = false;
	for (Mesh* i : materialGroup[object->material->sceneSlot])
	{
		if (start)
			for (int j = 0; j < i->geometry->indeciesCount; j++)
				i->geometry->transformedIndecies[j] -=  object->geometry->verticiesCount;
		if (i == object) start = true;
	}
	materialGroup[object->material->sceneSlot].erase(std::remove_if(materialGroup[object->material->sceneSlot].begin(),
		materialGroup[object->material->sceneSlot].end(),
		[&](Mesh* i) { return i == object; }), materialGroup[object->material->sceneSlot].end());
	
	materialGroup[material->sceneSlot].push_back(object);
	object->material = material;

	int offset = 0;
	for (int i = 0; i < materialGroup[material->sceneSlot].size() - 1; i++)
		offset += materialGroup[material->sceneSlot][i]->geometry->verticiesCount;
	
	for (int i = 0; i < object->geometry->indeciesCount; i++)
		object->geometry->transformedIndecies[i] = object->geometry->indecies[i] + offset;
}

void Scene::DeleteMaterial(Material* material)
{
	int offset = 0;
	for (Mesh* i : materialGroup[0])
		offset += i->geometry->verticiesCount;
	for (Mesh* i : materialGroup[material->sceneSlot])
	{
		materialGroup[0].push_back(i);
		for (int j = 0; j < i->geometry->indeciesCount; j++)
			i->geometry->transformedIndecies[j] = i->geometry->indecies[j] + offset;
		offset += i->geometry->indeciesCount;
	}
	
	materials.erase(materials.begin() + material->sceneSlot);
	materialGroup.erase(materialGroup.begin() + material->sceneSlot);
	delete material;
}

void Scene::DeleteLight(Light* light)
{
	lights.erase(std::remove_if(lights.begin(), lights.end(), [&](Light* i) { return i == light; }), lights.end());
	delete light;
}

void Scene::DeleteObject(Mesh* object)
{
	bool start = false;
	for (Mesh* i : materialGroup[object->material->sceneSlot])
	{
		if (i == object) start = true;
		if (start)
			for (int j = 0; j < i->geometry->indeciesCount; j++)
				i->geometry->transformedIndecies[j] = i->geometry->transformedIndecies[j] - object->geometry->verticiesCount;
	}
	materialGroup[object->material->sceneSlot].erase(std::remove_if(materialGroup[object->material->sceneSlot].begin(),
		materialGroup[object->material->sceneSlot].end(),
		[&](Mesh* i) { return i == object; }), materialGroup[object->material->sceneSlot].end());
	delete object;
}


void Scene::Render(float* proj, int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowRenderer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, shadowRenderer1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, fboAA);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	for (int i = 0; i < materialGroup.size(); i++)
	{
		glEnable(GL_DEPTH_TEST);
		materials[i]->Use();
		materials[i]->SetProj(proj);

		if (preview->update)
		{
			Vector3D camPos = preview->GetPosition();
			materials[i]->SetCamPos(camPos.x, camPos.y, camPos.z);
			materials[i]->SetView(preview->rotMetricies);
		}


		std::vector <Vertex> batchVerticies;
		std::vector <unsigned int> batchIndecies;

		for (int j = 0; j < materialGroup[i].size(); j++)
		{
			batchVerticies.insert(batchVerticies.end(), materialGroup[i][j]->geometry->transformedVerticies, 
				materialGroup[i][j]->geometry->transformedVerticies + materialGroup[i][j]->geometry->verticiesCount);

			batchIndecies.insert(batchIndecies.end(), materialGroup[i][j]->geometry->transformedIndecies,
				materialGroup[i][j]->geometry->transformedIndecies + materialGroup[i][j]->geometry->indeciesCount);
		}

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batchVerticies.size() * sizeof(Vertex), batchVerticies.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batchIndecies.size() * sizeof(unsigned int), batchIndecies.data());
		
		bool rendered = false;
		bool renderedSpot = false;
		bool second = false;

		for (int j = 0; j < lights.size(); j++)
		{
			int type = lights[j]->GetType();
			if (type == 1)
			{
				DirectLight* light = static_cast<DirectLight*>(lights[j]);
				if (updateLight)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, fboAA);
					materials[i]->Use();
					std::string buff = "u_DirectLightColor[" + std::to_string(j) + "]";
					linmath::vec3 color = light->GetColor();
					glUniform4f(glGetUniformLocation(materials[i]->program, buff.c_str()), color.x, color.y, color.z, light->intensity);
					buff = "u_DirectLightDir[" + std::to_string(j) + "]";
					linmath::vec3 dir = light->GetVector();
					glUniform3f(glGetUniformLocation(materials[i]->program, buff.c_str()), dir.x, dir.y, dir.z);
				}
				Vector3D camPos = preview->GetPosition();
				glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
				glUseProgram(shadowProgram);
				glClear(GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowWidth, shadowHeight);
				glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "lightProj"), 1, GL_TRUE, light->proj);
				glUniform3f(glGetUniformLocation(shadowProgram, "u_Pos"), -camPos.x, -camPos.y, -camPos.z);
				glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);
				
				glBindFramebuffer(GL_FRAMEBUFFER, shadowRenderer);
				glUseProgram(shadowRProgram);
				glClear(GL_DEPTH_BUFFER_BIT);
				glUniformMatrix4fv(glGetUniformLocation(shadowRProgram, "u_Proj"), 1, GL_TRUE, proj);

				glUniformMatrix4fv(glGetUniformLocation(shadowRProgram, "lightProj"), 1, GL_TRUE, light->proj);
				glUniform3f(glGetUniformLocation(shadowRProgram, "u_Pos"), -camPos.x, -camPos.y, -camPos.z);
				glUniform1f(glGetUniformLocation(shadowRProgram, "u_Bias"), 0.05f);

				if (preview->update)
				{
					glUniform3f(glGetUniformLocation(shadowRProgram, "u_CamPos"), -camPos.x, -camPos.y, -camPos.z);
					glUniformMatrix4fv(glGetUniformLocation(shadowRProgram, "u_View"), 1, GL_TRUE, preview->rotMetricies);
				}
				glViewport(0, 0, 850, 600);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, shadowMap);
				glUniform1i(glGetUniformLocation(shadowRProgram, "u_ShadowMap"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, shadowRendererTDir);
				glUniform1i(glGetUniformLocation(shadowRProgram, "u_ShadowRender"), 1);
				glUniform1i(glGetUniformLocation(shadowRProgram, "u_HasPrevious"), rendered);

				glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);

				glBindFramebuffer(GL_READ_FRAMEBUFFER, shadowRenderer);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowRendererDir);
				glBlitFramebuffer(0, 0, 850, 600, 0, 0, 850, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

				glActiveTexture(GL_TEXTURE0);
				rendered = true;
			}
			if (type == 2)
			{
				PointLight* light = static_cast<PointLight*>(lights[j]);
				if (updateLight)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, fboAA);
					materials[i]->Use();
					std::string buff = "u_PointLightColor[" + std::to_string(j) + "]";
					linmath::vec3 color = light->GetColor();
					glUniform4f(glGetUniformLocation(materials[i]->program, buff.c_str()), color.x, color.y, color.z, light->intensity);
					buff = "u_PointLightPos[" + std::to_string(j) + "]";
					Vector3D posiion = light->GetPosition();
					glUniform4f(glGetUniformLocation(materials[i]->program, buff.c_str()), posiion.x, posiion.y, posiion.z, light->GetDistance());
				}
			}
			if (type == 3)
			{
				SpotLight* light = static_cast<SpotLight*>(lights[j]);
				Vector3D position = light->GetPosition();
				if (updateLight)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, fboAA);
					materials[i]->Use();
					std::string buff = "u_SpotLightColor[" + std::to_string(j) + "]";
					linmath::vec3 color = light->GetColor();
					glUniform4f(glGetUniformLocation(materials[i]->program, buff.c_str()), color.x, color.y, color.z, light->intensity);

					buff = "u_SpotLightPos[" + std::to_string(j) + "]";
					glUniform4f(glGetUniformLocation(materials[i]->program, buff.c_str()), position.x, position.y, position.z, light->GetDistance());

					buff = "u_SpotLightDir[" + std::to_string(j) + "]";
					linmath::vec3 dir = light->GetVector();
					glUniform3f(glGetUniformLocation(materials[i]->program, buff.c_str()), dir.x, dir.y, dir.z);

					buff = "u_SpotLightAngle[" + std::to_string(j) + "]";
					float angle = light->GetAngle();
					glUniform2f(glGetUniformLocation(materials[i]->program, buff.c_str()), std::cos(linmath::deg2radians(angle-5)),
						std::cos(linmath::deg2radians(angle)));
				}
				light->UpdateProj({0, 0, 0});

				glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
				glUseProgram(shadowProgram);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glViewport(0, 0, shadowWidth, shadowHeight);
				glUniformMatrix4fv(glGetUniformLocation(shadowProgram, "lightProj"), 1, GL_TRUE, light->proj);
				glUniform3f(glGetUniformLocation(shadowProgram, "u_Pos"), -position.x, -position.y, -position.z);
				glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);

				glBindFramebuffer(GL_FRAMEBUFFER, shadowRenderer1);
				glUseProgram(shadowRProgram);
				glClear(GL_DEPTH_BUFFER_BIT);
				glUniformMatrix4fv(glGetUniformLocation(shadowRProgram, "u_Proj"), 1, GL_TRUE, proj);

				glUniformMatrix4fv(glGetUniformLocation(shadowRProgram, "lightProj"), 1, GL_TRUE, light->proj);
				glUniform3f(glGetUniformLocation(shadowRProgram, "u_Pos"), -position.x, -position.y, -position.z);
				glUniform1f(glGetUniformLocation(shadowRProgram, "u_Bias"), 0.001f);

				if (preview->update)
				{
					Vector3D camPos = preview->GetPosition();
					glUniform3f(glGetUniformLocation(shadowRProgram, "u_CamPos"), -camPos.x, -camPos.y, -camPos.z);
					glUniformMatrix4fv(glGetUniformLocation(shadowRProgram, "u_View"), 1, GL_TRUE, preview->rotMetricies);
				}
				glViewport(0, 0, 850, 600);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, shadowMap);
				glUniform1i(glGetUniformLocation(shadowRProgram, "u_ShadowMap"), 0);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, shadowRendererTSpot);
				glUniform1i(glGetUniformLocation(shadowRProgram, "u_ShadowRender"), 1);
				glUniform1i(glGetUniformLocation(shadowRProgram, "u_HasPrevious"), renderedSpot);

				glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);

				glBindFramebuffer(GL_READ_FRAMEBUFFER, shadowRenderer1);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowRendererSpot);
				glBlitFramebuffer(0, 0, 850, 600, 0, 0, 850, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

				glActiveTexture(GL_TEXTURE0);
				renderedSpot = true;
			}
		}
		glBindFramebuffer(GL_FRAMEBUFFER, fboAA);
		materials[i]->Bind();

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, shadowRendererTDir);
		glUniform1i(glGetUniformLocation(materials[i]->program, "u_ShadowDir"), 6);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, shadowRendererTSpot);
		glUniform1i(glGetUniformLocation(materials[i]->program, "u_ShadowSpot"), 7);
		glActiveTexture(GL_TEXTURE0);

		glViewport(0, 0, 850, 600);
		glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);

		glBindFramebuffer(GL_READ_FRAMEBUFFER, fboAA);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
		glBlitFramebuffer(0, 0, 850, 600, 0, 0, 850, 600, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(program);
		glBindVertexArray(rectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glViewport(0, 0, width, height);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, fbt);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	preview->update = false;
	updateLight = true;
}

