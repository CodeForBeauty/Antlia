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