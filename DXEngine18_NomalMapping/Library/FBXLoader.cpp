#include "FBXLoader.h"

namespace FBXLoader
{
	FbxManager* fbxManager;

	HRESULT LoadFBX(LPCSTR fileName, std::vector<Vertex>* outVertices,
		std::vector<DWORD>* outIndices)
	{
		// fbxManager�� ������ �ȵ����� �����ϱ�.
		if (fbxManager == NULL)
		{
			// SDK �Ŵ��� ����.
			fbxManager = FbxManager::Create();

			FbxIOSettings* ioSettings 
				= FbxIOSettings::Create(fbxManager, IOSROOT);
			fbxManager->SetIOSettings(ioSettings);
		}

		// GeometryConverter ����.
		FbxGeometryConverter geometryConverter(fbxManager);
		FbxImporter* importer
			= FbxImporter::Create(fbxManager, "");
		FbxScene* fbxScene
			= FbxScene::Create(fbxManager, "");

		// ������ �ʱ�ȭ.
		if (importer->Initialize(fileName, -1, fbxManager->GetIOSettings()) == false)
		{
			return E_FAIL;
		}

		// 3D ��(���) ����Ʈ.
		if (importer->Import(fbxScene) == false)
		{
			return E_FAIL;
		}

		// FBX Node �б�.
		FbxNode* fbxRootNode = fbxScene->GetRootNode();
		if (fbxRootNode == NULL)
			return E_FAIL;

		// �� ������ �⺻ ������ �ﰢ������ ����.
		geometryConverter.Triangulate(fbxScene, true);

		// ��Ʈ ����� ���� ��� ���� ������.
		int childCount = fbxRootNode->GetChildCount();
		for (int ix = 0; ix < childCount; ++ix)
		{
			// �ڽ� ���� ��� �о����.
			FbxNode* childNode = fbxRootNode->GetChild(ix);

			// �Ӽ��� ���� ���� �н�.
			if (childNode->GetNodeAttribute() == NULL)
				continue;

			// �Ӽ� �߿��� Ÿ�Ը� Ȯ��.
			FbxNodeAttribute::EType attrType
				= childNode->GetNodeAttribute()->GetAttributeType();

			// �޽� Ÿ������ Ȯ��.
			if (attrType != FbxNodeAttribute::EType::eMesh)
				continue;

			// �޽� ���� �о����.
			FbxMesh* fbxMesh = childNode->GetMesh();

			// �� �迭 ������(����).
			FbxVector4* vertices = fbxMesh->GetControlPoints();

			// ������ �� Ȯ��.
			int polygonCount = fbxMesh->GetPolygonCount();
			// �ε��� ������ ����.
			int vertexCounter = 0;

			// ������ ��ȸ.
			for (int jx = 0; jx < polygonCount; ++jx)
			{
				// �������� �����ϴ� ���� ���� Ȯ��.
				int vertexCount = fbxMesh->GetPolygonSize(jx);

				// ������ ���� ���� ��ȸ.
				for (int kx = 0; kx < vertexCount; ++kx)
				{
					// ���� ���� Ȯ��.
					int vertexIndex = fbxMesh->GetPolygonVertex(jx, kx);

					// ���� ���� �о����.
					Vertex vertex;
					vertex.position.x = static_cast<float>(vertices[vertexIndex].mData[0]);
					vertex.position.y = static_cast<float>(vertices[vertexIndex].mData[1]);
					vertex.position.z = static_cast<float>(vertices[vertexIndex].mData[2]);

					// UV �о����.
					// jx -> ������ ���� / kx -> ������ �ȿ����� ���� ����.
					vertex.textureCoord = ReadUV(fbxMesh, vertexIndex, jx, kx);

					// ��� �о����.
					vertex.normal = ReadNormal(fbxMesh, vertexIndex, vertexCounter);

					// ź��Ʈ �о����.
					vertex.tangent = ReadTangent(fbxMesh, vertexIndex, vertexCounter);

					// ���̳�� �о����.
					vertex.binormal = ReadBinormal(fbxMesh, vertexIndex, vertexCounter);

					// ���� �߰�.
					outVertices->push_back(vertex);

					// �ε��� �߰�.
					outIndices->push_back(vertexCounter);
					++vertexCounter;
				}
			}
		}

		return S_OK;
	}
	
	XMFLOAT2 ReadUV(FbxMesh * fbxMesh, int controlPointIndex, int polygonIndex, int positionInPolygon)
	{
		// UV�� �ִ��� Ȯ��.
		if (fbxMesh->GetElementUVCount() < 1)
		{
			MessageBox(NULL, TEXT("UV�� �����ϴ�"), TEXT("����"), MB_OK);
			return XMFLOAT2();
		}

		// ��ȯ�� ���� ����.
		XMFLOAT2 texCoord(0.0f, 0.0f);

		// UV ��ü �迭 �о����.
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
		// ����� �ִ��� Ȯ��.
		if (fbxMesh->GetElementNormalCount() < 1)
		{
			MessageBox(NULL, TEXT("����� �����ϴ�"), TEXT("����"), MB_OK);
			return XMFLOAT3();
		}

		// ��ȯ�� ���� ����.
		XMFLOAT3 normal(0.0f, 0.0f, 0.0f);

		// UV ��ü �迭 �о����.
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
		// ź��Ʈ�� �ִ��� Ȯ��.
		if (fbxMesh->GetElementTangentCount() < 1)
		{
			MessageBox(NULL, TEXT("ź��Ʈ�� �����ϴ�"), TEXT("����"), MB_OK);
			return XMFLOAT3();
		}

		// ��ȯ�� ���� ����.
		XMFLOAT3 tangent(0.0f, 0.0f, 0.0f);

		// UV ��ü �迭 �о����.
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
		// ���̳���� �ִ��� Ȯ��.
		if (fbxMesh->GetElementBinormalCount() < 1)
		{
			MessageBox(NULL, TEXT("���� ����� �����ϴ�"), TEXT("����"), MB_OK);
			return XMFLOAT3();
		}

		// ��ȯ�� ���� ����.
		XMFLOAT3 binormal(0.0f, 0.0f, 0.0f);

		// UV ��ü �迭 �о����.
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