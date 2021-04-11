#include "ObjectGame.h"

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Tema1.h"

Mesh* ObjectGame::CreateBalloon(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
	
	float pi = 3.1415f, radius = 50;
	int ind = 0;
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices;

	std::vector<unsigned short> indices;
	
	// Generare sfoara balon
	vertices.push_back(VertexFormat(corner, color));
	vertices.push_back(VertexFormat(corner + glm::vec3(30,30,0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(25,30,0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(0, 55, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(0, 54, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(17, 90, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(12, 90, 0), color));

	// Generare triunghi baza
	vertices.push_back(VertexFormat(corner + glm::vec3(-5, 90, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(35, 90, 0), color));
	vertices.push_back(VertexFormat(corner + glm::vec3(15, 120, 0), color));

	indices = { 0,1,2, //sfoara
				1,2,3, //sfoara
				4,5,6,  //sfoara
				7,8,9	//triunghi baza
	};
	ind = 9;
	// Se genereaza elipsa 
	for (float i = 0; i <= 4*3.1415f; i += 0.001f) {
		vertices.push_back(VertexFormat(corner + glm::vec3(15,175,0) +
			glm::vec3(cos(i)*radius, 1.5f*sin(i)*radius, 0), color));
		float x = i + 0.001f;
		vertices.push_back(VertexFormat(corner + glm::vec3(15, 175, 0) + 
			glm::vec3(cos(x)*radius, 1.5f*sin(x)*radius, 0), color));
		indices.push_back(ind);
		ind++;
		indices.push_back(ind);
		indices.push_back(9);
		}

	Mesh* balloon = new Mesh(name);
	
	balloon->InitFromData(vertices, indices);
	return balloon;
}

void Tema1::CollisionArrow(float deltaTimeSeconds) {
	int i, j;
	// Verificare coliziune sageata - balon,shuriken
	for (i = 0; i < arrows.size(); i++) {
		// Se extrage pozitia varfului sagetii
		float x_arrow = bow_distance_x + cos(arrows[i].radians_arrow) *
			(arrows[i].power + 0.8f * 240);
		float y_arrow = arrows[i].ty + sin(arrows[i].radians_arrow)*
			(arrows[i].power + 0.8f * 240);
		// Se verifica coliziunea sageata - balon
		for (j = 0; j < balloons.size(); j++) {
			// +15 pentru a ajunge in centru balonului
			float center_x = balloons[j].tx + 15;
			// 90 pentru a ajunge la baza triunghiului
			// 15 pentru a ajunge la locul de unde incepe elipsa
			// 75 pentru a ajunge in mijlocul elipsei(raza)
			float center_y = balloons[j].ty + 90 + 75 + 15;
			float diameter_x = 100 / 2;
			float diameter_y = 150 / 2;

			// Se verifica coliziunea
			if (Collision(x_arrow, y_arrow, center_x, center_y,
				diameter_x, diameter_y, 0)) {
				balloons[j].sx = 1 - deltaTimeSeconds;
				balloons[j].sy = 1 - deltaTimeSeconds;

				// Daca este un balon galben, scorul scade.
				if (balloons[j].color == 0) {
					if (score > balloons[j].speed) {
						score -= (int)balloons[j].speed;
					}
				}
				// Daca e un balon rosu, scorul creste
				else {
					score += (int)balloons[j].speed;
				}
				
				printf("Score : %d\n", score);

				bang_balloons.push_back(balloons[j]);
				balloons.erase(balloons.begin() + j);
				j--;
				continue;
			}
		}

		// Se verifica coliziunea sageata - shuriken
		for (j = 0; j < shurikens.size(); j++) {
			// Se calculeaza centrul shurikenului
			float cx_shu = shurikens[j].tx + (float)sqrt(2) * 0.7f * 
				shuriken_length / 2;
			float cy_shu = shurikens[j].ty + (float)sqrt(2) * 0.7f * 
				shuriken_length / 2;

			// Se calculeaza diametrul
			float d = 2.0f * (float)sqrt(2) * 0.7f * shuriken_length / 2.0f;

			// Se verifica coliziunea
			if (Collision(x_arrow, y_arrow, cx_shu, cy_shu, d /2.0f , d/2.0f , 0)) {
				
				score += (int)shurikens[j].speed;
				printf("Score : %d\n", score);

				bang_shu.push_back(shurikens[j]);
				shurikens.erase(shurikens.begin() + j);
				j--;
				continue;
			}
		}
	}

}

void Tema1::BangBalloon(float deltaTimeSeconds) {
	int i;
	glm::ivec2 resolution = window->GetResolution();
	
	// Animatie spargere balon
	for (i = 0; i < bang_balloons.size(); i++) {
		modelMatrix = glm::mat3(1);

		// Daca iese din ecran nu se mai afiseaza
		if (bang_balloons[i].ty > resolution.y) {
			bang_balloons.erase(bang_balloons.begin() + i);
			i--;
			continue;
		}

		// Se scleaza subunitar pana la o anumita valoare
		modelMatrix *= Transform2D::Translate(bang_balloons[i].tx +
			bang_balloons[i].trace * sin(bang_balloons[i].rad_trace),
			bang_balloons[i].ty);

		modelMatrix *= Transform2D::Translate(15, 90 + 75 + 15);
		modelMatrix *= Transform2D::Scale(bang_balloons[i].sx, bang_balloons[i].sy);
		modelMatrix *= Transform2D::Translate(-15, -90 - 75 - 15);

		if (bang_balloons[i].sx > 0.5f || bang_balloons[i].sy > 0.5f) {
			bang_balloons[i].sx -= deltaTimeSeconds;
			bang_balloons[i].sy -= deltaTimeSeconds;
		}
		// Dupa ce s-a ajuns la acea valoarea, balonul urca exponential
		else {
			if (bang_balloons[i].ty == 0) {
				bang_balloons[i].ty = 1;
			}
			bang_balloons[i].ty += bang_balloons[i].ty * 5 * deltaTimeSeconds;
		}


		if (bang_balloons[i].color == 0) {
			RenderMesh2D(meshes["balloon_y"], shaders["VertexColor"], modelMatrix);
		}
		else {
			RenderMesh2D(meshes["balloon_r"], shaders["VertexColor"], modelMatrix);
		}
	}
}

// Se genereaza baloanele
void Tema1::GenerateBalloons() {

	glm::ivec2 resolution = window->GetResolution();
	//Cazul in care jocul este bagat in bara
	if (resolution.x == 0) {
		return;
	}
	Balloons aux;
	// Se genereaza un numar aleator de baloane
	// Pentru fiecare balon se genereaza pozitia in jumatatea din 
	// dreapta a ecranului, viteza, culoare si traiectoria random.
	int i, number_balloons = Random(1, 4);
	for (i = 0; i < number_balloons; i++) {
		aux.tx = (float)Random(resolution.x / 2, resolution.x - 50);
		aux.ty = 0;
		aux.speed = (float)Random(100, 500);
		aux.color = Random(0, 1);
		aux.trace = (float)Random(10, 50);
		aux.rad_trace = 0;
		balloons.push_back(aux);
	}
}

void Tema1::SpawnBalloons(float deltaTimeSeconds) {
	// Afisarea baloane
	int i;
	
	glm::ivec2 resolution = window->GetResolution();

	for (i = 0; i < balloons.size(); i++) {
		modelMatrix = glm::mat3(1);

		// Daca balonul iese din ecran, acesta este eliminat
		if (balloons[i].ty > resolution.y) {
			balloons.erase(balloons.begin() + i);
			i--;
			continue;
		}

		// Se translateaza la coordonata dorita
		modelMatrix *= Transform2D::Translate(balloons[i].trace*sin(balloons[i].rad_trace), 0);
		modelMatrix *= Transform2D::Translate(balloons[i].tx, balloons[i].ty);

		// Se alege culoare balonului
		if (balloons[i].color == 0) {
			RenderMesh2D(meshes["balloon_y"], shaders["VertexColor"], modelMatrix);
		} else {
			RenderMesh2D(meshes["balloon_r"], shaders["VertexColor"], modelMatrix);
		}

		balloons[i].ty += balloons[i].speed*deltaTimeSeconds;
		balloons[i].rad_trace += 2 * deltaTimeSeconds;
	}
}