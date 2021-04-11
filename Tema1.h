#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

typedef struct Arrows{
	float tx;
	float ty;
	float radians_arrow;
	float power;
	float power_initial;
}Arrows;

typedef struct Balloons {
	float tx;
	float ty;
	float sx;
	float sy;
	float speed;
	float trace;
	float rad_trace;
	int color;	
}Balloons;

typedef struct Shurikens {
	float tx;
	float ty;
	float speed;
	float radians;
	float speed_rotation;
}Shurikens;


class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:

	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrix;
	float bow_ty; // Factor de translatare pentru arc
	float radians_mouse; //Factor de rotire pentru arc
	float power, min_power = 10, max_power = 40; // Puterea cu care se trage sageata
	std::vector<Arrows> arrows; // Sagetile din ecran
	std::vector<Balloons>balloons; // Baloanele din ecran
	std::vector<Shurikens>shurikens; // Shuriken-urile din ecran
	std::vector<Balloons>bang_balloons; // Baloanele sparte
	std::vector<Shurikens>bang_shu; // Shuriken doborat
	float bow_distance_x = 100; // Distante de la OY la arc
	float res_balloons; // Timpul de respawnare a baloanelor
	float time_arrow; // Timpul de tragere
	float res_shurikens; // Timpul de respawnare a shuriken
	float shuriken_length = 100; // Lungime shuriken
	float scalex_bow = 0.6f, scaley_bow = 1; // Scalare arc
	int score; // Scor jucator
	int hp; // Viata jucator

	void GenerateShurikens(); // Functia de spawnare a shuriken-urilor
	void GenerateBalloons(); // Functie de spawnare a baloanelor
	int Random(int a, int b); //Functie de calculare a unui numar intre [a,b)
	bool Collision(float x, float y, float cx, float cy, float mx, float my,
		float angle); // Verifica coliziune punct - elipsa
	bool CheckBowCollision(Shurikens s); // Coliziune shuriken - arc
	void CollisionArrow(float deltaTimeSeconds); // Functie coliziune balon sageata shuriekn
	void BangBalloon(float deltaTimeSeconds); // Animatie spargere balon
	void SpawnArrow(float deltaTimeSeconds); // Afisare sageti
	void SpawnBalloons(float deltaTimeSeconds); // Afisare baloane
	void SpawnShurikens(float deltaTimeSeconds); // Afisare shuriken;
	void BangShuriken(float deltaTimeSeconds); // Animatie shuriken doborat
	void SpawnBow(); // Afisare arc
	void CollisionBow(); // Verificare coliziune 
	void SpawnHp(); // Afisare Hp
	void SpawnPowerBar(); // Afiseaza power bar
};
