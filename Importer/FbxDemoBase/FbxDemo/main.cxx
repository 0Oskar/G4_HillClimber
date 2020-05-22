/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to detect if a scene is password 
// protected, import and browse the scene to access node and animation 
// information. It displays the content of the FBX file which name is 
// passed as program argument. You can try it with the various FBX files 
// output by the export examples.
//
/////////////////////////////////////////////////////////////////////////

#include "../Common/Common.h"
#include "DisplayCommon.h"
#include "DisplayHierarchy.h"
#include "DisplayAnimation.h"
#include "DisplayMarker.h"
#include "DisplaySkeleton.h"
#include "DisplayMesh.h"
#include "DisplayNurb.h"
#include "DisplayPatch.h"
#include "DisplayLodGroup.h"
#include "DisplayCamera.h"
#include "DisplayLight.h"
#include "DisplayGlobalSettings.h"
#include "DisplayPose.h"
#include "DisplayPivotsAndLimits.h"
#include "DisplayUserProperties.h"
#include "DisplayGenericInfo.h"

//#include "FileWrite.h"
#include "BiReader.h"

#pragma comment(lib,"libfbxsdk-mt.lib")
#pragma comment(lib,"libxml2-mt.lib")
#pragma comment(lib,"zlib-mt.lib")

// Local function prototypes.
void DisplayContent(FbxScene* pScene);
void DisplayContent(FbxNode* pNode);
void DisplayTarget(FbxNode* pNode);
void DisplayTransformPropagation(FbxNode* pNode);
void DisplayGeometricTransform(FbxNode* pNode);
void DisplayMetaData(FbxScene* pScene);

static bool gVerbose = true;
FileWrite myFile5("../biFile.bff");
FileWrite myStringFile5("../stringFile.bff");

MeshBFF meshData2;
std::vector<VertexBFF> vertexData2;
MaterialBFF materialData3;
std::vector<LightBFF> lightData2;
std::vector<CameraBFF> cameraData2;

std::vector<std::vector<BlendShapesBFF>> blendShapeDataArr3;
std::vector<VertexAnimBFF> keyFrameData2;

//Make a file

