#pragma once

#include <string>
#include <vector>
#include "Scene.h"
#include "Objects.h"

namespace load
{
	std::vector<Mesh*> loadObj(std::string filepath, Scene* scene);
	bool SaveScene(std::string filepath, Scene* scene);
}