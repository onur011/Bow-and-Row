#include "ObjectGame.h"
#include "Tema1.h"
#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include "Transform2D.h"

Mesh* ObjectGame::CreateShuriken(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
	
	float length = 50;
	int i;

	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices = {
		VertexFormat(corner , color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(2 * length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(2 * length, length, 0), color),
		VertexFormat(corner + glm::vec3(2 * length, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(length, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(0, 2 * length, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	std::vector<unsigned short> indices;

	for (i = 0; i < 12; i++) {
		indices.push_back(i);
	}

	Mesh* shuriken = new Mesh(name);
	

	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

// Se genereaza shuriken
void Tema1::GenerateShurikens() {
	glm::ivec2 resolution = window->GetResolution();

	Shurikens aux;
	// Se genereaza un numar aleator de shuriken
	// Pentru fiecare shuriken se genereaza pozitia pe axa Oy
	// viteaza  si viteza de rotatie aleatoriu
	int i, number_shurikens = Random(1, 3);
	for (i = 0; i < number_shurikens; i++) {
		aux.ty = (float)Random(0, resolution.y - 50);
		aux.tx = (float)resolution.x;
		aux.speed_rotation = (float)Random(2, 8);
		aux.speed = (float)Random(100, 500);
		aux.radians = 0;
		shurikens.push_back(aux);
	}
}

void Tema1::SpawnShurikens(float deltaTimeSeconds) {
	// Afisarea shuriken
	int i;
	for (i = 0; i < shurikens.size(); i++) {
		modelMatrix = glm::mat3(1);

		// Daca balonul iese din ecran, acesta este eliminat
		if (shurikens[i].tx < 0) {
			shurikens.erase(shurikens.begin() + i);
			i--;
			continue;
		}

		// Se translateaza la coordonata dorita
		modelMatrix *= Transform2D::Translate(shurikens[i].tx, shurikens[i].ty);

		// Se translateaza cu coltul din stanga jos in origine
		modelMatrix *= Transform2D::Translate(0.7f*shuriken_length / 2, 0.7f*shuriken_length / 2);
		// Se roteste
		modelMatrix *= Transform2D::Rotate(shurikens[i].radians);
		// Se scaleaza
		modelMatrix *= Transform2D::Scale(0.7f, 0.7f);
		// Se translateaza cu mijlocul in origine
		modelMatrix *= Transform2D::Translate(-shuriken_length / 2, -shuriken_length / 2);

		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);

		shurikens[i].tx -= shurikens[i].speed*deltaTimeSeconds;
		shurikens[i].radians += shurikens[i].speed_rotation*deltaTimeSeconds;
	}
}

void Tema1::BangShuriken(float deltaTimeSeconds) {
	int i;
	
	for (i = 0; i < bang_shu.size(); i++) {
		
		// Daca nu se afla pe ecran, shuriken este eliminat
		if (bang_shu[i].ty < 0) {
			bang_shu.erase(bang_shu.begin() + i);
			i--;
			continue;
		}
			
		modelMatrix = glm::mat3(1);
		// Se translateaza la coordonata dorita
		modelMatrix *= Transform2D::Translate(bang_shu[i].tx, bang_shu[i].ty);

		// Se translateaza cu coltul din stanga jos in origine
		modelMatrix *= Transform2D::Translate(0.7f*shuriken_length / 2, 0.7f*shuriken_length / 2);
		// Se roteste
		modelMatrix *= Transform2D::Rotate(bang_shu[i].radians);
		// Se scaleaza
		modelMatrix *= Transform2D::Scale(0.7f, 0.7f);
		// Se translateaza cu mijlocul in origine
		modelMatrix *= Transform2D::Translate(-shuriken_length / 2, -shuriken_length / 2);

		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrix);
		
		// Se incetineste rotirea
		bang_shu[i].speed_rotation -= bang_shu[i].speed_rotation * deltaTimeSeconds;
		bang_shu[i].radians += bang_shu[i].speed_rotation*deltaTimeSeconds;
	
		// Incepe sa cada
		bang_shu[i].speed += bang_shu[i].speed * deltaTimeSeconds;
		bang_shu[i].ty -= bang_shu[i].speed * deltaTimeSeconds;
				

	}
}