int main(int argc, char** argv)
{
    //myFile.writeToFile("test line from main");

    BiReader myFile("../biFile.bff");
    myFile.printToConsole();
    
    
    FbxManager* lSdkManager = NULL;
    FbxScene* lScene = NULL;
    bool lResult;

    // Prepare the FBX SDK.
    InitializeSdkObjects(lSdkManager, lScene);
    // Load the scene.

    // The example can take a FBX file as an argument.
	FbxString lFilePath("");
	for( int i = 1, c = argc; i < c; ++i )
	{
		if( FbxString(argv[i]) == "-test" ) gVerbose = false;
		else if( lFilePath.IsEmpty() ) lFilePath = argv[i];
	}

	if( lFilePath.IsEmpty() )
	{

        lFilePath = "../testTriangle(Tristan).fbx";
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		lResult = LoadScene(lSdkManager, lScene, lFilePath.Buffer());
	}

    if(lResult == false)
    {
        FBXSDK_printf("\n\nAn error occurred while loading the scene...");
    }
    else 
    {
        // Display the scene.
        DisplayMetaData(lScene);

        FBXSDK_printf("\n\n---------------------\nGlobal Light Settings\n---------------------\n\n");

        if( gVerbose ) DisplayGlobalLightSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n----------------------\nGlobal Camera Settings\n----------------------\n\n");

        if( gVerbose ) DisplayGlobalCameraSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n--------------------\nGlobal Time Settings\n--------------------\n\n");

        if( gVerbose ) DisplayGlobalTimeSettings(&lScene->GetGlobalSettings());

        FBXSDK_printf("\n\n---------\nHierarchy\n---------\n\n");

        if( gVerbose ) DisplayHierarchy(lScene);

        FBXSDK_printf("\n\n------------\nNode Content\n------------\n\n");

        if( gVerbose ) DisplayContent(lScene);

        FBXSDK_printf("\n\n----\nPose\n----\n\n");

        if( gVerbose ) DisplayPose(lScene);

        FBXSDK_printf("\n\n---------\nAnimation\n---------\n\n");

        if( gVerbose ) DisplayAnimation(lScene);

        //now display generic information

        FBXSDK_printf("\n\n---------\nGeneric Information\n---------\n\n");
        if( gVerbose ) DisplayGenericInfo(lScene);
    }



    // Destroy all objects created by the FBX SDK.
    DestroySdkObjects(lSdkManager, lResult);

    // ****************** Mesh ****************** //
    meshData2 = GetMeshData();
    myStringFile5.writeToStringFile(
        "Mesh Name: " + (std::string)meshData2.name +
        "\n" + 
        "NrOfVertex: " + std::to_string(meshData2.nrOfVertex));

    myFile5.writeToFile((const char*)&meshData2, sizeof(MeshBFF)); //Add to biFile

    // ****************** Vertex ****************** //
    vertexData2 = GetVertxData();
    for (int i = 0; i < meshData2.nrOfVertex; i++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(i) + ")\n\n" +
            "PosX: " + std::to_string(vertexData2[i].pos[0]) + "\n" +
            "PosY: " + std::to_string(vertexData2[i].pos[1]) + "\n" +
            "PosZ: " + std::to_string(vertexData2[i].pos[2]) + "\n" +
            "\n" +
            "U: " + std::to_string(vertexData2[i].uv[0]) + "\n" +
            "V: " + std::to_string(vertexData2[i].uv[1]) + "\n" +
            "\n" +
            "NormX: " + std::to_string(vertexData2[i].norm[0]) + "\n" +
            "NormY: " + std::to_string(vertexData2[i].norm[1]) + "\n" +
            "NormZ: " + std::to_string(vertexData2[i].norm[2]) + "\n" +
            "\n" +
            "biNormX: " + std::to_string(vertexData2[i].biNorm[0]) + "\n" +
            "biNormY: " + std::to_string(vertexData2[i].biNorm[1]) + "\n" +
            "biNormZ: " + std::to_string(vertexData2[i].biNorm[2]) + "\n" +
            "\n" +
            "TanX: " + std::to_string(vertexData2[i].tan[0]) + "\n" +
            "TanY: " + std::to_string(vertexData2[i].tan[1]) + "\n" +
            "TanZ: " + std::to_string(vertexData2[i].tan[2]) + "\n" + "\n" + "\n");

        myFile5.writeToFile((const char*)&vertexData2[i], sizeof(VertexBFF)); //Add to biFile
    }

    // ****************** Material ****************** //
    materialData3 = GetMaterialData2();
    myStringFile5.writeToStringFile(
        "\n------------- Material: \n\n"
        "DiffuseR: " + std::to_string(materialData3.Diffuse[0]) + "\n" +
        "DiffuseG: " + std::to_string(materialData3.Diffuse[1]) + "\n" +
        "DiffuseB: " + std::to_string(materialData3.Diffuse[2]) + "\n" +
        "\n" +
        "AmbientR: " + std::to_string(materialData3.Ambient[0]) + "\n" +
        "AmbientG: " + std::to_string(materialData3.Ambient[1]) + "\n" +
        "AmbientB: " + std::to_string(materialData3.Ambient[2]) + "\n" +
        "\n" +
        "Opacity: " + std::to_string(materialData3.Opacity) + "\n ");

    myFile5.writeToFile((const char*)&materialData3, sizeof(MaterialBFF)); //Add to biFile

    // ****************** Light ****************** //
    lightData2 = GetLightData();
    myStringFile5.writeToStringFile("\n\n\n------------- Light\n\n");

    for (int i = 0; i < getNrOfLights(); i++)
    {
        myStringFile5.writeToStringFile(
            "Type: " + (std::string)lightData2[i].type + "\n" +
            "\n" +
            "ColorR: " + std::to_string(lightData2[i].color[0]) + "\n" +
            "ColorG: " + std::to_string(lightData2[i].color[1]) + "\n" +
            "ColorB: " + std::to_string(lightData2[i].color[2]) + "\n" +
            "\n" +
            "Dir: " + std::to_string(lightData2[i].dir) + "\n" +
            "\n" +
            "Intencity: " + std::to_string(lightData2[i].intencity) + "\n\n");

        myFile5.writeToFile((const char*)&lightData2[i], sizeof(LightBFF)); //Add to biFile
    }

    // ****************** Camera ****************** //
    cameraData2 = GetCameraData();
    myStringFile5.writeToStringFile("\n\n\n------------- Camera:\n\n");
    
    for (int i = 0; i < getNrOfCameras(); i++)
    {

        myStringFile5.writeToStringFile(
            "PosX: " + std::to_string(cameraData2[i].pos[0]) + "\n" +
            "PosY: " + std::to_string(cameraData2[i].pos[1]) + "\n" +
            "PosZ: " + std::to_string(cameraData2[i].pos[2]) + "\n" +
            "\n" +
            "upVecX: " + std::to_string(cameraData2[i].upVec[0]) + "\n" +
            "upVecY: " + std::to_string(cameraData2[i].upVec[1]) + "\n" +
            "upVecZ: " + std::to_string(cameraData2[i].upVec[2]) + "\n" +
            "\n" +
            "forwardVecX: " + std::to_string(cameraData2[i].forwardVec[0]) + "\n" +
            "forwardVecY: " + std::to_string(cameraData2[i].forwardVec[1]) + "\n" +
            "forwardVecZ: " + std::to_string(cameraData2[i].forwardVec[2]) + "\n" +
            "\n" +
            "nearPlane: " + std::to_string(cameraData2[i].nearPlane) + "\n" +
            "\n" +
            "farPlane: " + std::to_string(cameraData2[i].farPlane) + "\n" +
            "\n" +
            "FOV: " + std::to_string(cameraData2[i].FOV) + " (degrees)" + "\n" + "\n" + "\n");
        myFile5.writeToFile((const char*)&cameraData2, sizeof(CameraBFF)); //Add to biFile

    }

    // ****************** Shapes ****************** //
    blendShapeDataArr3 = GetBlendShapeDataArr2();
    for (int i = 0; i < GetNrOfBlendShapes2(); i++)
    {
        myStringFile5.writeToStringFile("\n\n\n------------- BlendShape:\n\n");
        for (int v = 0; v < GetNrOfVertexInBlendShape2(); v++)
        {
            myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(v) + ")\n\n" +
            "PosX: " + std::to_string(blendShapeDataArr3[i][v].pos[0]) +
            "\n" +
            "PosY: " + std::to_string(blendShapeDataArr3[i][v].pos[1]) +
            "\n" +
            "PosZ: " + std::to_string(blendShapeDataArr3[i][v].pos[2]) +
            "\n" +
            "NormX: " + std::to_string(blendShapeDataArr3[i][v].norm[0]) +
            "\n" +
            "NormY: " + std::to_string(blendShapeDataArr3[i][v].norm[1]) +
            "\n" +
            "NormZ: " + std::to_string(blendShapeDataArr3[i][v].norm[2]) +
            "\n\n");
        }
    }
    // ****************** Vertex Animation ****************** //
    keyFrameData2 = getKeyFrameData();
    myStringFile5.writeToStringFile("\n\n\n-------------  Vertex Animation:\n\n");

    for (int i = 0; i < getNrOfkeyframes(); i++)
    {
        myStringFile5.writeToStringFile(
            "Frame: " + std::to_string(keyFrameData2[i].time) +
            "\n" +
            "PosX: " + std::to_string(keyFrameData2[i].pos[0]) +
            "\n" +
            "PosY: " + std::to_string(keyFrameData2[i].pos[1]) +
            "\n" +
            "PosZ: " + std::to_string(keyFrameData2[i].pos[2]) +
            "\n" +
            "RotX: " + std::to_string(keyFrameData2[i].rot[0]) +
            "\n" +
            "RotY: " + std::to_string(keyFrameData2[i].rot[1]) +
            "\n" +
            "RotZ: " + std::to_string(keyFrameData2[i].rot[2]) +
            "\n" +
            "ScaleX: " + std::to_string(keyFrameData2[i].scale[0]) +
            "\n" +
            "ScaleY: " + std::to_string(keyFrameData2[i].scale[1]) +
            "\n" +
            "ScaleZ: " + std::to_string(keyFrameData2[i].scale[2]) +
            "\n\n");

        myFile5.writeToFile((const char*)&keyFrameData2, sizeof(VertexAnimBFF)); // Add to biFile
    }


    return 0;
}


