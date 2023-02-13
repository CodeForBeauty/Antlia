#include <sstream>
#include <fstream>
#include "FileLoad.h"
#include "linmath.h"

std::vector<Mesh*> load::loadObj(std::string filepath, Scene* scene)
{
	std::vector<linmath::vec3> verPos;
	std::vector<linmath::vec3> verNor;
	std::vector<linmath::vec2> verUV;

	std::vector<std::string> names;
	std::vector<Vertex*> verticies;
	std::vector<unsigned int*> indecies;
	std::vector<unsigned int> verticiesCount;
	std::vector<unsigned int> indeciesCount;

	std::vector<Vertex> crntVert;
	std::vector<unsigned int> crntInd;

	std::string line;
	std::ifstream file(filepath);

	std::string word;

	int objCount = 0;

	int lastPosCount = 0;
	int lastNorCount = 0;
	int lastUVCount = 0;


	while (getline(file, line))
	{
		if (line[0] != '#')
		{
			std::stringstream ss(line);
			ss >> word;
			if (word == "o")
			{
				objCount++;
				ss >> word;
				names.push_back(word);
				lastPosCount += verPos.size();
				lastNorCount += verNor.size();
				lastUVCount += verUV.size();
				verPos.clear();
				verNor.clear();
				verUV.clear();
				if (!crntVert.empty())
				{
					verticies.push_back(crntVert.data());
					indecies.push_back(crntInd.data());
					verticiesCount.push_back(crntVert.size());
					indeciesCount.push_back(crntInd.size());
				}
				crntVert.clear();
				crntInd.clear();
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
					crntInd.push_back(crnt-1);
					crntVert[crnt-1].position = position;
					crntVert[crnt-1].normal = normal;
					crntVert[crnt-1].uv = uv;
				}
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

	verticies.push_back(crntVert.data());
	indecies.push_back(crntInd.data());
	verticiesCount.push_back(crntVert.size());
	indeciesCount.push_back(crntInd.size());

	std::vector<Mesh*> meshes;

	for (int i = 0; i < names.size(); i++)
	{
		Mesh* temp = new Mesh(verticies[i], indecies[i], verticiesCount[i], indeciesCount[i]);
		temp->SetName(names[i]);
		std::cout << verticiesCount[i];
		scene->AddObject(temp);
		meshes.push_back(temp);
	}
	return meshes;
}