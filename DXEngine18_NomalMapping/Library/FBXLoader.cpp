#include "FBXLoader.h"

namespace FBXLoader
{
	FbxManager* fbxManager;

	HRESULT LoadFBX(LPCSTR fileName, std::vector<Vertex>* outVertices,
		std::vector<DWORD>* outIndices)
	{
		// fbxManager가 생성이 안됐으면 생성하기.
		if (fbxManager == NULL)
		{
			// SDK 매니저 생성.
			fbxManager = FbxManager::Create();

			FbxIOSettings* ioSettings 
				= FbxIOSettings::Create(fbxManager, IOSROOT);
			fbxManager->SetIOSettings(ioSettings);
		}

		// GeometryConverter 생성.
		FbxGeometryConverter geometryConverter(fbxManager);
		FbxImporter* importer
			= FbxImporter::Create(fbxManager, "");
		FbxScene* fbxScene
			= FbxScene::Create(fbxManager, "");

		// 임포터 초기화.
		if (importer->Initialize(fileName, -1, fbxManager->GetIOSettings()) == false)
		{
			return E_FAIL;
		}

		// 3D 씬(장면) 임포트.
		if (importer->Import(fbxScene) == false)
		{
			return E_FAIL;
		}

		// FBX Node 읽기.
		FbxNode* fbxRootNode = fbxScene->GetRootNode();
		if (fbxRootNode == NULL)
			return E_FAIL;

		// 모델 폴리곤 기본 구조를 삼각면으로 설정.
		geometryConverter.Triangulate(fbxScene, true);

		// 루트 노드의 하위 노드 개수 얻어오기.
		int childCount = fbxRootNode->GetChildCount();
		for (int ix = 0; ix < childCount; ++ix)
		{
			// 자식 계층 노드 읽어오기.
			FbxNode* childNode = fbxRootNode->GetChild(ix);

			// 속성이 없는 노드는 패스.
			if (childNode->GetNodeAttribute() == NULL)
				continue;

			// 속성 중에서 타입만 확인.
			FbxNodeAttribute::EType attrType
				= childNode->GetNodeAttribute()->GetAttributeType();

			// 메시 타입인지 확인.
			if (attrType != FbxNodeAttribute::EType::eMesh)
				continue;

			// 메시 정보 읽어오기.
			FbxMesh* fbxMesh = childNode->GetMesh();

			// 점 배열 얻어오기(정점).
			FbxVector4* vertices = fbxMesh->GetControlPoints();

			// 폴리곤 수 확인.
			int polygonCount = fbxMesh->GetPolygonCount();
			// 인덱스 설정용 변수.
			int vertexCounter = 0;

			// 폴리곤 순회.
			for (int jx = 0; jx < polygonCount; ++jx)
			{
				// 폴리곤을 구성하는 정점 개수 확인.
				int vertexCount = fbxMesh->GetPolygonSize(jx);

				// 폴리곤 안의 정점 순회.
				for (int kx = 0; kx < vertexCount; ++kx)
				{
					// 정점 순번 확인.
					int vertexIndex = fbxMesh->GetPolygonVertex(jx, kx);

					// 정점 정보 읽어오기.
					Vertex vertex;
					vertex.position.x = static_cast<float>(vertices[vertexIndex].mData[0]);
					vertex.position.y = static_cast<float>(vertices[vertexIndex].mData[1]);
					vertex.position.z = static_cast<float>(vertices[vertexIndex].mData[2]);

					// UV 읽어오기.
					// jx -> 폴리곤 순번 / kx -> 폴리곤 안에서의 정점 순번.
					vertex.textureCoord = ReadUV(fbxMesh, vertexIndex, jx, kx);

					// 노멀 읽어오기.
					vertex.normal = ReadNormal(fbxMesh, vertexIndex, vertexCounter);

					// 탄젠트 읽어오기.
					vertex.tangent = ReadTangent(fbxMesh, vertexIndex, vertexCounter);

					// 바이노멀 읽어오기.
					vertex.binormal = ReadBinormal(fbxMesh, vertexIndex, vertexCounter);

					// 정점 추가.
					outVertices->push_back(vertex);

					// 인덱스 추가.
					outIndices->push_back(vertexCounter);
					++vertexCounter;
				}
			}
		}

		return S_OK;
	}
	
