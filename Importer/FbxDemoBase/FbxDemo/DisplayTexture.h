/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#ifndef _DISPLAY_TEXTURE_H
#define _DISPLAY_TEXTURE_H

#include "DisplayCommon.h"


void FindAndDisplayTextureInfoByProperty(FbxProperty pProperty, bool& pDisplayHeader, int pMaterialIndex);
void DisplayTexture(FbxGeometry* pGeometry);
void DisplayTextureInfo(FbxTexture* pTexture, int pBlendMode);
//void setTextureName(char* theTexture);
//static char* aTextureName;
//char* getTextureName();
//std::string GetTexturePath();

std::string GetTexturePath();

int getNrOfTextures();

char* GetTexturePath2(char arrayOfPaths[]);
char* getTexturePathArr(int index);

const char* GetTextureType();

const char* GetTextureTypeArr(int index);


#endif // #ifndef _DISPLAY_TEXTURE_H


