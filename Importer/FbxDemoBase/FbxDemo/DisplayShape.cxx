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

std::vector<BlendShapesBFF> blendShapeData;
//std::atoi

void DisplayShape(FbxGeometry* pGeometry)
{
    int lBlendShapeCount, lBlendShapeChannelCount, lTargetShapeCount;
    FbxBlendShape* lBlendShape;
	FbxBlendShapeChannel* lBlendShapeChannel;
	FbxShape* lShape;

    lBlendShapeCount = pGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
	//myStringFile5.writeToStringFile("\n\n\n------------- BlendShapes:\n\n");

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
				blendShapeData.resize(lShape->GetControlPointsCount());

				for(j = 0; j < lControlPointsCount; j++)
				{
					DisplayInt("        Control Point ", j);
					Display3DVector("            Coordinates: ", lControlPoints[j]);
					blendShapeData[j].pos[0] = lControlPoints[j][0];
					blendShapeData[j].pos[1] = lControlPoints[j][1];
					blendShapeData[j].pos[2] = lControlPoints[j][2];
					if (lStatus && lNormals && lNormals->GetCount() == lControlPointsCount)
					{
						Display3DVector("            Normal Vector: ", lNormals->GetAt(j));
						blendShapeData[j].norm[0] = lNormals->GetAt(j)[0];
						blendShapeData[j].norm[1] = lNormals->GetAt(j)[1];
						blendShapeData[j].norm[2] = lNormals->GetAt(j)[2];
					}
							//myStringFile5.writeToStringFile(
				   //         "PosX: " + std::to_string(blendShapeData[j].pos[0]) +
				   //         "\n" +
				   //         "PosY: " + std::to_string(blendShapeData[j].pos[1]) +
				   //         "\n" +
				   //         "PosZ: " + std::to_string(blendShapeData[j].pos[2]) +
				   //         "\n" +
				   //         "NormX: " + std::to_string(blendShapeData[j].norm[0]) +
				   //         "\n" +
				   //         "NormY: " + std::to_string(blendShapeData[j].norm[1]) +
				   //         "\n" +
				   //         "NormZ: " + std::to_string(blendShapeData[j].norm[2]) +
				   //         "\n\n");
				}

				DisplayString("");
			}
		}
    }
}

std::vector<BlendShapesBFF> GetBlendShapeData()
{
	return blendShapeData;
}


