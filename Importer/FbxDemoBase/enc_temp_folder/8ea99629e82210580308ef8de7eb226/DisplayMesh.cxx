/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayMesh.h"

#include "DisplayMaterial.h"
#include "DisplayTexture.h"
#include "DisplayLink.h"
#include "DisplayShape.h"
#include "DisplayCache.h"
#include <Windows.h>

//#include "FileWrite.h"
//#include "FileWrite.h"

#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

#define MAT_HEADER_LENGTH 200

void DisplayControlsPoints(FbxMesh* pMesh);
void DisplayPolygons(FbxMesh* pMesh);
void DisplayMaterialMapping(FbxMesh* pMesh);
void DisplayTextureMapping(FbxMesh* pMesh);
void DisplayTextureNames(FbxProperty& pProperty, FbxString& pConnectionString);
void DisplayMaterialConnections(FbxMesh* pMesh);
void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial,
	char* header, int pMatId, int l);


FileWrite myFile("../biFile.bff");
FileWrite myStringFile("../stringFile.bff");
int vertexCount;
std::vector<ControlPointBFF> controlPointData;

MeshBFF meshData;
int currentVertex = 0;

MaterialBFF materialData;

//FbxMesh* MeshRef;
std::vector<std::vector<BlendShapesBFF>> blendShapeDataArr2;
int nrOfShapes2;
int nrOfVertexInShape2;

void DisplayMesh(FbxNode* pNode)
{


	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();
	//MeshRef = (FbxMesh*)pNode->GetNodeAttribute();

	DisplayString("Mesh Name: ", (char*)pNode->GetName());

	char temp[64];
	strcpy_s(temp, _countof(temp), pNode->GetName());
	for (int i = 0; i < sizeof(temp); i++)
		meshData.name[i] = temp[i];




	//myFile.writeToFile(pNode->GetName(), strlen(pNode->GetName()) + 1); //strcpy_s()
	//c string func
	DisplayMetaDataConnections(lMesh);
	DisplayControlsPoints(lMesh);
	DisplayPolygons(lMesh);

	DisplayMaterialMapping(lMesh);
	DisplayMaterial(lMesh);
	materialData = getMaterialInfo();

	DisplayTexture(lMesh);

	std::string fullPath = GetTexturePath();
	if (fullPath != "")
	{
		int index1 = fullPath.find_last_of("\\");
		int index2 = fullPath.find_last_of("/");

		std::string start = fullPath.substr(0, index1);
		std::string end = fullPath.substr(index2, fullPath.size() - 1);

		std::string destination = start + end;
		CopyFile(GetTexturePath().c_str(), destination.c_str(), FALSE);

		char temp2[128];
		strcpy_s(temp2, _countof(temp2), destination.c_str()); 
		for (int i = 0; i < sizeof(temp2); i++)
			materialData.texturePath[i] = temp2[i];
	}
	else
	{
		std::string noTextureString = "No texture found!";
		char temp2[128];
		strcpy_s(temp2, _countof(temp2), noTextureString.c_str());
		for (int i = 0; i < sizeof(temp2); i++)
			materialData.texturePath[i] = temp2[i];
	}

	DisplayMaterialConnections(lMesh);
	DisplayLink(lMesh);
	DisplayShape(lMesh);
	DisplayCache(lMesh);
}


MeshBFF GetMeshData()
{
	return meshData;
}

std::vector<ControlPointBFF> GetControlPointData()
{
	return controlPointData;
}

MaterialBFF GetMaterialData2()
{
	return materialData;
}

