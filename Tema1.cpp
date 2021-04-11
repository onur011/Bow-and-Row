#include "Tema1.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
#include <time.h> 

#include <Core/Engine.h>
#include "ObjectGame.h"
#include "Transform2D.h"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	srand((unsigned int)time(NULL));

	glm::vec3 corner = glm::vec3(0, 0, 0);
	bow_ty = 0;
	radians_mouse = 0;
	power = min_power;
	res_balloons = 0;
	time_arrow = 0;
	score = 0;
	hp = 3;

	// Se creeaza arcul
	Mesh* bow = ObjectGame::CreateBow("bow", corner, glm::vec3(0, 1, 0));
	AddMeshToList(bow);

	// Se creeaza shurikenul
	Mesh* shuriken = ObjectGame::CreateShuriken("shuriken", corner, glm::vec3(0, 1, 1));
	AddMeshToList(shuriken);

	// Se creeza sageata
	Mesh* arrow = ObjectGame::CreateArrow("arrow", corner, glm::vec3(0, 0, 1));
	AddMeshToList(arrow);

	// Se creeaza balonul galben
	Mesh* balloon_y = ObjectGame::CreateBalloon("balloon_y", corner, glm::vec3(1, 1, 0));
	AddMeshToList(balloon_y);
	
	// Se creeaza balonul rosu
	Mesh* balloon_r = ObjectGame::CreateBalloon("balloon_r", corner, glm::vec3(1, 0, 0));
	AddMeshToList(balloon_r);

	// Se creeaza hp
	Mesh* hp_mesh = ObjectGame::CreateHp("hp", corner, glm::vec3(1, 0, 0));
	AddMeshToList(hp_mesh);

	// Se creeaza power bar
	Mesh* powerBar = ObjectGame::CreatePowerBar("powerbar", corner, glm::vec3(0, 1, 0));
	AddMeshToList(powerBar);
}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Se seteaza aria de desenar
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->GetResolution();

	res_balloons += deltaTimeSeconds;
	res_shurikens += deltaTimeSeconds;
	time_arrow += deltaTimeSeconds;

	// Afisare hp
	SpawnHp();

	// Afisare power bar
	SpawnPowerBar();

	// Afisare arc
	SpawnBow();

	// Verificare coliziune shuriken - arc
	CollisionBow();
	
	// Coliziune sageata - balon - shuriken
	CollisionArrow(deltaTimeSeconds);

	// Animatie baloane sparte
	BangBalloon(deltaTimeSeconds);

	// Animatie shuriken doborat
	BangShuriken(deltaTimeSeconds);

	// Afisare sageti
	SpawnArrow(deltaTimeSeconds);

	// Afisare baloane
	SpawnBalloons(deltaTimeSeconds);

	// Afisare shuriken
	SpawnShurikens(deltaTimeSeconds);
	
	// Se spawneaza baloanele la un interval de 2 secunde
	if (res_balloons > 2) {
		res_balloons = 0;
		GenerateBalloons();
	}

	// Se spawneaza shuriken la un interval de 3 secunde
	if (res_shurikens > 3) {
		res_shurikens = 0;
		GenerateShurikens();
	}

	// Se verifica daca jocul s-a incheiat
	if (hp == 0) {
		Exit();
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	// Se verifica daca deltaTime este in parametrii.
	// De obiecei, la deschiderea programului, deltaTime are 
	// o valoare foarte mare
	if (deltaTime > 0.3f) {
		return;
	}
	glm::ivec2 resolution = window->GetResolution();

	// Miscare pe Oy+ a arcului. Se impune o limita superioara
	if (window->KeyHold(GLFW_KEY_W)) {
		if (bow_ty < resolution.y/2 - 100 ) {
			bow_ty += 300 * deltaTime;
		}
	}
	
	// Miscarea pe OY- a arcului. Se impune o limita inferioara
	if (window->KeyHold(GLFW_KEY_S)) {
		if (bow_ty > -resolution.y / 2 + 100) {
			bow_ty -= 300 * deltaTime;
		}
	}

	// Se calculeaza puterea de tragere a sagetii,
	// in functie de cat este apasat click
	if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
		if (power < max_power && time_arrow > 1.3f) {
			power++;
		}
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	glm::ivec2 resolution = window->GetResolution();
	glm::ivec2 mouse = window->GetCursorPosition();
	
	// Se converteste la coordonata de ecran
	int mouse_x = mouse.x, mouse_y = resolution.y - mouse.y;
	
	//Se scade din mouse_y pentru a putea calcula unghiul fata de Ox, din origine
	radians_mouse = atan2(mouse_y - resolution.y/2 - bow_ty, mouse_x - bow_distance_x);

}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	//window->MouseHold;
	

}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{	
	// In momentul in care se elibereaza click-ul, se memoreaza 
	// pozitia arcului, unghiul pe care il face mouse-ul fata de
	// orizontala arcului si puterea
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && time_arrow > 1.3f) {
		glm::ivec2 resolution = window->GetResolution();

		time_arrow = 0;
		Arrows aux;
		aux.tx = bow_distance_x;
		aux.ty = resolution.y / 2 + bow_ty;
		aux.radians_arrow = radians_mouse;
		aux.power = power;
		aux.power_initial = power;
		arrows.push_back(aux);

		power = min_power;
	}
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}


// Functie de generare a unui numar aleator din intervalul [a,b)
int Tema1::Random(int a, int b) {
	return a + (rand() % (b - a + 1));
}

// Functia verifica daca exista o coliziune punct-elipsa
bool Tema1::Collision(float x, float y, float cx, float cy,
	float mx, float my, float angle) {
	float frac1 = (cos(angle)*(x - cx) + sin(angle)*(y - cy)) *
		(cos(angle)*(x - cx) + sin(angle)*(y - cy)) / (mx*mx);

	float frac2 = (sin(angle)*(x - cx) - cos(angle)*(y - cy)) *
		(sin(angle)*(x - cx) - cos(angle)*(y - cy)) / (my*my);
	if (frac1 + frac2 <= 1)
		return true;
	else
		return false;
}