	XMFLOAT2 ReadUV(FbxMesh * fbxMesh, int controlPointIndex, int polygonIndex, int positionInPolygon)
	{
		// UV가 있는지 확인.
		if (fbxMesh->GetElementUVCount() < 1)
		{
			MessageBox(NULL, TEXT("UV가 없습니다"), TEXT("오류"), MB_OK);
			return XMFLOAT2();
		}

		// 반환용 변수 선언.
		XMFLOAT2 texCoord(0.0f, 0.0f);

		// UV 전체 배열 읽어오기.
		FbxGeometryElementUV* vertexUV = fbxMesh->GetLayer(0)->GetUVs();
		FbxLayerElement::EReferenceMode referenceMode = vertexUV->GetReferenceMode();
		FbxLayerElement::EMappingMode mappingMode = vertexUV->GetMappingMode();

		int index = 0;
		if (mappingMode == FbxLayerElement::eByControlPoint)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = controlPointIndex;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexUV->GetIndexArray().GetAt(controlPointIndex);
		}
		else if (mappingMode == FbxLayerElement::eByPolygonVertex)
		{
			if (referenceMode == FbxLayerElement::eDirect || referenceMode == FbxLayerElement::eIndexToDirect)
			{
				index = fbxMesh->GetTextureUVIndex(polygonIndex, positionInPolygon);
			}
		}

		texCoord.x = static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[0]);
		texCoord.y = 1.0f - static_cast<float>(vertexUV->GetDirectArray().GetAt(index).mData[1]);

		return texCoord;
	}

	XMFLOAT3 ReadNormal(FbxMesh * fbxMesh, int controlPointIndex, int vertexCounter)
	{
		// 노멀이 있는지 확인.
		if (fbxMesh->GetElementNormalCount() < 1)
		{
			MessageBox(NULL, TEXT("노멀이 없습니다"), TEXT("오류"), MB_OK);
			return XMFLOAT3();
		}

		// 반환용 변수 선언.
		XMFLOAT3 normal(0.0f, 0.0f, 0.0f);

		// UV 전체 배열 읽어오기.
		FbxGeometryElementNormal* vertexNormal = fbxMesh->GetLayer(0)->GetNormals();
		FbxLayerElement::EReferenceMode referenceMode = vertexNormal->GetReferenceMode();
		FbxLayerElement::EMappingMode mappingMode = vertexNormal->GetMappingMode();

		int index = 0;
		if (mappingMode == FbxLayerElement::eByControlPoint)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = controlPointIndex;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
		}
		else if (mappingMode == FbxLayerElement::eByPolygonVertex)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = vertexCounter;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
		}

		normal.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
		normal.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
		normal.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);

		return normal;
	}

	XMFLOAT3 ReadTangent(FbxMesh * fbxMesh, int controlPointIndex, int vertexCounter)
	{
		// 탄젠트가 있는지 확인.
		if (fbxMesh->GetElementTangentCount() < 1)
		{
			MessageBox(NULL, TEXT("탄젠트가 없습니다"), TEXT("오류"), MB_OK);
			return XMFLOAT3();
		}

		// 반환용 변수 선언.
		XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);

		// UV 전체 배열 읽어오기.
		FbxGeometryElementTangent* vertexTangent 
			= fbxMesh->GetLayer(0)->GetTangents();
		FbxLayerElement::EReferenceMode referenceMode 
			= vertexTangent->GetReferenceMode();
		FbxLayerElement::EMappingMode mappingMode 
			= vertexTangent->GetMappingMode();

		int index = 0;
		if (mappingMode == FbxLayerElement::eByControlPoint)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = controlPointIndex;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexTangent->GetIndexArray().GetAt(controlPointIndex);
		}
		else if (mappingMode == FbxLayerElement::eByPolygonVertex)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = vertexCounter;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
		}

		tangent.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
		tangent.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
		tangent.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);

		return tangent;
	}

	XMFLOAT3 ReadBinormal(FbxMesh * fbxMesh, int controlPointIndex, int vertexCounter)
	{
		// 바이노멀이 있는지 확인.
		if (fbxMesh->GetElementBinormalCount() < 1)
		{
			MessageBox(NULL, TEXT("바이 노멀이 없습니다"), TEXT("오류"), MB_OK);
			return XMFLOAT3();
		}

		// 반환용 변수 선언.
		XMFLOAT3 binormal(0.0f, 0.0f, 0.0f);

		// UV 전체 배열 읽어오기.
		FbxGeometryElementBinormal* vertexBinormal
			= fbxMesh->GetLayer(0)->GetBinormals();
		FbxLayerElement::EReferenceMode referenceMode
			= vertexBinormal->GetReferenceMode();
		FbxLayerElement::EMappingMode mappingMode
			= vertexBinormal->GetMappingMode();

		int index = 0;
		if (mappingMode == FbxLayerElement::eByControlPoint)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = controlPointIndex;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexBinormal->GetIndexArray().GetAt(controlPointIndex);
		}
		else if (mappingMode == FbxLayerElement::eByPolygonVertex)
		{
			if (referenceMode == FbxLayerElement::eDirect)
				index = vertexCounter;
			else if (referenceMode == FbxLayerElement::eIndexToDirect)
				index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
		}

		binormal.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
		binormal.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
		binormal.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);

		return binormal;
	}
}