std::vector<std::vector<BlendShapesBFF>> GetBlendShapeDataArr2(FbxNode* pNode)
{
	blendShapeDataArr2 = GetBlendShapeDataArr();

	FbxMesh* MeshRef = (FbxMesh*)pNode->GetNodeAttribute();
	FbxShape* shape;
	FbxBlendShape* blendShape;
	FbxBlendShapeChannel* blendShapeChannels;
	int blendShapeCount = MeshRef->GetDeformerCount(FbxDeformer::eBlendShape);
	int nrOfFaces = MeshRef->GetPolygonCount();
	int nrOfBlendShapeChannels;
	int lTargetShapeCount;
	int nrOfControlPoints;

	
	for (int i = 0; i < blendShapeCount; ++i)														//Alla belnds shapes					2
	{
		//myStringFile.writeToStringFile("\n\n----------\n\n");
		//myStringFile.writeToStringFile("\nShapeNr = " + std::to_string(i));
		blendShape = (FbxBlendShape*)MeshRef->GetDeformer(i, FbxDeformer::eBlendShape);
		nrOfBlendShapeChannels = blendShape->GetBlendShapeChannelCount();
		for (int j = 0; j < nrOfBlendShapeChannels; ++j)											//Alla kanaler i blend shapen			1
		{
			blendShapeChannels = blendShape->GetBlendShapeChannel(j);
			lTargetShapeCount = blendShapeChannels->GetTargetShapeCount();
			for (int k = 0; k < lTargetShapeCount; ++k)												//Alla targets för blend shapen			1
			{
				shape = blendShapeChannels->GetTargetShape(k);
				nrOfControlPoints = shape->GetControlPointsCount();
				for (int l = 0; l < nrOfControlPoints; l++)											//Alla Control points i blend shapen	3
				{
					//myStringFile.writeToStringFile("\nControlPointIndex = " + std::to_string(l));
					FbxLayerElementArrayTemplate<FbxVector4>* normals = NULL;
					bool status = shape->GetNormals(&normals);
					FbxVector4* controlPoints = shape->GetControlPoints();
					//for (int m = 0; m < GetNrOfBlendShapes(); m++)								//
					//{
						for (int n = 0; n < nrOfFaces; n++)											//Alla faces i meshen					1
						{
							//myStringFile.writeToStringFile("\nFace = " + std::to_string(n));
							int nrOfVertexInFace = MeshRef->GetPolygonSize(n);
							for (int o = 0; o < nrOfVertexInFace; o++)								//Alla vertex punkter i facen			3
							{
								//myStringFile.writeToStringFile("\nNrOfVertexInFace = " + std::to_string(o));
								int currentControllPoint = MeshRef->GetPolygonVertex(n, o);

								blendShapeDataArr2[i][o].pos[0] = controlPoints[currentControllPoint][0];
								blendShapeDataArr2[i][o].pos[1] = controlPoints[currentControllPoint][1];
								blendShapeDataArr2[i][o].pos[2] = controlPoints[currentControllPoint][2];
								
								//Innan var i = m
								if (status && normals && normals->GetCount() == nrOfControlPoints)
								{
									blendShapeDataArr2[i][o].norm[0] = normals->GetAt(currentControllPoint)[0];
									blendShapeDataArr2[i][o].norm[1] = normals->GetAt(currentControllPoint)[1];
									blendShapeDataArr2[i][o].norm[2] = normals->GetAt(currentControllPoint)[2];
								}
							}
						}
					//}
				}
			}
		}
	}
	return blendShapeDataArr2;
	//return GetBlendShapeDataArr();
}

std::vector<std::vector<BlendshapeBFF>> GetNewBlendShapeData2()
{
	return GetNewBlendShapeData();
}

int GetNrOfVertexInBlendShape2()
{
	return GetNrOfVertexInBlendShape();
}

int GetNrOfBlendShapes2()
{
	return GetNrOfBlendShapes();
}








