#include "ObjectGame.h"

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include "Transform2D.h"
#include "Tema1.h"

Mesh* ObjectGame::CreateBow(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
	
	float pi = 3.1415f, i, radius = 100;
	int ind = 0;
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices;

	std::vector<unsigned short> indices;
	// Se genereaza arcul de cerc si vectorul cu indici
	for (i = -3.1415f/2; i <= 3.1415f/2; i += 0.001f){

		indices.push_back(ind);
		ind++;
		vertices.push_back(VertexFormat(corner + glm::vec3(cos(i)*radius, sin(i)*radius, 0), color));
	}

	Mesh* bow = new Mesh(name);
	

	bow->SetDrawMode(GL_LINE_LOOP);

	bow->InitFromData(vertices, indices);
	return bow;
}

void Tema1::SpawnBow() {
	glm::ivec2 resolution = window->GetResolution();
	modelMatrix = glm::mat3(1);
	// Afisare arc in centru stanga
	modelMatrix *= Transform2D::Translate(bow_distance_x, resolution.y / 2.0f);
	modelMatrix *= Transform2D::Translate(0, bow_ty);
	// Se roteste dupa mouse
	modelMatrix *= Transform2D::Rotate(radians_mouse);
	modelMatrix *= Transform2D::Scale(scalex_bow, scaley_bow);

	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrix);

}

void Tema1::CollisionBow() {
	int i;
	// Se verifica fiecare shuriken daca intra in contact cu arcul
	for (i = 0; i < shurikens.size(); i++) {
		if (CheckBowCollision(shurikens[i])) {
			shurikens.erase(shurikens.begin() + i);
			i--;
			hp--;
			continue;
		}
	}
}

bool Tema1::CheckBowCollision(Shurikens s) {

	glm::ivec2 resolution = window->GetResolution();

	float i;

	// Se determina centrul shurikenului
	float cx_shu = s.tx + 1.14f * 0.7f * shuriken_length / 2;
	float cy_shu = s.ty + 1.14f * 0.7f * shuriken_length / 2;

	// Se ia fiecare punct de pe cercul descris de shuriken si se verfica daca
	// punctul se afla in interiorul arcului.
	for (i = 0; i < 2 * 3.14f; i += 0.01f) {
		float x = cos(i)* 0.7f * shuriken_length / 2 + cx_shu;
		float y = sin(i)* 0.7f * shuriken_length / 2 + cy_shu;
		//Se adauga 30 pentru a ajunge in centrul arcului
		float cx = bow_distance_x + 30;
		float cy = resolution.y / 2.0f + bow_ty;
		float mx = 30;
		float my = 100;

		if (Collision(x, y, cx, cy, mx, my, radians_mouse)) {
			return true;
		}
	}

	return false;
}