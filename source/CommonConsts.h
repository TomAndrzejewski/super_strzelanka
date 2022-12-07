#pragma once
#include <string>

#include "CommonTypes.h"

using std::string;


// SHADERS //
#define FONT_VS_SHADER_FILENAME			L"../Engine/font.vs"
#define FONT_PS_SHADER_FILENAME			L"../Engine/font.ps"

#define LIGHT_VS_SHADER_FILENAME		L"../Engine/light.vs"
#define LIGHT_PS_SHADER_FILENAME		L"../Engine/light.ps"


// TEXTURES AND MODELS //
#define SPHERE_MODEL_FILENAME			(char*)"../Engine/data/sphere.txt"
const WCHAR SPHERE_TEXTURE_FILENAME[] =	L"../Engine/data/seafloor.dds";

#define CUBE_MODEL_FILENAME				(char*)"../Engine/data/cube.txt"
const WCHAR CUBE_TEXTURE_FILENAME[] =	L"../Engine/data/seafloor.dds";

#define WALL_MODEL_FILENAME				(char*)"../Engine/data/indoor_wall_2D.txt"
const WCHAR WALL_TEXTURE_FILENAME[] =	L"../Engine/data/seafloor.dds";
#define WALL_BBOX_FILENAME				(char*)"../Engine/data/indoor_wall_2D_bbox.txt"

#define FONT_MODEL_FILENAME				(char*)"../Engine/data/fontdata.txt"
const WCHAR FONT_TEXTURE_FILENAME[] =	L"../Engine/data/font.dds";


// SIMPLE CUBE ENEMY MODEL //
#define SIMPLE_CUBE_ENEMY_NUMBER		10
#define SIMPLE_CUBE_BBOX_NUMBER			1
const string simpleCubeBBoxFiles[SIMPLE_CUBE_BBOX_NUMBER] = { "../Engine/data/cube_bbox.txt" };

// ALL ENEMY MODELS //
const float playerYLevel = -5.0f;
const float enemyHeight = 8.0f;

const ModelOrganizerStruct enemies[] = { { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, enemyHeight, 1.0f) }
										};

const int sizeOf_enemies = sizeof(enemies) / sizeof(ModelOrganizerStruct);


// ALL WALLS //
const float mainSquareLen = 400.0f;
const float mainSquareWidth = 400.0f;
const float wallsHeight = 15.0f;
const XMFLOAT3 rot0 = XMFLOAT3(0.0f, -90.0f, 0.0f); // refer to rotation of wall number 0
const XMFLOAT3 rot1 = XMFLOAT3(0.0f, 90.0f, 0.0f); // refer to rotation of wall number 1
const XMFLOAT3 rot2 = XMFLOAT3(180.0f, 180.0f, 0.0f); // refer to rotation of wall number 2
const XMFLOAT3 rot3 = XMFLOAT3(0.0f, 180.0f, 0.0f); // refer to rotation of wall number 3