void DisplayControlsPoints(FbxMesh* pMesh)
{

	FbxVector4* lControlPoints = pMesh->GetControlPoints();

	DisplayString("    Control Points");

	for (int i = 0; i < pMesh->GetControlPointsCount(); i++)
	{
		DisplayInt("        Control Point ", i);
		Display3DVector("            Coordinates: ", lControlPoints[i]);


		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);

			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];

				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					auto array = leNormals->GetDirectArray().GetAt(i);
					Display3DVector(header, array);

				}



			}

		}
	}

	// Eget:
	int nrOfControlPoints = pMesh->GetControlPointsCount();
	int nrOfUVs = pMesh->GetElementUVCount();
	int nrOfNormals = pMesh->GetElementNormalCount();
	int nrOfInfluences = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetClusterCount();
	
	FbxCluster* cluster;
	meshData.nrOfControlPoints = nrOfControlPoints;
	controlPointData.resize(nrOfControlPoints);

	for (int c = 0; c < nrOfControlPoints; c++)
	{
		//Add positions
		controlPointData[c].pos[0] = lControlPoints[c][0];
		controlPointData[c].pos[1] = lControlPoints[c][1];
		controlPointData[c].pos[2] = lControlPoints[c][2];

		for (int uv = 0; uv < nrOfUVs; ++uv)
		{
			FbxGeometryElementUV* UVs = pMesh->GetElementUV(uv);
			auto UVArray = UVs->GetDirectArray().GetAt(c);

			//Add uvs
			controlPointData[c].uv[0] = UVArray[0];
			controlPointData[c].uv[1] = UVArray[1];
		}

		for (int n = 0; n < nrOfNormals; n++)
		{
			FbxGeometryElementNormal* normals = pMesh->GetElementNormal(n);
			FbxGeometryElementTangent* tangents = pMesh->GetElementTangent(n);
			FbxGeometryElementBinormal* biNormals = pMesh->GetElementBinormal(n);
			auto normalArray = normals->GetDirectArray().GetAt(c);
			auto tangentArray = tangents->GetDirectArray().GetAt(c);
			auto biNormalArray = biNormals->GetDirectArray().GetAt(c);

			//Add normals
			controlPointData[c].norm[0] = normalArray[0];
			controlPointData[c].norm[1] = normalArray[1];
			controlPointData[c].norm[2] = normalArray[2];

			//Add tangets
			controlPointData[c].tan[0] = tangentArray[0];
			controlPointData[c].tan[1] = tangentArray[1];
			controlPointData[c].tan[2] = tangentArray[2];

			//Add biNormals
			controlPointData[c].biNorm[0] = biNormalArray[0];
			controlPointData[c].biNorm[1] = biNormalArray[1];
			controlPointData[c].biNorm[2] = biNormalArray[2];
		}

		for (int i = 0; i < nrOfInfluences; ++i)
		{
			//controlPointData[0].boneID[i] = nrOfInfluences;
			cluster = ((FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(i);
			int* jointIndex = cluster->GetControlPointIndices();
			int lIndexCount = cluster->GetControlPointIndicesCount();

			for (int k = 0; k < lIndexCount; k++)
			{
				if (i <= 4)
				{
					controlPointData[c].boneID[i] = jointIndex[i];
				}
			}
		}
	}




	DisplayString("");
}



void DisplayPolygons(FbxMesh* pMesh)
{
	int i, j, lPolygonCount = pMesh->GetPolygonCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	char header[100];

	vertexCount = 3 * lPolygonCount;
	
	DisplayString("    Polygons");

	int vertexId = 0;
	//MeshRef = pMesh;


	for (i = 0; i < lPolygonCount; i++) // i = nrOfFaces
	{
		DisplayInt("        Polygon ", i);
		int l;
		
		
		for (l = 0; l < pMesh->GetElementPolygonGroupCount(); l++)
		{
			FbxGeometryElementPolygonGroup* lePolgrp = pMesh->GetElementPolygonGroup(l);
			switch (lePolgrp->GetMappingMode())
			{
			case FbxGeometryElement::eByPolygon:
				if (lePolgrp->GetReferenceMode() == FbxGeometryElement::eIndex)
				{
					FBXSDK_sprintf(header, 100, "        Assigned to group: ");
					int polyGroupId = lePolgrp->GetIndexArray().GetAt(i);
					DisplayInt(header, polyGroupId);
					break;
				}
			default:
				// any other mapping modes don't make sense
				DisplayString("        \"unsupported group assignment\"");
				break;
			}
		}

		int lPolygonSize = pMesh->GetPolygonSize(i);
		
		FbxVector4 currentBiNormal;
		FbxVector4 currentTangent;

		for (j = 0; j < lPolygonSize; j++) //j = nrOfVertexInFace
		{
			
			int lControlPointIndex = pMesh->GetPolygonVertex(i, j);
			if (lControlPointIndex < 0)
			{
				DisplayString("            Coordinates: Invalid index found!");
				continue;
			}

			else
			{
				Display3DVector("            Coordinates: ", lControlPoints[lControlPointIndex]);
				//vertexData[currentVertex].pos[0] = lControlPoints[lControlPointIndex][0];
				//vertexData[currentVertex].pos[1] = lControlPoints[lControlPointIndex][1];
				//vertexData[currentVertex].pos[2] = lControlPoints[lControlPointIndex][2]; //Add VertexPos <------------------------------------------
				
			}

			for (l = 0; l < pMesh->GetElementVertexColorCount(); l++)
			{
				FbxGeometryElementVertexColor* leVtxc = pMesh->GetElementVertexColor(l);
				FBXSDK_sprintf(header, 100, "            Color vertex: ");

				switch (leVtxc->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(lControlPointIndex);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					switch (leVtxc->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(vertexId));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leVtxc->GetIndexArray().GetAt(vertexId);
						DisplayColor(header, leVtxc->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}
			for (l = 0; l < pMesh->GetElementUVCount(); ++l)
			{
				FbxGeometryElementUV* leUV = pMesh->GetElementUV(l);
				FBXSDK_sprintf(header, 100, "            Texture UV: ");

				switch (leUV->GetMappingMode())
				{
				default:
					break;
				case FbxGeometryElement::eByControlPoint:
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display2DVector(header, leUV->GetDirectArray().GetAt(lControlPointIndex));
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leUV->GetIndexArray().GetAt(lControlPointIndex);
						Display2DVector(header, leUV->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
				{
					int lTextureUVIndex = pMesh->GetTextureUVIndex(i, j);
					switch (leUV->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
					case FbxGeometryElement::eIndexToDirect:
					{
						Display2DVector(header, leUV->GetDirectArray().GetAt(lTextureUVIndex));
						//vertexData[currentVertex].uv[0] = leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						//vertexData[currentVertex].uv[1] = leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]; //Add UV <------------------------------------------
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				}
			}

			for (l = 0; l < pMesh->GetElementTangentCount(); ++l)
			{
				FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(l);
				FBXSDK_sprintf(header, 100, "            Tangent: ");

				if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					//myStringFile.writeToStringFile("khaskd\n");
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						//myStringFile.writeToStringFile("IASHDKHASD\n");
						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leTangent->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leTangent->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				currentTangent = leTangent->GetDirectArray().GetAt(vertexId);
				//vertexData[currentVertex].tan[0] = currentTangent[0];
				//vertexData[currentVertex].tan[1] = currentTangent[1];
				//vertexData[currentVertex].tan[2] = currentTangent[2]; //Add Tangent <------------------------------------------

			}
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);
				//leBinormal->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
				
				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));

						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leBinormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(id));
					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				currentBiNormal = leBinormal->GetDirectArray().GetAt(vertexId);
				//vertexData[currentVertex].biNorm[0] = currentBiNormal[0];
				//vertexData[currentVertex].biNorm[1] = currentBiNormal[1];
				//vertexData[currentVertex].biNorm[2] = currentBiNormal[2]; //Add BiNormal <------------------------------------------
			}

			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "            Normal: ");
				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));		

						break;
					case FbxGeometryElement::eIndexToDirect:
					{
						int id = leNormal->GetIndexArray().GetAt(vertexId);
						Display3DVector(header, leNormal->GetDirectArray().GetAt(id));

					}
					break;
					default:
						break; // other reference modes not shown here!
					}
				}
				FbxVector4 currentNormal;
				currentNormal[0] = (currentTangent[1] * currentBiNormal[2] - currentTangent[2] * currentBiNormal[1]);
				currentNormal[1] = (currentTangent[2] * currentBiNormal[0] - currentTangent[0] * currentBiNormal[2]);
				currentNormal[2] = (currentTangent[0] * currentBiNormal[1] - currentTangent[1] * currentBiNormal[0]);
				currentNormal[3] = 0;
				//vertexData[currentVertex].norm[0] = currentNormal[0];
				//vertexData[currentVertex].norm[1] = currentNormal[1];
				//vertexData[currentVertex].norm[2] = currentNormal[2]; //Add Normal <------------------------------------------		
			}
			vertexId++;
			currentVertex += 1;
		} // for polygonSize
	} // for polygonCount

	//myFile.writeToFile((const char*)&vertexData, sizeof(Vertex)); //Add vertex data to output <------------------------------------------ 


	//check visibility for the edges of the mesh
	for (int l = 0; l < pMesh->GetElementVisibilityCount(); ++l)
	{
		FbxGeometryElementVisibility* leVisibility = pMesh->GetElementVisibility(l);
		FBXSDK_sprintf(header, 100, "    Edge Visibility : ");
		DisplayString(header);
		switch (leVisibility->GetMappingMode())
		{
		default:
			break;
			//should be eByEdge
		case FbxGeometryElement::eByEdge:
			//should be eDirect
			for (j = 0; j != pMesh->GetMeshEdgeCount(); ++j)
			{
				DisplayInt("        Edge ", j);
				DisplayBool("              Edge visibility: ", leVisibility->GetDirectArray().GetAt(j));
			}

			break;
		}
	}
	DisplayString("");
}

