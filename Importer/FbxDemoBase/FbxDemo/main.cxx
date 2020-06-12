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
#include <Windows.h>

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

// Outpus ---------------
SceneBFF sceneData;

MeshBFF meshData2;

std::vector<MaterialBFF> finalMaterialData;

std::vector<TextureBFF> finalTextureData;
std::string textureTempName;
std::string textureType;
std::string texturePath;

std::vector<ControlPointBFF> controlPointData2;

FbxAMatrix lightTransforms;

std::vector<LightBFF> lightData2;

std::vector<CameraBFF> cameraData2;

std::vector<std::vector<BlendshapeBFF>> newBlendShapeData;

std::vector<KeyFrameBFF> keyFrameData2;
std::vector<JointBFF> jointData2;



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


        //lFilePath = "../TriAnim.fbx";


        lFilePath = "../Plane1__Blend2__Joint5__Anim20__Texture2__Material2__Light3__Camera2.fbx";

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
    sceneData.nrOfTextures = GetNrOfTextures2();
    sceneData.nrOfLights = getNrOfLights();
    sceneData.nrOfCameras = getNrOfCameras();
    sceneData.nrOfVertexAnimFrames = getNrOfkeyframes();
    sceneData.nrOfBlendShapes = GetNrOfBlendShapes2();
    /*
    std::vector<std::string> names2;
    names2 = GetJointName();

    char temp[64];
    for (int i = 0; i < getNrOfJoints(); i++)
    {
        strcpy_s(temp, _countof(temp), names2[i].c_str());
        for (int j = 0; j < sizeof(temp); j++)
            //boneData2[i].name[j] = temp[j];
    }

    std::vector<FbxVector4> bindRots2;
    bindRots2 = GetBindRots();
    for (int i = 0; i < getNrOfJoints(); i++)
    {
        //boneData2[i].bindRot[0] = bindRots2[i + 1][0];
        //boneData2[i].bindRot[1] = bindRots2[i + 1][1];
        //boneData2[i].bindRot[2] = bindRots2[i + 1][2];
    }
    */

    // ****************** Scene ****************** //
    myStringFile5.writeToStringFile("############ Scene ############\n");
    myStringFile5.writeToStringFile(
        "NrOfTextures: " + std::to_string(sceneData.nrOfTextures) +
        "\n" +
        "NrOfLights: " + std::to_string(sceneData.nrOfLights) +
        "\n" +
        "NrOfCameras: " + std::to_string(sceneData.nrOfCameras) +
        "\n" +
        "NrOfFrames: " + std::to_string(sceneData.nrOfVertexAnimFrames) +
        "\n" +
        "NrOfBlendShapes: " + std::to_string(sceneData.nrOfBlendShapes) +
        "\n\n"
    );
    myFile5.writeToFile((const char*)&sceneData, sizeof(SceneBFF));

    // ****************** Mesh ****************** //
    meshData2 = GetMeshData();
    meshData2.nrOfMaterials = GetNrOfMaterials2();
    meshData2.nrJoints = getNrOfJoints();
    myStringFile5.writeToStringFile("\n############ Mesh ############\n");
    myStringFile5.writeToStringFile(
        "Mesh Name: " + (std::string)meshData2.name + "\n" +
        "NrOfControlPoints: " + std::to_string(meshData2.nrOfControlPoints) + "\n" +
        "NrOfMaterials: " + std::to_string(meshData2.nrOfMaterials) + "\n" +
        "NrOfJoints: " + std::to_string(meshData2.nrJoints) +
        "\n\n"
    );
    myFile5.writeToFile((const char*)&meshData2, sizeof(MeshBFF)); //Add to biFile

    // ****************** Material ****************** //
    finalMaterialData = GetMaterialData2();

    myStringFile5.writeToStringFile("\n############ Materials ############\n");
    for (int m = 0; m < GetNrOfMaterials2(); m++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(m) + ")\n" +
            "DiffuseR: " + std::to_string(finalMaterialData[m].Diffuse[0]) + "\n" +
            "DiffuseG: " + std::to_string(finalMaterialData[m].Diffuse[1]) + "\n" +
            "DiffuseB: " + std::to_string(finalMaterialData[m].Diffuse[2]) + "\n" +
            "\n" +
            "AmbientR: " + std::to_string(finalMaterialData[m].Ambient[0]) + "\n" +
            "AmbientG: " + std::to_string(finalMaterialData[m].Ambient[1]) + "\n" +
            "AmbientB: " + std::to_string(finalMaterialData[m].Ambient[2]) + "\n" +
            "\n" +
            "Opacity: " + std::to_string(finalMaterialData[m].Opacity) + "\n " +
            "\n\n"
        );

        myFile5.writeToFile((const char*)&finalMaterialData[m], sizeof(MaterialBFF)); //Add to biFile
    }

    // ****************** Texture ****************** //
    finalTextureData.resize(GetNrOfTextures2());
    for (int t = 0; t < GetNrOfTextures2(); t++)
    {
        //--- copy file  ---
        std::string fullPath = GETNAME(t);
        int index = fullPath.find_last_of("/");

        //std::string start = fullPath.substr(0, index);
        std::string end = fullPath.substr(index, fullPath.size() - 1);
        int stop = 0;
        std::string destination = ".." + end;
        CopyFile(GETNAME(t).c_str(), destination.c_str(), FALSE);
        

        //--- Assign values ---
        //texturePath = GETNAME(t);
        textureType = GetTextureType2(t);

        strcpy_s(finalTextureData[t].texturePath, TextureBFF::PATH_SIZE, destination.c_str());
        strcpy_s(finalTextureData[t].textureType, TextureBFF::PATH_SIZE, textureType.c_str());

        finalTextureData[t].parentMaterialIndex = GetMaterialIndex2(t);
    }

    myStringFile5.writeToStringFile("\n############ Textures ############\n");
    for (int t = 0; t < GetNrOfTextures2(); t++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(t) + ")\n" +
            "Texture belongs to Material : " + std::to_string(finalTextureData[t].parentMaterialIndex) + "\n" +
            "TextureType: " + (std::string)finalTextureData[t].textureType + "\n" +
            "TexturePath: " + (std::string)finalTextureData[t].texturePath + "\n" +
            "\n\n"
        );

        myFile5.writeToFile((const char*)&finalTextureData[t], sizeof(TextureBFF)); //Add to biFile
    }

    // ****************** Control Point ****************** //
    controlPointData2 = GetControlPointData();
    std::vector<ControlPointBFF> tempControlJointData;
    tempControlJointData = GetControlPointJointData2();
    for (int c = 0; c < meshData2.nrOfControlPoints; c++)
    {
        controlPointData2[c].influencedByJoints[0] = tempControlJointData[c].influencedByJoints[0];
        controlPointData2[c].influencedByJoints[1] = tempControlJointData[c].influencedByJoints[1];
        controlPointData2[c].influencedByJoints[2] = tempControlJointData[c].influencedByJoints[2];
        controlPointData2[c].influencedByJoints[3] = tempControlJointData[c].influencedByJoints[3];

        controlPointData2[c].boneWeight[0] = tempControlJointData[c].boneWeight[0];
        controlPointData2[c].boneWeight[1] = tempControlJointData[c].boneWeight[1];
        controlPointData2[c].boneWeight[2] = tempControlJointData[c].boneWeight[2];
        controlPointData2[c].boneWeight[3] = tempControlJointData[c].boneWeight[3];
    }
    
    myStringFile5.writeToStringFile("\n############ Control Points ############\n");
    for (int i = 0; i < meshData2.nrOfControlPoints; i++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(i) + ")\n" +
            "Position (" + std::to_string(controlPointData2[i].pos[0]) + ", " + std::to_string(controlPointData2[i].pos[1]) + ", " + std::to_string(controlPointData2[i].pos[2]) + ")\n" +
            "UV (" + std::to_string(controlPointData2[i].uv[0]) + ", " + std::to_string(controlPointData2[i].uv[1]) + ")\n" +
            "\n" +
            "Normal (" + std::to_string(controlPointData2[i].norm[0]) + ", " + std::to_string(controlPointData2[i].norm[1]) + ", " + std::to_string(controlPointData2[i].norm[2]) + ")\n" +
            "biNormal (" + std::to_string(controlPointData2[i].biNorm[0]) + ", " + std::to_string(controlPointData2[i].biNorm[1]) + ", " + std::to_string(controlPointData2[i].biNorm[2]) + ")\n" +
            "Tangent (" + std::to_string(controlPointData2[i].tan[0]) + ", " + std::to_string(controlPointData2[i].tan[1]) + ", " + std::to_string(controlPointData2[i].tan[2]) + ")\n" +
            "\n"
            "Influenced by joint:" + std::to_string(controlPointData2[i].influencedByJoints[0]) + " (" + std::to_string(controlPointData2[i].boneWeight[0]) + "%)\n" +
            "Influenced by joint:" + std::to_string(controlPointData2[i].influencedByJoints[1]) + " (" + std::to_string(controlPointData2[i].boneWeight[1]) + "%)\n" +
            "Influenced by joint:" + std::to_string(controlPointData2[i].influencedByJoints[2]) + " (" + std::to_string(controlPointData2[i].boneWeight[2]) + "%)\n" +
            "Influenced by joint:" + std::to_string(controlPointData2[i].influencedByJoints[3]) + " (" + std::to_string(controlPointData2[i].boneWeight[3]) + "%)\n" +
            "\n\n"
        );

        myFile5.writeToFile((const char*)&controlPointData2[i], sizeof(ControlPointBFF)); //Add to biFile
    }

    // ****************** Light ****************** //
    lightData2 = GetLightData();
    for (int i = 0; i < getNrOfLights(); i++)
    {
        lightTransforms = getLightTransformMatrix(i);

        FbxVector4 lightTranslation2 = lightTransforms.GetT();
        FbxVector4 lightRotation = lightTransforms.GetR();
        FbxVector4 lightScaling = lightTransforms.GetS();


        lightData2[i].translation[0] = lightTranslation2[0];
        lightData2[i].translation[1] = lightTranslation2[1];
        lightData2[i].translation[2] = lightTranslation2[2];

        lightData2[i].rotation[0] = lightRotation[0];
        lightData2[i].rotation[1] = lightRotation[1];
        lightData2[i].rotation[2] = lightRotation[2];

        lightData2[i].scale[0] = lightScaling[0];
        lightData2[i].scale[1] = lightScaling[1];
        lightData2[i].scale[2] = lightScaling[2];
    }

    myStringFile5.writeToStringFile("\n############ Lights ############\n");
    for (int i = 0; i < getNrOfLights(); i++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(i) + ")\n" +
            "Type: " + (std::string)lightData2[i].type + "\n" +
            "\n" +
            "ColorR: " + std::to_string(lightData2[i].color[0]) + "\n" +
            "ColorG: " + std::to_string(lightData2[i].color[1]) + "\n" +
            "ColorB: " + std::to_string(lightData2[i].color[2]) + "\n" +
            "\n" +
            "Dir: " + std::to_string(lightData2[i].dir) + "\n" +
            "Intencity: " + std::to_string(lightData2[i].intencity) + "\n" +
            "\n" +
            "Translation: " + std::to_string(lightData2[i].translation[0]) + ", " + std::to_string(lightData2[i].translation[1]) + ", " + std::to_string(lightData2[i].translation[2]) + "\n" +
            "Rotation: " + std::to_string(lightData2[i].rotation[0]) + ", " + std::to_string(lightData2[i].rotation[1]) + ", " + std::to_string(lightData2[i].rotation[2]) + "\n" +
            "Scale: " + std::to_string(lightData2[i].scale[0]) + ", " + std::to_string(lightData2[i].scale[1]) + ", " + std::to_string(lightData2[i].scale[2]) + "\n" + 
            "\n\n"
        );

        myFile5.writeToFile((const char*)&lightData2[i], sizeof(LightBFF)); //Add to biFile
    }

    // ****************** Camera ****************** //
    cameraData2 = GetCameraData();
    
    myStringFile5.writeToStringFile("\n############ Camera ############\n");
    for (int i = 0; i < getNrOfCameras(); i++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(i) + ")\n" +
            "Position (" + std::to_string(cameraData2[i].pos[0]) + ", " + std::to_string(cameraData2[i].pos[1]) + ", "  + std::to_string(cameraData2[i].pos[2]) + ")\n" +
            "\n" +
            "UpVector (" + std::to_string(cameraData2[i].upVec[0]) + ", " + std::to_string(cameraData2[i].upVec[1]) + ", " + std::to_string(cameraData2[i].upVec[2]) + ")\n" +
            "\n" +
            "ForwardVector (" + std::to_string(cameraData2[i].forwardVec[0]) + ", " + std::to_string(cameraData2[i].forwardVec[1]) + ", " + std::to_string(cameraData2[i].forwardVec[2]) + ")\n" +
            "\n" +
            "NearPlane: " + std::to_string(cameraData2[i].nearPlane) + "\n" +
            "\n" +
            "FarPlane: " + std::to_string(cameraData2[i].farPlane) + "\n" +
            "\n" +
            "FOV: " + std::to_string(cameraData2[i].FOV) + " (degrees)" + "\n" + 
            "\n" + "\n"
        );

        myFile5.writeToFile((const char*)&cameraData2, sizeof(CameraBFF)); //Add to biFile
    }
    
    // ****************** Blendshape ****************** //
    newBlendShapeData = GetNewBlendShapeData2();
    for (int i = 0; i < GetNrOfBlendShapes2(); i++)
    {
        myStringFile5.writeToStringFile("\n\n\n############ BlendShape ############\n");
        for (int v = 0; v < GetNrOfVertexInBlendShape2(); v++)
        {
            myStringFile5.writeToStringFile(
                "\n------ Index " + std::to_string(v) + ")\n" +
                "Position (" + std::to_string(newBlendShapeData[i][v].pos[0]) + ", " + std::to_string(newBlendShapeData[i][v].pos[1]) + ", " + std::to_string(newBlendShapeData[i][v].pos[2]) + ")\n" +
                "Normals (" + std::to_string(newBlendShapeData[i][v].norm[0]) + ", " + std::to_string(newBlendShapeData[i][v].norm[1]) + ", " + std::to_string(newBlendShapeData[i][v].norm[2]) + ")\n" +
                "\n\n"
            );

            myFile5.writeToFile((const char*)&newBlendShapeData[i][v], sizeof(BlendshapeBFF)); //Add to biFile
        }
    }

    // ****************** Joints ****************** //
    jointData2 = GetJointData();
    std::vector<JointBFF> tempJointKeyFrameData;
    tempJointKeyFrameData = GetJointKeyFrameData();
    for (int j = 0; j < getNrOfJoints(); j++)
    {
        jointData2[j].nrOfKeyFrames = tempJointKeyFrameData[j].nrOfKeyFrames;
        jointData2[j].animationFrames.resize(jointData2[j].nrOfKeyFrames);

        for (int k = 0; k < jointData2[j].nrOfKeyFrames; k++)
        {
            jointData2[j].animationFrames[k].timestamp = tempJointKeyFrameData[j].animationFrames[k].timestamp;

            for (int v = 0; v < 9; v++)
            {
                jointData2[j].animationFrames[k].pose[v] = tempJointKeyFrameData[j].animationFrames[k].pose[v];
            }
        }
    }

    myStringFile5.writeToStringFile("\n############ Joints ############\n");
    for (int j = 0; j < getNrOfJoints(); j++)
    {
        myStringFile5.writeToStringFile(
            "\n------ Index " + std::to_string(j) + ")\n" +
            "Joint Index: " + std::to_string(jointData2[j].jointIndex) + "\n" +
            "Bindpose matrix:\n" +
            "(" + std::to_string(jointData2[j].bindPoseMatrix[0]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[1]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[2]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[3]) + ")\n" +
            "(" + std::to_string(jointData2[j].bindPoseMatrix[4]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[5]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[6]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[7]) + ")\n" +
            "(" + std::to_string(jointData2[j].bindPoseMatrix[8]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[9]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[10]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[11]) + ")\n" +
            "(" + std::to_string(jointData2[j].bindPoseMatrix[12]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[13]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[14]) + ", " + std::to_string(jointData2[j].bindPoseMatrix[15]) + ")\n" +
            "\n"
            "Keyframes: " + std::to_string(jointData2[j].nrOfKeyFrames) + "\n"
        );

        for (int k = 0; k < jointData2[j].nrOfKeyFrames; k++)
        {
            myStringFile5.writeToStringFile(
                "Time: " + std::to_string(jointData2[j].animationFrames[k].timestamp) + "\n" +
                "(" + std::to_string(jointData2[j].animationFrames[k].pose[0]) + ", " + std::to_string(jointData2[j].animationFrames[k].pose[1]) + ", " + std::to_string(jointData2[j].animationFrames[k].pose[2]) + ")\n" +
                "(" + std::to_string(jointData2[j].animationFrames[k].pose[3]) + ", " + std::to_string(jointData2[j].animationFrames[k].pose[4]) + ", " + std::to_string(jointData2[j].animationFrames[k].pose[5]) + ")\n" +
                "(" + std::to_string(jointData2[j].animationFrames[k].pose[6]) + ", " + std::to_string(jointData2[j].animationFrames[k].pose[7]) + ", " + std::to_string(jointData2[j].animationFrames[k].pose[8]) + ")\n\n"
            );
        }
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
            //blendShapeDataArr3 = GetBlendShapeDataArr2(pNode);
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

