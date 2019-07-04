#pragma once

#include "../Engine/DXUtil.h"
#include "../Engine/Mesh.h"
#include "../Engine/Vertex.h"
#include <vector>
#include <fbxsdk.h>

#pragma comment(lib, "libfbxsdk.lib")

namespace FBXLoader
{
	HRESULT LoadFBX(
		LPCSTR fileName,
		std::vector<Vertex> outVertices,
		std::vector<DWORD> outIndices);
}