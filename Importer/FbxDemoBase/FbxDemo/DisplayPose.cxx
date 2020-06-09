/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayPose.h"
std::vector<JointBFF> jointData;

void DisplayPose(FbxScene* pScene)
{
    int      i,j,k,lPoseCount;
    FbxString  lName;

    lPoseCount = pScene->GetPoseCount();
    int nrOf = 0;
    int nrOf2 = 0;
    for (i = 0; i < lPoseCount; i++)
    {
        FbxPose* lPose = pScene->GetPose(i);
        jointData.resize(lPose->GetCount() - 1);
        
        lName = lPose->GetName();
        DisplayString("Pose Name: ", lName.Buffer());

        DisplayBool("    Is a bind pose: ", lPose->IsBindPose());

        DisplayInt("    Number of items in the pose: ", lPose->GetCount());

        DisplayString("","");

        for (j=0; j<lPose->GetCount(); j++)
        {
            lName = lPose->GetNodeName(j).GetCurrentName();
            DisplayString("    Item name: ", lName.Buffer());

            if (!lPose->IsBindPose())
            {
                // Rest pose can have local matrix
                DisplayBool("    Is local space matrix: ", lPose->IsLocalMatrix(j));
            }

            DisplayString("    Matrix value: ","");

            FbxString lMatrixValue;
            nrOf2 = 0;
            for (k=0; k<4; k++)
            {
                
                FbxMatrix  lMatrix = lPose->GetMatrix(j);
                FbxVector4 lRow = lMatrix.GetRow(k);
                if (j >= 1)
                {
                    jointData[j - 1].jointIndex = j - 1;

                    nrOf = 0;
                    jointData[j - 1].bindPoseMatrix[nrOf2] = lMatrix.GetRow(k)[nrOf];
                    nrOf++;
                    nrOf2++;
                    jointData[j - 1].bindPoseMatrix[nrOf2] = lMatrix.GetRow(k)[nrOf];
                    nrOf++;
                    nrOf2++;
                    jointData[j - 1].bindPoseMatrix[nrOf2] = lMatrix.GetRow(k)[nrOf];
                    nrOf++;
                    nrOf2++;
                    jointData[j - 1].bindPoseMatrix[nrOf2] = lMatrix.GetRow(k)[nrOf];
                    nrOf++;
                    nrOf2++;
                }

                char        lRowValue[1024];
                
                FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
                lMatrixValue += FbxString("        ") + FbxString(lRowValue);

            }
            
            DisplayString("", lMatrixValue.Buffer());
        }
    }
    
    lPoseCount = pScene->GetCharacterPoseCount();

    for (i = 0; i < lPoseCount; i++)
    {
        FbxCharacterPose* lPose = pScene->GetCharacterPose(i);
        FbxCharacter*     lCharacter = lPose->GetCharacter();

        if (!lCharacter) break;

		DisplayString("Character Pose Name: ", lCharacter->GetName());

        FbxCharacterLink lCharacterLink;
        FbxCharacter::ENodeId  lNodeId = FbxCharacter::eHips;

        while (lCharacter->GetCharacterLink(lNodeId, &lCharacterLink))
        {
            FbxAMatrix& lGlobalPosition = lCharacterLink.mNode->EvaluateGlobalTransform(FBXSDK_TIME_ZERO);

            DisplayString("    Matrix value: ","");

            FbxString lMatrixValue;

            for (k=0; k<4; k++)
            {
                FbxVector4 lRow = lGlobalPosition.GetRow(k);
                char        lRowValue[1024];

                FBXSDK_sprintf(lRowValue, 1024, "%9.4f %9.4f %9.4f %9.4f\n", lRow[0], lRow[1], lRow[2], lRow[3]);
                lMatrixValue += FbxString("        ") + FbxString(lRowValue);
            }

            DisplayString("", lMatrixValue.Buffer());

            lNodeId = FbxCharacter::ENodeId(int(lNodeId) + 1);
        }
    }
    int stop = 0;
}

std::vector<JointBFF> GetJointData()
{
    return jointData;
}


