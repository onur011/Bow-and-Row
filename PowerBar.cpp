#include "ObjectGame.h"
#include "Tema1.h"
#include <Core/Engine.h>
#include <Component/SimpleScene.h>
#include "Transform2D.h"

Mesh* ObjectGame::CreatePowerBar(std::string name, glm::vec3 leftBottomCorner, glm::vec3 color)
{
	
	float radius = 10;
	int ind = 0;
	// Se construieste un dreptunghi
	glm::vec3 corner = leftBottomCorner;
	std::vector<VertexFormat> vertices = {

		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(70, 0, 0), color),
		VertexFormat(corner + glm::vec3(70, 15, 0), color),
		VertexFormat(corner + glm::vec3(0, 15, 0), color),
	};

	std::vector<unsigned short> indices = { 0,1,2,3 };

	Mesh* powerBar = new Mesh(name);	
	powerBar->SetDrawMode(GL_POLYGON);

	powerBar->InitFromData(vertices, indices);
	return powerBar;
}

void Tema1::SpawnPowerBar() {
	glm::ivec2 resolution = window->GetResolution();
	if (power > min_power) {
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(bow_distance_x, resolution.y / 2 + bow_ty - 120);
		// Se scaleaza in functie de putere
		modelMatrix *= Transform2D::Scale(power / max_power, 1.0f);

		RenderMesh2D(meshes["powerbar"], shaders["VertexColor"], modelMatrix);
	}
}
