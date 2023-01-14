#pragma once

#include <directxmath.h>
#include <SimpleMath.h>
using namespace DirectX;


struct ModelOrganizerStruct
{
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
};

struct TextData
{
	char* text;
	XMFLOAT2 pos;
	XMFLOAT3 col = XMFLOAT3(1.0f,1.0f,1.0f);
};
