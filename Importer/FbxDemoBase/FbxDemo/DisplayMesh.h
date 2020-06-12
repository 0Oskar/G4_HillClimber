/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_MESH_H
#define _DISPLAY_MESH_H

#include "DisplayCommon.h"

void DisplayMesh(FbxNode* pNode);
MeshBFF GetMeshData();
std::vector<ControlPointBFF> GetControlPointData();
std::vector<IndexListBFF> GetIndexList();
int GetNrOfMaterials2();
std::vector<MaterialBFF> GetMaterialData2();
int GetNrOfTextures2();
//char* GetTextureName(int index);
std::string GETNAME(int index);


std::string GetTextureType2(int index);
int GetMaterialIndex2(int index);

std::vector<std::vector<BlendshapeBFF>> GetNewBlendShapeData2();
int GetNrOfVertexInBlendShape2();
int GetNrOfBlendShapes2();
char* getTextureName2();
std::vector<JointBFF> GetJointData2(int nrOfJoints);
std::vector<ControlPointBFF> GetControlPointJointData2();
void newFunc(FbxMesh* pMesh);


#endif // #ifndef _DISPLAY_MESH_H