const ModelOrganizerStruct walls[] = { { XMFLOAT3(-100.0f, playerYLevel, 350.0f), rot0, XMFLOAT3(mainSquareLen, wallsHeight, 1.0f) }, //0
									  { XMFLOAT3(2*mainSquareWidth + (-100.0f), playerYLevel, 350.0f), rot1, XMFLOAT3(mainSquareLen, wallsHeight, 1.0f) }, //1
									  { XMFLOAT3(mainSquareWidth + (-100.0f), playerYLevel, 350.0f + mainSquareLen), rot2, XMFLOAT3(mainSquareLen, wallsHeight, 1.0f) }, //2
									  { XMFLOAT3(mainSquareWidth + (-100.0f), playerYLevel, 350.0f - mainSquareLen), rot3, XMFLOAT3(mainSquareLen, wallsHeight, 1.0f) }, //3
									  { XMFLOAT3(-100.0f + mainSquareLen / 4 - 1.0f, playerYLevel, 350.0f - mainSquareLen / 3), rot2, XMFLOAT3(mainSquareLen / 6, wallsHeight, 1.0f)}, //4
									  { XMFLOAT3(-100.0f + mainSquareLen / 6, playerYLevel, 350.0f + mainSquareLen / 3), rot2, XMFLOAT3(mainSquareLen / 6, wallsHeight, 1.0f)}, //5
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + mainSquareLen / 3), rot2, XMFLOAT3(mainSquareLen / 2, wallsHeight, 1.0f)}, //6
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 7.0f) * mainSquareLen - 1.0f), rot1, XMFLOAT3(mainSquareLen / 7, wallsHeight, 1.0f)}, //7
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 - 1 / 7.0f) * mainSquareLen), rot1, XMFLOAT3(mainSquareLen / 7, wallsHeight, 1.0f)}, //8
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f + 1 / 12.0f - 1 / 40.0f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 7.0f - 1 / 2.64f) * mainSquareLen - 1.0f), XMFLOAT3(0.0f, 75.0f, 0.0f), XMFLOAT3(mainSquareLen / 4, wallsHeight, 1.0f)}, //9
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f + 1 / 8.2f + 1 / 3.5f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 7.0f - 1 / 2.64f - 1 / 2.3f + 1 / 5.0f) * mainSquareLen - 1.0f), rot2, XMFLOAT3(mainSquareLen / 5.5f, wallsHeight, 1.0f)}, //10
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f + 1 / 8.2f + 1 / 11.3f + 1 / 56.0f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 7.0f - 1 / 2.64f - 1 / 2.3f) * mainSquareLen - 1.0f), rot1, XMFLOAT3(mainSquareLen / 5, wallsHeight, 1.0f)}, //11
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f + 1 / 8.2f + 1 / 3.5f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 7.0f - 1 / 2.64f - 1 / 2.3f - 1 / 16.0f) * mainSquareLen - 1.0f), rot2, XMFLOAT3(mainSquareLen / 5.5f, wallsHeight, 1.0f)}, //13
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f + 1 / 2.0f + 1 / 8.2f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 7.0f - 1 / 2.64f - 1 / 2.3f) * mainSquareLen - 1.0f), rot1, XMFLOAT3(mainSquareLen / 5, wallsHeight, 1.0f)}, //14
									  { XMFLOAT3(-100.0f + (1 + 1 / 3.55f) * mainSquareLen, playerYLevel, 350.0f - (1 / 5.0f) * mainSquareLen), rot2, XMFLOAT3(mainSquareLen / 4, wallsHeight, 1.0f)}, //15
									  { XMFLOAT3(-100.0f + (1 + 1 / 4.0f - 2 / 6.0f) * mainSquareLen, playerYLevel, 350.0f - (2 / 6.0f) * mainSquareLen), XMFLOAT3(0.0f, -45.0f, 0.0f), XMFLOAT3(mainSquareLen / 5, wallsHeight, 1.0f)}, //16
									  { XMFLOAT3(-100.0f + (1 - 1 / 4.6f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f + 1 / 3.9f - 1 / 2.64f - 1 / 2.3f) * mainSquareLen - 1.0f), rot1, XMFLOAT3(mainSquareLen / 4, wallsHeight, 1.0f)}, //17
									  { XMFLOAT3(-100.0f + (1 / 4.0f + 1 / 6.0f + 1 / 2.0f - 1 / 3.34f) * mainSquareLen - 1.0f, playerYLevel, 350.0f + (1 / 3.0f - 1 / 3.0f) * mainSquareLen), rot2, XMFLOAT3(mainSquareLen / 5, wallsHeight, 1.0f)}, //18
									  { XMFLOAT3(-100.0f + mainSquareLen / 4 + mainSquareLen / 6, playerYLevel, 350.0f - mainSquareLen/3), rot1, XMFLOAT3(2*mainSquareLen / 3, wallsHeight, 1.0f)}, }; //19

const int sizeOf_walls = sizeof(walls) / sizeof(ModelOrganizerStruct);

// GAME CONSTANTS
const XMFLOAT3 startPos = XMFLOAT3(0.0f, 0.0f, -10.0f);
const XMFLOAT3 startRot = XMFLOAT3(0.0f, 0.0f, 0.0f);