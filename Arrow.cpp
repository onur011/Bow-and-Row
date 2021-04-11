#include "ObjectGame.h"

#include <Core/Engine.h>
#include "Tema1.h"
#include "Transform2D.h"
#include <Component/SimpleScene.h>

Mesh* ObjectGame::CreateArrow(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;
	// Se genereaza sageata
	std::vector<VertexFormat> vertices = {
		
		VertexFormat(corner + glm::vec3(0, 20, 0), color),
		VertexFormat(corner + glm::vec3(200, 30, 0), color),
		VertexFormat(corner + glm::vec3(200, 20, 0), color),
		VertexFormat(corner + glm::vec3(200, 0, 0), color),
		VertexFormat(corner + glm::vec3(240, 25, 0), color),
		VertexFormat(corner + glm::vec3(200, 50, 0), color),
		VertexFormat(corner + glm::vec3(200, 30, 0), color),
		VertexFormat(corner + glm::vec3(0, 30, 0), color),
		VertexFormat(corner + glm::vec3(0, 20, 0), color)
		
	};

	std::vector<unsigned short> indices = { 0,1,2,3,4,5,6,7, 8 };

	Mesh* arrow = new Mesh(name);
	arrow->InitFromData(vertices, indices);
	return arrow;
}

void Tema1::SpawnArrow(float deltaTimeSeconds) {
	int i;
	glm::ivec2 resolution = window->GetResolution();

	// Afisarea sagetilor
	for (i = 0; i < arrows.size(); i++) {
		float cos_arrow = cos(arrows[i].radians_arrow) *
			arrows[i].power;
		float sin_arrow = sin(arrows[i].radians_arrow)*
			arrows[i].power;

		// Sageata se verifica daca se regaseste pe ecran
		// Nu se permite tragerea in spatele arcului
		if (cos_arrow > resolution.x || sin_arrow > resolution.y ||
			cos_arrow < 0 || sin_arrow < -resolution.y) {
			arrows.erase(arrows.begin() + i);
			i--;
			continue;
		}
		modelMatrix = glm::mat3(1);

		//Se translateaza utilizand functiile sin si cos pentru 
		//a creea efectul de translatare liniara
		modelMatrix *= Transform2D::Translate(cos_arrow, sin_arrow);
		// Se translateaza la nivelul arcului din momentul in care s-a efectuat tragerea
		modelMatrix *= Transform2D::Translate(0, arrows[i].ty);
		//Se translateaza la distanta mijlocului arcului fata de axa Oy
		modelMatrix *= Transform2D::Translate(bow_distance_x, 0);
		//Se roteste obiectul
		modelMatrix *= Transform2D::Rotate(arrows[i].radians_arrow);
		//Se modifica dimensiunile sagetii
		modelMatrix *= Transform2D::Scale(0.8f, 0.2f);
		//Se centreaza obiectul astfel incat sa fie simetric fata de OX
		modelMatrix *= Transform2D::Translate(0, -25);

		// Se afiseaza obiectul
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrix);

		arrows[i].power += deltaTimeSeconds * 20 * arrows[i].power_initial;

	}
}