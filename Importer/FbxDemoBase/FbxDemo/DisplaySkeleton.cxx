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
std::vector<FbxNode*> listOfJoints;
std::vector<std::string> parents;

void DisplaySkeleton(FbxNode* pNode)
{ 
    nrOfJoints++;
    FbxSkeleton* lSkeleton = (FbxSkeleton*) pNode->GetNodeAttribute();
    
    DisplayString("Skeleton Name: ", (char *) pNode->GetName());
    DisplayMetaDataConnections(lSkeleton);

    names.push_back(pNode->GetName());
    listOfJoints.push_back(pNode);
    parents.push_back(pNode->GetParent()->GetName());

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

std::vector<int> GetJointParent()
{
    std::vector<int> list;
    bool foundAParent = false;

    list.resize(nrOfJoints);
    for (int j = 0; j < nrOfJoints; j++)
    {
        foundAParent = false;
        for (int j2 = 0; j2 < nrOfJoints; j2++)
        {
            if (parents[j] == names[j2])
            {
                list[j] = j2;
                foundAParent = true;
            }
            if (j2 == nrOfJoints && foundAParent == false)
            {
                list[j] = -1;
            }
        }
    }
    list[0] = -1;

    int stop = 0;
    return list;
}
