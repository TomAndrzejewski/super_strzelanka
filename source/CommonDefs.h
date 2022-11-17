#pragma once
#include <string>

using std::string;


// SHADERS //
#define FONT_VS_SHADER_FILENAME			L"../Engine/font.vs"
#define FONT_PS_SHADER_FILENAME			L"../Engine/font.ps"

#define LIGHT_VS_SHADER_FILENAME		L"../Engine/light.vs"
#define LIGHT_PS_SHADER_FILENAME		L"../Engine/light.ps"


// TEXTURES AND MODELS //
#define SPHERE_MODEL_FILENAME			(char*)"../Engine/data/sphere.txt"
#define SPHERE_TEXTURE_FILENAME			L"../Engine/data/seafloor.dds"

#define CUBE_MODEL_FILENAME				(char*)"../Engine/data/cube.txt"
#define CUBE_TEXTURE_FILENAME			L"../Engine/data/seafloor.dds"

#define FONT_MODEL_FILENAME				(char*)"../Engine/data/fontdata.txt"
#define FONT_TEXTURE_FILENAME			L"../Engine/data/font.dds"


// SIMPLE CUBE ENEMY MODEL //
#define SIMPLE_CUBE_ENEMY_NUMBER		10
#define SIMPLE_CUBE_BBOX_NUMBER			1
const string simpleCubeBBoxFiles[SIMPLE_CUBE_BBOX_NUMBER] = { "../Engine/data/cube_bbox.txt" };

// ALL MODELS //
#define ENEMY_NUMBER	SIMPLE_CUBE_ENEMY_NUMBER // +...