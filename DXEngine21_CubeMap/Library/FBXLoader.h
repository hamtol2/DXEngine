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
	// FBX �ε�.
	HRESULT LoadFBX(LPCSTR fileName, std::vector<Vertex>* outVertices, std::vector<DWORD>* outIndices);

	// ���� UV �о���� �޼ҵ�.
	XMFLOAT2 ReadUV(FbxMesh* fbxMesh, int controlPointIndex, int polygonIndex,  int positionInPolygon);
	XMFLOAT3 ReadNormal(FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);

	XMFLOAT3 ReadTangent(FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);

	XMFLOAT3 ReadBinormal(FbxMesh* fbxMesh, int controlPointIndex, int vertexCounter);
}