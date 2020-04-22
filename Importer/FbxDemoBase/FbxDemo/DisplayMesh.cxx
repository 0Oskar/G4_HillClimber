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

//#include "FileWrite.h"
//#include "FileWrite.h"

#if defined (FBXSDK_ENV_MAC)
// disable the �format not a string literal and no format arguments� warning since
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
std::vector<Vertex> vertexData;
//Vertex vertexData[];
Mesh meshData;
int currentVertex = 0;

void DisplayMesh(FbxNode* pNode)
{
	myFile.EmptyFile();
	//Vertex vertexData;

	FbxMesh* lMesh = (FbxMesh*)pNode->GetNodeAttribute();

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

	myFile.writeToFile((const char*)&meshData, sizeof(Mesh)); //Add mesh data to output <------------------------------------------
	myStringFile.writeToStringFile(meshData.name + std::to_string(meshData.nrOfVertex));
	for (int i = 0; i < vertexCount; i++)
	{
		myFile.writeToFile((const char*)&vertexData[i], sizeof(Vertex)); //Add vertex data to output <------------------------------------------ 
		
		myStringFile.writeToStringFile(
			"\n------------- Index " + std::to_string(i) + ")\n\n" +
			"PosX: " + std::to_string(vertexData[i].pos[0]) + "\n" +
			"PosY: " + std::to_string(vertexData[i].pos[1]) + "\n" +
			"PosZ: " + std::to_string(vertexData[i].pos[2]) + "\n" +
			"\n" +
			"U: " + std::to_string(vertexData[i].uv[0]) + "\n" +
			"V: " + std::to_string(vertexData[i].uv[1]) + "\n" +
			"\n" +
			"NormX: " + std::to_string(vertexData[i].norm[0]) + "\n" +
			"NormY: " + std::to_string(vertexData[i].norm[1]) + "\n" +
			"NormZ: " + std::to_string(vertexData[i].norm[2]) + "\n" +
			"\n" +
			"biNormX: " + std::to_string(vertexData[i].biNorm[0]) + "\n" +
			"biNormY: " + std::to_string(vertexData[i].biNorm[1]) + "\n" +
			"biNormZ: " + std::to_string(vertexData[i].biNorm[2]) + "\n" +
			"\n" +
			"TanX: " + std::to_string(vertexData[i].tan[0]) + "\n" +
			"TanY: " + std::to_string(vertexData[i].tan[1]) + "\n" +
			"TanZ: " + std::to_string(vertexData[i].tan[2]) + "\n");

	}

	DisplayMaterialMapping(lMesh);
	DisplayMaterial(lMesh);
	DisplayTexture(lMesh);
	DisplayMaterialConnections(lMesh);
	DisplayLink(lMesh);
	DisplayShape(lMesh);

	DisplayCache(lMesh);
}


