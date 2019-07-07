#pragma once

#include "../Engine/DXUtil.h"
#include "../Engine/Mesh.h"
#include "../Engine/Vertex.h"
#include "../Engine/Engine.h"
#include <vector>
#include <fbxsdk.h>

#pragma comment(lib, "libfbxsdk.lib")

namespace FBXLoader
{
	// FBX 로드.
	HRESULT LoadFBX(LPCSTR fileName, std::vector<Vertex>* outVertices, std::vector<DWORD>* outIndices);

	// 정점 UV 읽어오는 메소드.
	XMFLOAT2 ReadUV(FbxMesh* fbxMesh, int controlPointIndex, int polygonIndex,  int positionInPolygon);
	XMFLOAT3 ReadNormal(FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);
}