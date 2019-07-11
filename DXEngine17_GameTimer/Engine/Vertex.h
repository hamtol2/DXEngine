#pragma once

#include "DXUtil.h"

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 textureCoord;
	XMFLOAT3 normal;

	Vertex() { }
	Vertex(float x, float y, float z) :
		position(x, y, z) { }
	Vertex(XMFLOAT3 position) : position(position) { }
	Vertex(XMFLOAT3 position, XMFLOAT2 uv)
		: position(position), textureCoord(uv) { }
};