/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplaySkeleton.h"
int nrOfJoints = 0;
std::vector<std::string> names;

void DisplaySkeleton(FbxNode* pNode)
{ 
    nrOfJoints++;
    FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();
    
    DisplayString("Skeleton Name: ", (char *) pNode->GetName());
    DisplayMetaDataConnections(lSkeleton);
    names.push_back(pNode->GetName());

    const char* lSkeletonTypes[] = { "Root", "Limb", "Limb Node", "Effector" };

    DisplayString("    Type: ", lSkeletonTypes[lSkeleton->GetSkeletonType()]);

    if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimb)
    {
        DisplayDouble("    Limb Length: ", lSkeleton->LimbLength.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eLimbNode)
    {
        DisplayDouble("    Limb Node Size: ", lSkeleton->Size.Get());
    }
    else if (lSkeleton->GetSkeletonType() == FbxSkeleton::eRoot)
    {
        DisplayDouble("    Limb Root Size: ", lSkeleton->Size.Get());
    }

    DisplayColor("    Color: ", lSkeleton->GetLimbNodeColor());
}

int getNrOfJoints()
{
    return nrOfJoints;
}

std::vector<std::string> GetJointName()
{
    return names;
}