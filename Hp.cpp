#include "ObjectGame.h"
#include "Tema1.h"
#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include "Transform2D.h"

Mesh* ObjectGame::CreateHp(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
	
	float radius = 10, i;
	int ind = 0;
	glm::vec3 corner = leftBottomCorner;
	std::vector<unsigned short> indices;
	std::vector<VertexFormat> vertices;
	// Se genereaza doua semicercuri
	for (i = 0; i < 3.1415f; i += 0.001f) {

		indices.push_back(ind);
		ind++;
		vertices.push_back(VertexFormat(corner + glm::vec3(2*radius, 0, 0) + glm::vec3(cos(i)*radius, sin(i)*radius, 0), color));
	}

	for (i = 0; i <= 3.1415f; i += 0.001f) {

		indices.push_back(ind);
		ind++;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(i)*radius, sin(i)*radius, 0), color));
	}
	
	// Se adauga inca un punct, reprezentand varful inimii
	indices.push_back(ind);
	vertices.push_back(VertexFormat(corner + glm::vec3( radius, -3*radius, 0), color));


	Mesh* hp = new Mesh(name);

	hp->SetDrawMode(GL_LINE_LOOP);
	

	hp->InitFromData(vertices, indices);
	return hp;
}

void Tema1::SpawnHp() {
	int i;
	float next = 50;
	glm::ivec2 resolution = window->GetResolution();

	// Se afiseaza fiecare hp points
	for (i = 0; i < hp; i++) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(resolution.x * 0.2f + next*i, resolution.y*0.95f);
		RenderMesh2D(meshes["hp"], shaders["VertexColor"], modelMatrix);
	}

}