void DisplayTextureNames(FbxProperty& pProperty, FbxString& pConnectionString)
{
	int lLayeredTextureCount = pProperty.GetSrcObjectCount<FbxLayeredTexture>();
	if (lLayeredTextureCount > 0)
	{
		for (int j = 0; j < lLayeredTextureCount; ++j)
		{
			FbxLayeredTexture* lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
			pConnectionString += " Texture ";

			for (int k = 0; k < lNbTextures; ++k)
			{
				//lConnectionString += k;
				pConnectionString += "\"";
				pConnectionString += (char*)lLayeredTexture->GetName();
				pConnectionString += "\"";
				pConnectionString += " ";
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " on layer ";
			pConnectionString += j;
		}
		pConnectionString += " |";
	}
	else
	{
		//no layered texture simply get on the property
		int lNbTextures = pProperty.GetSrcObjectCount<FbxTexture>();

		if (lNbTextures > 0)
		{
			pConnectionString += " Texture ";
			pConnectionString += " ";

			for (int j = 0; j < lNbTextures; ++j)
			{
				FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(j);
				if (lTexture)
				{
					pConnectionString += "\"";
					pConnectionString += (char*)lTexture->GetName();
					pConnectionString += "\"";
					pConnectionString += " ";
				}
			}
			pConnectionString += "of ";
			pConnectionString += pProperty.GetName();
			pConnectionString += " |";
		}
	}
}

void DisplayMaterialTextureConnections(FbxSurfaceMaterial* pMaterial, char* header, int pMatId, int l)
{
	if (!pMaterial)
		return;

	FbxString lConnectionString = "            Material %d -- ";
	//Show all the textures

	FbxProperty lProperty;
	//Diffuse Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	DisplayTextureNames(lProperty, lConnectionString);

	//DiffuseFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Emissive Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	DisplayTextureNames(lProperty, lConnectionString);

	//EmissiveFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sEmissiveFactor);
	DisplayTextureNames(lProperty, lConnectionString);


	//Ambient Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbient);
	DisplayTextureNames(lProperty, lConnectionString);

	//AmbientFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sAmbientFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Specular Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	DisplayTextureNames(lProperty, lConnectionString);

	//SpecularFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sSpecularFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Shininess Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	DisplayTextureNames(lProperty, lConnectionString);

	//Bump Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sBump);
	DisplayTextureNames(lProperty, lConnectionString);

	//Normal Map Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	DisplayTextureNames(lProperty, lConnectionString);

	//Transparent Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparentColor);
	DisplayTextureNames(lProperty, lConnectionString);

	//TransparencyFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sTransparencyFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Reflection Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflection);
	DisplayTextureNames(lProperty, lConnectionString);

	//ReflectionFactor Textures
	lProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sReflectionFactor);
	DisplayTextureNames(lProperty, lConnectionString);

	//Update header with material info
	bool lStringOverflow = (lConnectionString.GetLen() + 10 >= MAT_HEADER_LENGTH); // allow for string length and some padding for "%d"
	if (lStringOverflow)
	{
		// Truncate string!
		lConnectionString = lConnectionString.Left(MAT_HEADER_LENGTH - 10);
		lConnectionString = lConnectionString + "...";
	}
	FBXSDK_sprintf(header, MAT_HEADER_LENGTH, lConnectionString.Buffer(), pMatId, l);
	DisplayString(header);
}

