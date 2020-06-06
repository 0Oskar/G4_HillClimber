/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplayShape.h"
std::vector<std::vector<BlendShapesBFF>> blendShapeDataArr;
std::vector<std::vector<BlendshapeBFF>> newBlendShapeArr;
int currentShape = 0;
int nrOfShapes = 0;
int nrOfVertexInShape;
int nrOfControlPointsInShape;

void DisplayShape(FbxGeometry* pGeometry)
{
    int lBlendShapeCount, lBlendShapeChannelCount, lTargetShapeCount;
    FbxBlendShape* lBlendShape;
	FbxBlendShapeChannel* lBlendShapeChannel;
	FbxShape* lShape;

    lBlendShapeCount = pGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
	nrOfShapes = lBlendShapeCount;
	blendShapeDataArr.resize(lBlendShapeCount);
	newBlendShapeArr.resize(nrOfShapes);

    for (int lBlendShapeIndex = 0; lBlendShapeIndex < lBlendShapeCount; ++lBlendShapeIndex)
    {
		lBlendShape = (FbxBlendShape*) pGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape);   
        DisplayString("    BlendShape ", (char *) lBlendShape->GetName());
		

		lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
		for(int lBlendShapeChannelIndex = 0;  lBlendShapeChannelIndex < lBlendShapeChannelCount; ++lBlendShapeChannelIndex)
		{
			lBlendShapeChannel = lBlendShape->GetBlendShapeChannel(lBlendShapeChannelIndex);
			DisplayString("    BlendShapeChannel ", (char *) lBlendShapeChannel->GetName());
			DisplayDouble("    Default Deform Value: ", lBlendShapeChannel->DeformPercent.Get());

			lTargetShapeCount = lBlendShapeChannel->GetTargetShapeCount();
			for (int lTargetShapeIndex = 0; lTargetShapeIndex < lTargetShapeCount; ++lTargetShapeIndex)
			{
				lShape = lBlendShapeChannel->GetTargetShape(lTargetShapeIndex);
				DisplayString("    TargetShape ", (char *) lShape->GetName());

				int j, lControlPointsCount = lShape->GetControlPointsCount();
				FbxVector4* lControlPoints = lShape->GetControlPoints();
				FbxLayerElementArrayTemplate<FbxVector4>* lNormals = NULL;    
				bool lStatus = lShape->GetNormals(&lNormals); 

				blendShapeDataArr[currentShape].resize(lShape->GetControlPointsCount());
				newBlendShapeArr[currentShape].resize(lShape->GetControlPointsCount());
				nrOfVertexInShape = lShape->GetControlPointsCount();

				for(j = 0; j < lControlPointsCount; j++)
				{
					DisplayInt("        Control Point ", j);
					Display3DVector("            Coordinates: ", lControlPoints[j]);
					//pMesh->GetPolygonVertex(i, j);

					blendShapeDataArr[currentShape][j].pos[0] = lControlPoints[j][0];
					blendShapeDataArr[currentShape][j].pos[1] = lControlPoints[j][1];
					blendShapeDataArr[currentShape][j].pos[2] = lControlPoints[j][2];

					newBlendShapeArr[currentShape][j].pos[0] = lControlPoints[j][0];
					newBlendShapeArr[currentShape][j].pos[1] = lControlPoints[j][1];
					newBlendShapeArr[currentShape][j].pos[2] = lControlPoints[j][2];
					if (lStatus && lNormals && lNormals->GetCount() == lControlPointsCount)
					{
						Display3DVector("            Normal Vector: ", lNormals->GetAt(j));

						blendShapeDataArr[currentShape][j].norm[0] = lNormals->GetAt(j)[0];
						blendShapeDataArr[currentShape][j].norm[1] = lNormals->GetAt(j)[1];
						blendShapeDataArr[currentShape][j].norm[2] = lNormals->GetAt(j)[2];

						newBlendShapeArr[currentShape][j].norm[0] = lNormals->GetAt(j)[0];
						newBlendShapeArr[currentShape][j].norm[1] = lNormals->GetAt(j)[1];
						newBlendShapeArr[currentShape][j].norm[2] = lNormals->GetAt(j)[2];
					}
				}

				DisplayString("");
			}
		}
		currentShape++;
    }
}


std::vector<std::vector<BlendShapesBFF>> GetBlendShapeDataArr()
{
	return blendShapeDataArr;
}

std::vector<std::vector<BlendshapeBFF>> GetNewBlendShapeData()
{
	return newBlendShapeArr;
}

int GetNrOfVertexInBlendShape()
{
	return nrOfVertexInShape;
}

int GetNrOfControlPointsInBlendShape()
{
	return 0;
}

int GetNrOfBlendShapes()
{
	return nrOfShapes;
}


