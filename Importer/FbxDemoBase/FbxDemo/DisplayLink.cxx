/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLink.h"
#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

std::vector<JointBFF> jointData;

void DisplayLink(FbxGeometry* pGeometry)
{
    //Display cluster now

    //int i, lLinkCount;
    //FbxCluster* lLink;

    int i, j;
    int lSkinCount=0;
    int lClusterCount=0;
    FbxCluster* lCluster;

    lSkinCount=pGeometry->GetDeformerCount(FbxDeformer::eSkin);



    //lLinkCount = pGeometry->GetLinkCount();
    for(i=0; i!=lSkinCount; ++i)
    {
        lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        jointData.resize(lClusterCount);
        for (j = 0; j != lClusterCount; ++j)
        {
            DisplayInt("    Cluster ", i);

            lCluster=((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetCluster(j);
            //lLink = pGeometry->GetLink(i);    

            const char* lClusterModes[] = { "Normalize", "Additive", "Total1" };

            DisplayString("    Mode: ", lClusterModes[lCluster->GetLinkMode()]);

            if(lCluster->GetLink() != NULL)
            {
                DisplayString("        Name: ", (char *) lCluster->GetLink()->GetName());
            }

            FbxString lString1 = "        Link Indices: ";
            FbxString lString2 = "        Weight Values: ";

            int k, lIndexCount = lCluster->GetControlPointIndicesCount();
            int* lIndices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();


            for(k = 0; k < lIndexCount; k++)
            {
                lString1 += lIndices[k];
                lString2 += (float) lWeights[k];



                if (k < lIndexCount - 1)
                {
                    lString1 += ", ";
                    lString2 += ", ";
                }
            }
            //jointData[j].influencedControlPoints[0]

            lString1 += "\n";
            lString2 += "\n";

            FBXSDK_printf(lString1);
            FBXSDK_printf(lString2);

            DisplayString("");

            FbxAMatrix lMatrix;

            lMatrix = lCluster->GetTransformMatrix(lMatrix);
            Display3DVector("        Transform Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Scaling: ", lMatrix.GetS());

            lMatrix = lCluster->GetTransformLinkMatrix(lMatrix);
            Display3DVector("        Transform Link Translation: ", lMatrix.GetT());
            Display3DVector("        Transform Link Rotation: ", lMatrix.GetR());
            Display3DVector("        Transform Link Scaling: ", lMatrix.GetS());

            if (lCluster->GetAssociateModel() != NULL)
            {
                lMatrix = lCluster->GetTransformAssociateModelMatrix(lMatrix);
                DisplayString("        Associate Model: ", (char *) lCluster->GetAssociateModel()->GetName());
                Display3DVector("        Associate Model Translation: ", lMatrix.GetT());
                Display3DVector("        Associate Model Rotation: ", lMatrix.GetR());
                Display3DVector("        Associate Model Scaling: ", lMatrix.GetS());
            }

            DisplayString("");
        }
    }

    //EGET:
    int nrOfControlPoints = pGeometry->GetControlPointsCount();
    int nrOfJoints = ((FbxSkin*)pGeometry->GetDeformer(0, FbxDeformer::eSkin))->GetClusterCount();
    int nrOfInfluencedControlPoints = 0;
    std::vector<std::vector<int>> contolIndexList;
    contolIndexList.resize(nrOfControlPoints);

    for (int c = 0; c < nrOfControlPoints; c++)
    {
        for (int j = 0; j < nrOfJoints; j++)
        {
            lCluster = ((FbxSkin*)pGeometry->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(j);
            nrOfInfluencedControlPoints = lCluster->GetControlPointIndicesCount();
            contolIndexList[c].resize(nrOfInfluencedControlPoints);
            for (int i = 0; i < nrOfInfluencedControlPoints; i++)
            {
                int* indices = lCluster->GetControlPointIndices();
                contolIndexList[c][i] == indices[i];
                //if (indices[i] == c)
                //{
                //    
                //}
            }
        }
    }

    int hej = 1;
}

std::vector<JointBFF> GetJointData(int nrOfJoints)
{
    return jointData;
}
/*
for (alla controlPoints) c
{
    for (alla joints) j
    {
        for (alla controlpoint som är influerade) k
        {
            if (lIndices[k] == c)
            {
                Detta betyder att control point c blir influerad av denna jointen
            }
        }
    }
}
*/
