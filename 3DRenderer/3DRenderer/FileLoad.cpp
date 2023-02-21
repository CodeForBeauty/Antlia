#include <filesystem>
#include <sstream>
#include <fstream>
#include <cmath>
#include "FileLoad.h"
#include "linmath.h"

namespace fs = std::filesystem;


std::vector<Mesh*> load::loadObj(std::string filepath, Scene* scene)
{
	std::vector<linmath::vec3> verPos;
	std::vector<linmath::vec3> verNor;
	std::vector<linmath::vec2> verUV;

	std::vector<std::string> names;
	std::vector<int> materialIdx;
	std::vector<Vertex*> verticies;
	std::vector<unsigned int*> indecies;
	std::vector<unsigned int> verticiesCount;
	std::vector<unsigned int> indeciesCount;
	std::vector<bool> isSmooth;

	std::vector<Vertex> crntVert;
	std::vector<Vertex> crntMatVert;
	std::vector<unsigned int> crntInd;
	std::string crntName;

	std::string line;
	std::ifstream file(filepath);

	std::stringstream ss(filepath);
	std::string path;
	while (true)
	{
		getline(ss, line, '/');
		if (ss.eof()) break;
		path += line + "/";
	}

	std::string word;

	int objCount = 0;
	bool crntSmooth = true;

	int lastPosCount = 0;
	int lastNorCount = 0;
	int lastUVCount = 0;

	std::vector<Material*> materials;
	Material* crntMat = nullptr;

	std::vector<std::string> materialNames;

	while (getline(file, line))
	{
		if (line[0] != '#')
		{
			std::stringstream ss(line);
			ss >> word;
			if (word == "mtllib")
			{
				ss >> word;
				std::ifstream mtl(path + word);
				while (getline(mtl, line))
				{
					if (!line.empty())
					{
						std::stringstream ss(line);
						ss >> word;
						if (word == "newmtl")
						{
							ss >> word;
							crntMat = new Material();
							crntMat->SetName(word);
							scene->AddMaterial(crntMat);
							materialNames.push_back(word);
							materials.push_back(crntMat);
						}
						else if (word == "Ns")
						{
							ss >> word;
							float roughness = std::stof(word);
							roughness = 1 - (std::sqrt(roughness) * 0.03162277660168379);
							crntMat->SetRoughness(roughness);
						}
						else if (word == "Ka")
						{
							ss >> word;
							crntMat->SetMetalic(std::stof(word));
						}
						else if (word == "Kd")
						{
							linmath::vec3 color{};
							ss >> word;
							color.x = std::stof(word);
							ss >> word;
							color.y = std::stof(word);
							ss >> word;
							color.z = std::stof(word);
							crntMat->SetAlbedo(color.x, color.y, color.z, 1.0f);
						}
						else if (word == "Ks")
						{
							ss >> word;
							crntMat->SetSpecular(std::stof(word));
						}
						else if (word == "illum")
						{
							ss >> word;
							if (std::stoi(word) < 3)
							{
								crntMat->SetMetalic(0.0f);
							}
						}
						else if (word == "map_Kd")
						{
							ss >> word;
							fs::path tex(word);
							if (tex.is_relative())
							{
								word = path + word;
							}
							crntMat->LoadTexture(word.c_str(), TEXTURE_ALBEDO);
						}
						else if (word == "map_Ks")
						{
							ss >> word;
							fs::path tex(word);
							if (tex.is_relative())
							{
								word = path + word;
							}
							crntMat->LoadTexture(word.c_str(), TEXTURE_SPECULAR);
						}
						else if (word == "map_Ns")
						{
							ss >> word;
							fs::path tex(word);
							if (tex.is_relative())
							{
								word = path + word;
							}
							crntMat->LoadTexture(word.c_str(), TEXTURE_ROUGHNESS);
						}
						else if (word == "map_refl")
						{
							ss >> word;
							fs::path tex(word);
							if (tex.is_relative())
							{
								word = path + word;
							}
							crntMat->LoadTexture(word.c_str(), TEXTURE_METALIC);
						}
						else if (word == "map_Bump")
						{
							ss >> word;
							if (word == "-bm")
								ss >> word; ss >> word;
							fs::path tex(word);
							if (tex.is_relative())
							{
								word = path + word;
							}
							crntMat->LoadTexture(word.c_str(), TEXTURE_NORMAL);
						}
					}
				}
			}
			else if (word == "o")
			{
				objCount++;
				ss >> word;
				names.push_back(word);
				crntName = word;
				lastPosCount += verPos.size();
				lastNorCount += verNor.size();
				lastUVCount += verUV.size();
				verPos.clear();
				verNor.clear();
				verUV.clear();
				if (!crntVert.empty())
				{
					Vertex* data = new Vertex[crntVert.size()];
					copy(crntVert.begin(), crntVert.end(), data);
					unsigned int* dataInd = new unsigned int[crntInd.size()];
					copy(crntInd.begin(), crntInd.end(), dataInd);
					verticies.push_back(data);
					indecies.push_back(dataInd);
					verticiesCount.push_back(crntVert.size());
					indeciesCount.push_back(crntInd.size());
					isSmooth.push_back(crntSmooth);
				}
				crntSmooth = true;
				crntVert.clear();
				crntInd.clear();
			}
			else if (word == "usemtl")
			{
				ss >> word;
				auto it = std::find(materialNames.begin(), materialNames.end(), word);
				materialIdx.push_back(it - materialNames.begin());
				
				if (!crntInd.empty())
				{
					objCount++;
					names.push_back(crntName + word);

					Vertex* data = new Vertex[crntVert.size()];
					copy(crntVert.begin(), crntVert.end(), data);
					unsigned int* dataInd = new unsigned int[crntInd.size()];
					copy(crntInd.begin(), crntInd.end(), dataInd);

					verticies.push_back(data);
					indecies.push_back(dataInd);
					verticiesCount.push_back(crntVert.size());
					indeciesCount.push_back(crntInd.size());
					isSmooth.push_back(crntSmooth);
					crntInd.clear();
				}
			}
			else if (word == "f")
			{
				for (int i = 0; i < 3; i++)
				{
					ss >> word;
					std::stringstream index(word);
					std::string vind;
					std::string vnind;
					std::string vtind;
					Vertex vertex{};

					linmath::vec3 position{};
					linmath::vec3 normal{};
					linmath::vec2 uv{};
					int crnt = 0;

					std::getline(index, vind, '/');
					crnt = std::stoi(vind) - lastPosCount;
					position = verPos[crnt-1];
					if (!index.eof())
					{
						std::getline(index, vtind, '/');
						if (vtind.empty())
							uv = { 0, 0 };
						else
						{
							crnt = std::stoi(vtind) - lastUVCount;
							uv = verUV[crnt-1];
						}
						if (!index.eof())
						{
							std::getline(index, vnind, '/');
							if (vnind.empty())
							{
								normal = { 0, 0, 0 };
							}
							else
								normal = verNor[std::stoi(vnind) - lastNorCount - 1];
						}
						else
							normal = { 0, 0, 0 };
					}
					else
					{
						uv = { 0, 0 };
						normal = { 0, 0, 0 };
					}
					crntInd.push_back(crnt - 1);
					crntVert[crnt - 1].position = position;
					crntVert[crnt - 1].normal = normal;
					crntVert[crnt - 1].uv = uv;
				}
			}
			else if (word == "s")
			{
				ss >> word;
				crntSmooth = word == "1";
			}
			else if (word == "vt")
			{
				linmath::vec2 uv{};
				ss >> word;
				uv.x = std::stof(word);
				ss >> word;
				uv.y = std::stof(word);
				verUV.push_back(uv);
				if (verUV.size() > crntVert.size())
					crntVert.push_back({});
			}
			else if (word == "vn")
			{
				linmath::vec3 normal{};
				ss >> word;
				normal.x = std::stof(word);
				ss >> word;
				normal.y = std::stof(word);
				ss >> word;
				normal.z = std::stof(word);
				verNor.push_back(normal);
			}
			else if (word == "v")
			{
				linmath::vec3 vertex{};
				ss >> word;
				vertex.x = std::stof(word);
				ss >> word;
				vertex.y = std::stof(word);
				ss >> word;
				vertex.z = std::stof(word);
				verPos.push_back(vertex);
				crntVert.push_back({});
			}
		}
	}
	file.close();

	Vertex* data = new Vertex[crntVert.size()];
	copy(crntVert.begin(), crntVert.end(), data);
	unsigned int* dataInd = new unsigned int[crntInd.size()];
	copy(crntInd.begin(), crntInd.end(), dataInd);
	verticies.push_back(data);
	indecies.push_back(dataInd);
	verticiesCount.push_back(crntVert.size());
	indeciesCount.push_back(crntInd.size());
	isSmooth.push_back(crntSmooth);

	std::vector<Mesh*> meshes;

	for (int i = 0; i < names.size(); i++)
	{
		Mesh* temp = new Mesh(verticies[i], indecies[i], verticiesCount[i], indeciesCount[i]);
		temp->SetName(names[i]);
		temp->isSmooth = isSmooth[i];
		scene->AddObject(temp);
		meshes.push_back(temp);
		if (materialIdx.size() > i)
		{
			scene->SetObjectMaterial(temp, materials[materialIdx[i]]);
		}
	}
	return meshes;
}