void DisplayMaterialConnections(FbxMesh* pMesh)
{
	int i, l, lPolygonCount = pMesh->GetPolygonCount();

	char header[MAT_HEADER_LENGTH];

	DisplayString("    Polygons Material Connections");

	//check whether the material maps with only one mesh
	bool lIsAllSame = true;
	for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{

		FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
		if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
		{
			lIsAllSame = false;
			break;
		}
	}

	//For eAllSame mapping type, just out the material and texture mapping info once
	if (lIsAllSame)
	{
		for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
		{

			FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
			if (lMaterialElement->GetMappingMode() == FbxGeometryElement::eAllSame)
			{
				FbxSurfaceMaterial* lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(0));
				int lMatId = lMaterialElement->GetIndexArray().GetAt(0);
				if (lMatId >= 0)
				{
					DisplayInt("        All polygons share the same material in mesh ", l);
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}

		//no material
		if (l == 0)
			DisplayString("        no material applied");
	}

	//For eByPolygon mapping type, just out the material and texture mapping info once
	else
	{
		for (i = 0; i < lPolygonCount; i++)
		{
			DisplayInt("        Polygon ", i);

			for (l = 0; l < pMesh->GetElementMaterialCount(); l++)
			{

				FbxGeometryElementMaterial* lMaterialElement = pMesh->GetElementMaterial(l);
				FbxSurfaceMaterial* lMaterial = NULL;
				int lMatId = -1;
				lMaterial = pMesh->GetNode()->GetMaterial(lMaterialElement->GetIndexArray().GetAt(i));
				lMatId = lMaterialElement->GetIndexArray().GetAt(i);

				if (lMatId >= 0)
				{
					DisplayMaterialTextureConnections(lMaterial, header, lMatId, l);
				}
			}
		}
	}
}


void DisplayMaterialMapping(FbxMesh* pMesh)
{
	const char* lMappingTypes[] = { "None", "By Control Point", "By Polygon Vertex", "By Polygon", "By Edge", "All Same" };
	const char* lReferenceMode[] = { "Direct", "Index", "Index to Direct" };

	int lMtrlCount = 0;
	FbxNode* lNode = NULL;
	if (pMesh) {
		lNode = pMesh->GetNode();
		if (lNode)
			lMtrlCount = lNode->GetMaterialCount();
	}

	for (int l = 0; l < pMesh->GetElementMaterialCount(); l++)
	{
		FbxGeometryElementMaterial* leMat = pMesh->GetElementMaterial(l);
		if (leMat)
		{
			char header[100];
			FBXSDK_sprintf(header, 100, "    Material Element %d: ", l);
			DisplayString(header);


			DisplayString("           Mapping: ", lMappingTypes[leMat->GetMappingMode()]);
			DisplayString("           ReferenceMode: ", lReferenceMode[leMat->GetReferenceMode()]);

			int lMaterialCount = 0;
			FbxString lString;

			if (leMat->GetReferenceMode() == FbxGeometryElement::eDirect ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				lMaterialCount = lMtrlCount;
			}

			if (leMat->GetReferenceMode() == FbxGeometryElement::eIndex ||
				leMat->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				int i;

				lString = "           Indices: ";

				int lIndexArrayCount = leMat->GetIndexArray().GetCount();
				for (i = 0; i < lIndexArrayCount; i++)
				{
					lString += leMat->GetIndexArray().GetAt(i);

					if (i < lIndexArrayCount - 1)
					{
						lString += ", ";
					}
				}

				lString += "\n";

				FBXSDK_printf(lString);
			}
		}
	}

	DisplayString("");
}