void DisplayContent(FbxScene* pScene)
{
    int i;
    FbxNode* lNode = pScene->GetRootNode();

    if(lNode)
    {
        for(i = 0; i < lNode->GetChildCount(); i++)
        {
            DisplayContent(lNode->GetChild(i));
        }
    }
}

void DisplayContent(FbxNode* pNode)
{
    FbxNodeAttribute::EType lAttributeType;
    int i;

    if(pNode->GetNodeAttribute() == NULL)
    {
        FBXSDK_printf("NULL Node Attribute\n\n");
    }
    else
    {
        lAttributeType = (pNode->GetNodeAttribute()->GetAttributeType());

        switch (lAttributeType)
        {
	    default:
	        break;
        case FbxNodeAttribute::eMarker:  
            DisplayMarker(pNode);
            break;

        case FbxNodeAttribute::eSkeleton:  
            DisplaySkeleton(pNode);
            break;

        case FbxNodeAttribute::eMesh:      
            DisplayMesh(pNode);
            break;

        case FbxNodeAttribute::eNurbs:      
            DisplayNurb(pNode);
            break;

        case FbxNodeAttribute::ePatch:     
            DisplayPatch(pNode);
            break;

        case FbxNodeAttribute::eCamera:    
            DisplayCamera(pNode);
            break;

        case FbxNodeAttribute::eLight:     
            DisplayLight(pNode);
            break;

        case FbxNodeAttribute::eLODGroup:
            DisplayLodGroup(pNode);
            break;
        }

    }

    DisplayUserProperties(pNode);
    DisplayTarget(pNode);
    DisplayPivotsAndLimits(pNode);
    DisplayTransformPropagation(pNode);
    DisplayGeometricTransform(pNode);

    for(i = 0; i < pNode->GetChildCount(); i++)
    {
        DisplayContent(pNode->GetChild(i));
    }

}