bool load::SaveScene(std::string filepath, Scene* scene)
{
	std::ofstream file;
	file.open(filepath);

	std::string line;
	line = std::to_string(scene->preview->fov) + " " + std::to_string(scene->preview->near) + " " + std::to_string(scene->preview->far) + "\n";
	file << line;
	Vector3D pos = scene->preview->GetPosition();
	Vector3D rot = scene->preview->GetRotation();
	line = std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
	file << line;
	line = std::to_string(rot.x) + " " + std::to_string(rot.y) + " " + std::to_string(rot.z) + "\n";
	file << line;

	line = std::to_string(scene->renderCamera->fov) + " " + std::to_string(scene->renderCamera->near) + " " + std::to_string(scene->renderCamera->far) + "\n";
	file << line;
	pos = scene->renderCamera->GetPosition();
	rot = scene->renderCamera->GetRotation();
	line = std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + "\n";
	file << line;
	line = std::to_string(rot.x) + " " + std::to_string(rot.y) + " " + std::to_string(rot.z) + "\n";
	file << line;

	linmath::vec3 color;
	for (Light* light : scene->lights)
	{
		int type = light->GetType();
		line = std::to_string(type) + " ";
		color = light->GetColor();
		line += std::to_string(color.x) + " " + std::to_string(color.y) + " " + std::to_string(color.z) + " ";
		line += light->GetName() + "\n";
		file << line;
		if (type == 1)
		{
			DirectLight* dirLight = reinterpret_cast<DirectLight*>(light);
			rot = dirLight->GetRotation();
			line = std::to_string(rot.x) + " " + std::to_string(rot.y) + " " + std::to_string(rot.z) + "\n";
			file << line;
		}
		else if (type == 2)
		{
			PointLight* point = reinterpret_cast<PointLight*>(light);
			pos = point->GetPosition();
			line = std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + " " + std::to_string(point->GetDistance()) + "\n";
			file << line;
		}
		else if (type == 3)
		{
			SpotLight* spot = reinterpret_cast<SpotLight*>(light);
			pos = spot->GetPosition();
			rot = spot->GetRotation();
			line = std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z) + " " + std::to_string(spot->GetDistance()) + "\n";
			file << line;
			line = std::to_string(rot.x) + " " + std::to_string(rot.y) + " " + std::to_string(rot.z) + " " + std::to_string(spot->GetAngle()) + "\n";
			file << line;
		}
	}
	file << "mat\n";

	for (Material* mat : scene->materials)
	{
		float* albedo = mat->GetAlbedo();
		line = std::to_string(albedo[0]) + " " + std::to_string(albedo[1]) + " " + std::to_string(albedo[2]) + " " + std::to_string(albedo[3]) + "\n";
		file << line;
		file << mat->GetMetalic() << " " << mat->GetRoughness() << " " << mat->GetSpecular() << " " << mat->GetName() << "\n";
		std::string textures;
		int count = 0;
		if (mat->texture)
			textures += "1 " + mat->texture->path + "\n"; count++;
		if (mat->metalTex)
			textures += "2 " + mat->metalTex->path + "\n"; count++;
		if (mat->specTex)
			textures += "3 " + mat->specTex->path + "\n"; count++;
		if (mat->roughTex)
			textures += "4 " + mat->roughTex->path + "\n"; count++;
		if (mat->normal)
			textures += "5 " + mat->normal->path + "\n"; count++;
		file << count << "\n" << textures;
	}
	file << "entity\n";

	Vector3D scale;
	for (Entity* entity : scene->entities)
	{
		pos = entity->GetPosition();
		rot = entity->GetRotation();
		scale = entity->GetScale();

		file << pos.x << " " << pos.y << " " << pos.z << "\n";
		file << rot.x << " " << rot.y << " " << rot.z << "\n";
		file << scale.x << " " << scale.y << " " << scale.z << "\n";
		file << entity->GetName() << "\n";
	}


	return true;
}