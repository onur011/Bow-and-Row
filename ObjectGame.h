#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace ObjectGame
{
	// Creeaza arc
	Mesh* CreateBow(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);

	// Creeaza shuriken
	Mesh* CreateShuriken(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);

	// Creeaza sageata
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);

	// Creeaza balon
	Mesh* CreateBalloon(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);

	// Creeaza hp
	Mesh* CreateHp(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);

	// Creaeaza power bar
	Mesh* CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color);
}