void DisplayControlsPoints(FbxMesh* pMesh)
{
	int i, lControlPointsCount = pMesh->GetControlPointsCount();
	FbxVector4* lControlPoints = pMesh->GetControlPoints();
	//vertexCount = lControlPointsCount;
	//vertexData.resize(vertexCount);
	//meshData.nrOfVertex = vertexCount;

	DisplayString("    Control Points");
	std::string VertexData = "";
	for (i = 0; i < lControlPointsCount; i++)
	{
		DisplayInt("        Control Point ", i);
		Display3DVector("            Coordinates: ", lControlPoints[i]);
		/*vertexData[currentVertex].pos[0] = lControlPoints[i][0];
		vertexData[currentVertex].pos[1] = lControlPoints[i][1];
		vertexData[currentVertex].pos[2] = lControlPoints[i][2]; *///Add VertexPos <------------------------------------------

		for (int j = 0; j < pMesh->GetElementNormalCount(); j++)
		{
			FbxGeometryElementNormal* leNormals = pMesh->GetElementNormal(j);
			//FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(j);
			//FbxGeometryElementBinormal* leBiNormals = pMesh->GetElementBinormal(j);
			if (leNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			{
				char header[100];

				FBXSDK_sprintf(header, 100, "            Normal Vector: ");
				if (leNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				{
					auto array = leNormals->GetDirectArray().GetAt(i);
					Display3DVector(header, array);
					//FbxVector4 temp = leNormals->GetDirectArray().GetAt(i);
					//vertexData[currentVertex].norm[0] = array[0];
					//vertexData[currentVertex].norm[1] = array[1];
					//vertexData[currentVertex].norm[2] = array[2]; //Add Normal <------------------------------------------	
				}

				//FBXSDK_sprintf(header, 100, "            Tanget Vector: ");
				//if (leTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
				//{
				//	//Display3DVector(header, leTangent->GetDirectArray().GetAt(i));
				//	//vertexData[currentVertex].tan[0] = leTangent->GetDirectArray().GetAt(i)[0];
				//	//vertexData[currentVertex].tan[1] = leTangent->GetDirectArray().GetAt(i)[1];
				//	//vertexData[currentVertex].tan[2] = leTangent->GetDirectArray().GetAt(i)[2]; //Add Tangent <------------------------------------------
				//}

				//FBXSDK_sprintf(header, 100, "            BiNormal Vector: ");
				//if (leBiNormals->GetReferenceMode() == FbxGeometryElement::eDirect)
				//{
				//	Display3DVector(header, leBiNormals->GetDirectArray().GetAt(i));
				//	//vertexData[currentVertex].norm[0] = leBiNormals->GetDirectArray().GetAt(i)[0];
				//	//vertexData[currentVertex].norm[1] = leBiNormals->GetDirectArray().GetAt(i)[1];
				//	//vertexData[currentVertex].norm[2] = leBiNormals->GetDirectArray().GetAt(i)[2]; //Add BiNormals <------------------------------------------
				//}

			}
			//if (leBiNormals->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			//{
			//	myStringFile.writeToStringFile("Bajs");
			//}
			//currentVertex += 1;
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
	vertexData.resize(vertexCount);
	meshData.nrOfVertex = vertexCount;
	DisplayString("    Polygons");

	int vertexId = 0;
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
				vertexData[currentVertex].pos[0] = lControlPoints[lControlPointIndex][0];
				vertexData[currentVertex].pos[1] = lControlPoints[lControlPointIndex][1];
				vertexData[currentVertex].pos[2] = lControlPoints[lControlPointIndex][2]; //Add VertexPos <------------------------------------------
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
						vertexData[currentVertex].uv[0] = leUV->GetDirectArray().GetAt(lTextureUVIndex)[0];
						vertexData[currentVertex].uv[1] = leUV->GetDirectArray().GetAt(lTextureUVIndex)[1]; //Add UV <------------------------------------------
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
					switch (leTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leTangent->GetDirectArray().GetAt(vertexId));
						currentTangent = leTangent->GetDirectArray().GetAt(vertexId);
						vertexData[currentVertex].tan[0] = currentTangent[0];
						vertexData[currentVertex].tan[1] = currentTangent[1];
						vertexData[currentVertex].tan[2] = currentTangent[2]; //Add Tangent <------------------------------------------
						
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

			}
			for (l = 0; l < pMesh->GetElementBinormalCount(); ++l)
			{

				FbxGeometryElementBinormal* leBinormal = pMesh->GetElementBinormal(l);
				
				FBXSDK_sprintf(header, 100, "            Binormal: ");
				if (leBinormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leBinormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leBinormal->GetDirectArray().GetAt(vertexId));
						currentBiNormal = leBinormal->GetDirectArray().GetAt(vertexId);
						vertexData[currentVertex].biNorm[0] = currentBiNormal[0];
						vertexData[currentVertex].biNorm[1] = currentBiNormal[1];
						vertexData[currentVertex].biNorm[2] = currentBiNormal[2]; //Add BiNormal <------------------------------------------
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
			}

			for (l = 0; l < pMesh->GetElementNormalCount(); ++l)
			{
				FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(l);
				FBXSDK_sprintf(header, 100, "            Normal: ");
				//myStringFile.writeToStringFile(std::to_string(l));
				//leNormal->SetMappingMode(FbxGeometryElement::eByPolygonVertex);
				if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (leNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						Display3DVector(header, leNormal->GetDirectArray().GetAt(vertexId));
						//FbxVector4 currentNormal;
						//currentNormal[0] = ( currentTangent[1] * currentBiNormal[2] - currentTangent[2] * currentBiNormal[1] );
						//currentNormal[1] = ( currentTangent[2] * currentBiNormal[0] - currentTangent[0] * currentBiNormal[2] );
						//currentNormal[2] = ( currentTangent[0] * currentBiNormal[1] - currentTangent[1] * currentBiNormal[0] );
						//currentNormal[3] = 0;
						//vertexData[currentVertex].norm[0] = currentNormal[0];
						//vertexData[currentVertex].norm[1] = currentNormal[1];
						//vertexData[currentVertex].norm[2] = currentNormal[2]; //Add Normal <------------------------------------------				

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
				vertexData[currentVertex].norm[0] = currentNormal[0];
				vertexData[currentVertex].norm[1] = currentNormal[1];
				vertexData[currentVertex].norm[2] = currentNormal[2]; //Add Normal <------------------------------------------		
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