void DisplayTarget(FbxNode* pNode)
{
    if(pNode->GetTarget() != NULL)
    {
        DisplayString("    Target Name: ", (char *) pNode->GetTarget()->GetName());
    }
}

void DisplayTransformPropagation(FbxNode* pNode)
{
    FBXSDK_printf("    Transformation Propagation\n");

    // 
    // Rotation Space
    //
    EFbxRotationOrder lRotationOrder;
    pNode->GetRotationOrder(FbxNode::eSourcePivot, lRotationOrder);

    FBXSDK_printf("        Rotation Space: ");

    switch (lRotationOrder)
    {
    case eEulerXYZ: 
        FBXSDK_printf("Euler XYZ\n");
        break;
    case eEulerXZY:
        FBXSDK_printf("Euler XZY\n");
        break;
    case eEulerYZX:
        FBXSDK_printf("Euler YZX\n");
        break;
    case eEulerYXZ:
        FBXSDK_printf("Euler YXZ\n");
        break;
    case eEulerZXY:
        FBXSDK_printf("Euler ZXY\n");
        break;
    case eEulerZYX:
        FBXSDK_printf("Euler ZYX\n");
        break;
    case eSphericXYZ:
        FBXSDK_printf("Spheric XYZ\n");
        break;
    }

    //
    // Use the Rotation space only for the limits
    // (keep using eEulerXYZ for the rest)
    //
    FBXSDK_printf("        Use the Rotation Space for Limit specification only: %s\n",
        pNode->GetUseRotationSpaceForLimitOnly(FbxNode::eSourcePivot) ? "Yes" : "No");


    //
    // Inherit Type
    //
    FbxTransform::EInheritType lInheritType;
    pNode->GetTransformationInheritType(lInheritType);

    FBXSDK_printf("        Transformation Inheritance: ");

    switch (lInheritType)
    {
    case FbxTransform::eInheritRrSs:
        FBXSDK_printf("RrSs\n");
        break;
    case FbxTransform::eInheritRSrs:
        FBXSDK_printf("RSrs\n");
        break;
    case FbxTransform::eInheritRrs:
        FBXSDK_printf("Rrs\n");
        break;
    }
}

void DisplayGeometricTransform(FbxNode* pNode)
{
    FbxVector4 lTmpVector;

    FBXSDK_printf("    Geometric Transformations\n");

    //
    // Translation
    //
    lTmpVector = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Translation: %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Rotation
    //
    lTmpVector = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
    FBXSDK_printf("        Rotation:    %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);

    //
    // Scaling
    //
    lTmpVector = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
    FBXSDK_printf("        Scaling:     %f %f %f\n", lTmpVector[0], lTmpVector[1], lTmpVector[2]);
}


void DisplayMetaData(FbxScene* pScene)
{
    FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
    if (sceneInfo)
    {
        FBXSDK_printf("\n\n--------------------\nMeta-Data\n--------------------\n\n");
        FBXSDK_printf("    Title: %s\n", sceneInfo->mTitle.Buffer());
        FBXSDK_printf("    Subject: %s\n", sceneInfo->mSubject.Buffer());
        FBXSDK_printf("    Author: %s\n", sceneInfo->mAuthor.Buffer());
        FBXSDK_printf("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
        FBXSDK_printf("    Revision: %s\n", sceneInfo->mRevision.Buffer());
        FBXSDK_printf("    Comment: %s\n", sceneInfo->mComment.Buffer());

        FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
        if (thumbnail)
        {
            FBXSDK_printf("    Thumbnail:\n");

            switch (thumbnail->GetDataFormat())
            {
            case FbxThumbnail::eRGB_24:
                FBXSDK_printf("        Format: RGB\n");
                break;
            case FbxThumbnail::eRGBA_32:
                FBXSDK_printf("        Format: RGBA\n");
                break;
            }

            switch (thumbnail->GetSize())
            {
	        default:
	            break;
            case FbxThumbnail::eNotSet:
                FBXSDK_printf("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e64x64:
                FBXSDK_printf("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
                break;
            case FbxThumbnail::e128x128:
                FBXSDK_printf("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
            }
        }
    }
}

