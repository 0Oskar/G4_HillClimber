/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_LIGHT_H
#define _DISPLAY_LIGHT_H

#include "DisplayCommon.h"

void DisplayLight(FbxNode* pNode);

int getNrOfLights();
std::vector<LightBFF> GetLightData();
FbxAMatrix getLightTransformMatrix(int index);

#endif // #ifndef _DISPLAY_LIGHT_H


