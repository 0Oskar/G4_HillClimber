/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLink.h"
#include "algorithm"
#if defined (FBXSDK_ENV_MAC)
// disable the “format not a string literal and no format arguments” warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

std::vector<JointBFF> jointData;
std::vector<ControlPointBFF> controlPointDataTemp;

bool sortbysec(const std::pair<int, float>& a, const std::pair<int, float>& b)
{
    return (a.second > b.second);
}

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
    int nrOfJoints = 0;


    //lLinkCount = pGeometry->GetLinkCount();
    for(i=0; i!=lSkinCount; ++i)
    {
        lClusterCount = ((FbxSkin *) pGeometry->GetDeformer(i, FbxDeformer::eSkin))->GetClusterCount();
        nrOfJoints = lClusterCount;
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
    //int nrOfJoints = ((FbxSkin*)pGeometry->GetDeformer(0, FbxDeformer::eSkin))->GetClusterCount();
    int nrOfInfluencedControlPoints = 0;

    std::vector<std::vector<std::pair<int, float>>> arr;

    arr.resize(nrOfControlPoints);

    for (int j = 0; j < nrOfJoints; j++)
    {
        lCluster = ((FbxSkin*)pGeometry->GetDeformer(0, FbxDeformer::eSkin))->GetCluster(j);
        nrOfInfluencedControlPoints = lCluster->GetControlPointIndicesCount();

        for (int i = 0; i < nrOfInfluencedControlPoints; i++)
        {
            int* indices = lCluster->GetControlPointIndices();
            double* lWeights = lCluster->GetControlPointWeights();
            
            arr[indices[i]].push_back(std::make_pair(j, (float)lWeights[i]));
        }
    }


    //Sort
    for (int c = 0; c < nrOfControlPoints; c++)
    {
        std::sort(arr[c].begin(), arr[c].end(), sortbysec);
    }


    //if less then 4 influences
    for (int c = 0; c < nrOfControlPoints; c++)
    {
        //int testing = arr[c].size();
        if (arr[c].size() > 4)
        {
            int nrOfExtras = arr[c].size() - 4;
            float extraWeight = 0;
            //Om fler
            for (int e = 0; e < nrOfExtras; e++)
            {
                extraWeight += arr[c][4 + e].second;
            }
            extraWeight = extraWeight / 4;
            arr[c][0].second += extraWeight;
            arr[c][1].second += extraWeight;
            arr[c][2].second += extraWeight;
            arr[c][3].second += extraWeight;
        }

        if (arr[c].size() < 4)
        {
            int nrOfMissing = 4 - arr[c].size();
      
            //Om färre
            for (int e = 0; e < nrOfMissing; e++)
            {
                arr[c].push_back(std::make_pair(-1, 0));
            }
        }

    }

    //Tilldela till structen
    controlPointDataTemp.resize(nrOfControlPoints);
    for (int c = 0; c < nrOfControlPoints; c++)
    {
        controlPointDataTemp[c].influencedByJoints[0] = arr[c][0].first;
        controlPointDataTemp[c].influencedByJoints[1] = arr[c][1].first;
        controlPointDataTemp[c].influencedByJoints[2] = arr[c][2].first;
        controlPointDataTemp[c].influencedByJoints[3] = arr[c][3].first;

        controlPointDataTemp[c].boneWeight[0] = arr[c][0].second;
        controlPointDataTemp[c].boneWeight[1] = arr[c][1].second;
        controlPointDataTemp[c].boneWeight[2] = arr[c][2].second;
        controlPointDataTemp[c].boneWeight[3] = arr[c][3].second;
    }
    int num = 0;
}

std::vector<ControlPointBFF> GetControlPointJointData()
{
    return controlPointDataTemp;
}

