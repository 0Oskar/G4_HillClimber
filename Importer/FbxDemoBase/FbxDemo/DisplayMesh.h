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
std::vector<VertexBFF> GetVertxData();
MaterialBFF GetMaterialData2();

std::vector<std::vector<BlendShapesBFF>> GetBlendShapeDataArr2();
int GetNrOfVertexInBlendShape2();
int GetNrOfBlendShapes2();


#endif // #ifndef _DISPLAY_MESH_H


