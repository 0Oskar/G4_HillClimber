/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplayCamera.h"
//#include "DisplayCamera.h"
//#include "DisplayCamera.h"

void DisplayCamera(FbxCamera* pCamera, char* pName, FbxNode* pTargetNode, FbxNode* pTargetUpNode);
void DisplayDefaultAnimationValues(FbxCamera* pCamera);
void DisplayRenderOptions(FbxCamera* pCamera);
void DisplayCameraViewOptions(FbxCamera* pCamera);
void DisplayBackgroundProperties(FbxCamera* pCamera);
void DisplayApertureAndFilmControls(FbxCamera* pCamera);
void DisplayViewingAreaControls(FbxCamera* pCamera);
void DisplayCameraPositionAndOrientation(FbxCamera* pCamera, FbxNode* pTargetNode, FbxNode* pUpTargetNode);

FileWrite myFile2("../biFile.bff");
FileWrite myStringFile2("../stringFile.bff");

std::vector<CameraBFF> cameraData;

int nrOfCameras = 0;


void DisplayCamera(FbxNode* pNode)
{
    DisplayCamera((FbxCamera*) pNode->GetNodeAttribute(), (char *) pNode->GetName(), pNode->GetTarget(), pNode->GetTargetUp());
}

void DisplayCamera(FbxCamera* pCamera, char* pName, FbxNode* pTargetNode, FbxNode* pTargetUpNode)
{
    //myStringFile2.writeToStringFile("\n\n\n------------- Camera:\n\n");


    DisplayString("Camera Name: ", pName);
    if (!pCamera)
    {
        DisplayString("NOT FOUND");
        return;
    }
    DisplayMetaDataConnections(pCamera);

    DisplayCameraPositionAndOrientation(pCamera, pTargetNode, pTargetUpNode);

    const char* lProjectionTypes[] = { "Perspective", "Orthogonal" };

    DisplayString("    Projection Type: ", lProjectionTypes[pCamera->ProjectionType.Get()]);

    DisplayViewingAreaControls(pCamera);

    // If camera projection type is set to FbxCamera::eOrthogonal, the 
    // aperture and film controls are not relevant.
    if (pCamera->ProjectionType.Get() != FbxCamera::eOrthogonal)
    {
        DisplayApertureAndFilmControls(pCamera);
    }

    DisplayBackgroundProperties(pCamera);
    DisplayCameraViewOptions(pCamera);
    DisplayRenderOptions(pCamera);
    DisplayDefaultAnimationValues(pCamera);

}


void DisplayCameraPositionAndOrientation(FbxCamera* pCamera, FbxNode* pTargetNode, FbxNode* pTargetUpNode)
{
    DisplayString("    Camera Position and Orientation");
    Display3DVector("        Position: ", pCamera->Position.Get());
    cameraData.push_back(CameraBFF());
    cameraData.back().pos[0] = float(pCamera->Position.Get()[0]);
    cameraData.back().pos[1] = float(pCamera->Position.Get()[1]);
    cameraData.back().pos[2] = float(pCamera->Position.Get()[2]);

    if (pTargetNode)
    {
        DisplayString("        Camera Interest: ",(char *) pTargetNode->GetName());
    }
    else
    {
        Display3DVector("        Default Camera Interest Position: ", pCamera->InterestPosition.Get());
        FbxVector4 tempPos = pCamera->Position.Get();
        FbxVector4 tempLoc = pCamera->InterestPosition.Get();
        FbxVector4 temp = tempPos - tempLoc;
        temp[3] = 0;
        temp.Normalize();
        cameraData.back().forwardVec[0] = temp[0];
        cameraData.back().forwardVec[1] = temp[1];
        cameraData.back().forwardVec[2] = temp[2];
    }


    if (pTargetUpNode)
    {
        DisplayString("        Camera Up Target: ", (char *) pTargetUpNode->GetName());
    }
    else
    {
        Display3DVector("        Up Vector: ", pCamera->UpVector.Get());
        cameraData.back().upVec[0] = float(pCamera->UpVector.Get()[0]);
        cameraData.back().upVec[1] = float(pCamera->UpVector.Get()[1]);
        cameraData.back().upVec[2] = float(pCamera->UpVector.Get()[2]);
    }

    DisplayDouble("        Roll: ", pCamera->Roll.Get());

    nrOfCameras++;
}

std::vector<CameraBFF> GetCameraData()
{
    return cameraData;
}

int getNrOfCameras()
{
    return cameraData.size();
    //return nrOfCameras;
}


void DisplayViewingAreaControls(FbxCamera* pCamera)
{
    DisplayString("    Viewing Area Controls");

    const char* lCameraFormat[] = { "Custom", "D1 NTSC", "NTSC", "PAL", "D1 PAL", 
        "HD", "640x480", "320x200", "320x240", "128x128",
        "Full Screen"  };

    DisplayString("        Format: ", lCameraFormat[pCamera->GetFormat()]);

    const char* lAspectRatioModes[] = { "Window Size", "Fixed Ratio", "Fixed Resolution", 
        "Fixed Width", "Fixed Height" };

    DisplayString("        Aspect Ratio Mode: ", lAspectRatioModes[pCamera->GetAspectRatioMode()]);

    // If the ratio mode is eWINDOW_SIZE, both width and height values aren't relevant.
    if (pCamera->GetAspectRatioMode() != FbxCamera::eWindowSize)
    {
        DisplayDouble("        Aspect Width: ", pCamera->AspectWidth.Get());
        DisplayDouble("        Aspect Height: ", pCamera->AspectHeight.Get());
    }

    DisplayDouble("        Pixel Ratio: ", pCamera->PixelAspectRatio.Get());
    DisplayDouble("        Near Plane: ", pCamera->NearPlane.Get());
    cameraData.back().nearPlane = float(pCamera->NearPlane.Get());
    DisplayDouble("        Far Plane: ", pCamera->FarPlane.Get());
    cameraData.back().farPlane = float(pCamera->FarPlane.Get());
    DisplayBool("        Mouse Lock: ", pCamera->LockMode.Get());
}


void DisplayApertureAndFilmControls(FbxCamera* pCamera)
{
    DisplayString("    Aperture and Film Controls");

    const char* lCameraApertureFormats[] = { "Custom", 
        "16mm Theatrical",
        "Super 16mm",
        "35mm Academy",
        "35mm TV Projection",
        "35mm Full Aperture",
        "35mm 1.85 Projection",
        "35mm Anamorphic",
        "70mm Projection",
        "VistaVision",
        "Dynavision",
        "Imax" };

    DisplayString("        Aperture Format: ", lCameraApertureFormats[pCamera->GetApertureFormat()]);

    const char* lCameraApertureModes[] = { "Horizontal and Vertical", "Horizontal", "Vertical", "Focal Length" };

    DisplayString("        Aperture Mode: ", lCameraApertureModes[pCamera->GetApertureMode()]);

    DisplayDouble("        Aperture Width: ", pCamera->GetApertureWidth(), " inches");
    DisplayDouble("        Aperture Height: ", pCamera->GetApertureHeight(), " inches");
    DisplayDouble("        Squeeze Ratio: ", pCamera->GetSqueezeRatio());
    DisplayDouble("        Focal Length: ", pCamera->FocalLength.Get(), "mm");
    DisplayDouble("        Field of View: ", pCamera->FieldOfView.Get(), " degrees");
    cameraData.back().FOV = float(pCamera->FieldOfView.Get());
}


void DisplayBackgroundProperties(FbxCamera* pCamera)
{
    DisplayString("    Background Properties");

    DisplayString("        Background File Name: \"", (char *) pCamera->GetBackgroundFileName(), "\"");

    const char* lBackgroundDisplayModes[] = { "Disabled", "Always", "When Media" };

    DisplayString("        Background Display Mode: ", lBackgroundDisplayModes[pCamera->ViewFrustumBackPlaneMode.Get()]);

    DisplayBool("        Foreground Matte Threshold Enable: ", pCamera->ShowFrontplate.Get());

    // This option is only relevant if background drawing mode is set to eFOREGROUND or eBACKGROUND_AND_FOREGROUND.
    if (pCamera->ForegroundOpacity.Get())
    {
        DisplayDouble("        Foreground Matte Threshold: ", pCamera->BackgroundAlphaTreshold.Get());
    }

    FbxString lBackgroundPlacementOptions;

    if (pCamera->GetBackPlateFitImage())
    {
        lBackgroundPlacementOptions += " Fit,";
    }
    if (pCamera->GetBackPlateCenter())
    {
        lBackgroundPlacementOptions += " Center,";
    }
    if (pCamera->GetBackPlateKeepRatio())
    {
        lBackgroundPlacementOptions += " Keep Ratio,";
    }
    if (pCamera->GetBackPlateCrop())
    {
        lBackgroundPlacementOptions += " Crop,";
    }
    if (!lBackgroundPlacementOptions.IsEmpty())
    {
        FbxString lString =  lBackgroundPlacementOptions.Left(lBackgroundPlacementOptions.GetLen() - 1);
        DisplayString("        Background Placement Options: ",lString.Buffer());
    }

    DisplayDouble("        Background Distance: ", pCamera->BackPlaneDistance.Get());

    const char* lCameraBackgroundDistanceModes[] = { "Relative to Interest", "Absolute from Camera" };

    DisplayString("        Background Distance Mode: ", lCameraBackgroundDistanceModes[pCamera->BackPlaneDistanceMode.Get()]);
}


void DisplayCameraViewOptions(FbxCamera* pCamera)
{
    DisplayString("    Camera View Options");

    DisplayBool("        View Camera Interest: ", pCamera->ViewCameraToLookAt.Get());
    DisplayBool("        View Near Far Planes: ", pCamera->ViewFrustumNearFarPlane.Get());
    DisplayBool("        Show Grid: ", pCamera->ShowGrid.Get());
    DisplayBool("        Show Axis: ", pCamera->ShowAzimut.Get());
    DisplayBool("        Show Name: ", pCamera->ShowName.Get());
    DisplayBool("        Show Info on Moving: ", pCamera->ShowInfoOnMoving.Get());
    DisplayBool("        Show Time Code: ", pCamera->ShowTimeCode.Get());
    DisplayBool("        Display Safe Area: ", pCamera->DisplaySafeArea.Get());

    const char* lSafeAreaStyles[] = { "Round", "Square" };
    FbxColor color;
    FbxDouble3 c;

    DisplayString("        Safe Area Style: ", lSafeAreaStyles[pCamera->SafeAreaDisplayStyle.Get()]);
    DisplayBool("        Show Audio: ", pCamera->ShowAudio.Get());

    c = pCamera->BackgroundColor.Get();
    color = FbxColor(c[0], c[1], c[2]);
    DisplayColor("        Background Color: ", color);

    c = pCamera->AudioColor.Get();
    color = FbxColor(c[0], c[1], c[2]);
    DisplayColor("        Audio Color: ", color);

    DisplayBool("        Use Frame Color: ", pCamera->UseFrameColor.Get());

    c = pCamera->FrameColor.Get();
    color = FbxColor(c[0], c[1], c[2]);
    DisplayColor("        Frame Color: ", color);
}


void DisplayRenderOptions(FbxCamera* pCamera)
{
    DisplayString("    Render Options");

    const char* lCameraRenderOptionsUsageTimes[] = { "Interactive", "At Render" };

    DisplayString("        Render Options Usage Time: ", lCameraRenderOptionsUsageTimes[pCamera->UseRealTimeDOFAndAA.Get()]);
    DisplayBool("        Use Antialiasing: ", pCamera->UseAntialiasing.Get());
    DisplayDouble("        Antialiasing Intensity: ", pCamera->AntialiasingIntensity.Get());

    const char* lCameraAntialiasingMethods[] = { "Oversampling Antialiasing", "Hardware Antialiasing" };

    DisplayString("        Antialiasing Method: ", lCameraAntialiasingMethods[pCamera->AntialiasingMethod.Get()]);

    // This option is only relevant if antialiasing method is set to eOVERSAMPLING_ANTIALIASING.
    if (pCamera->AntialiasingMethod.Get() == FbxCamera::eAAOversampling)
    {
        DisplayInt("        Number of Samples: ", pCamera->FrameSamplingCount.Get());
    }

    const char* lCameraSamplingTypes[] = { "Uniform", "Stochastic" };

    DisplayString("        Sampling Type: ", lCameraSamplingTypes[pCamera->FrameSamplingType.Get()]);
    DisplayBool("        Use Accumulation Buffer: ", pCamera->UseAccumulationBuffer.Get());
    DisplayBool("        Use Depth of Field: ", pCamera->UseDepthOfField.Get());

    const char* lCameraFocusDistanceSources[] = { "Camera Interest", "Specific Distance" };

    DisplayString("        Focus Distance Source: ", lCameraFocusDistanceSources[pCamera->FocusSource.Get()]);

    // This parameter is only relevant if focus distance source is set to eSPECIFIC_DISTANCE.
    if (pCamera->FocusSource.Get() == FbxCamera::eFocusSpecificDistance)
    {
        DisplayDouble("        Specific Distance: ", pCamera->FocusDistance.Get());
    }

    DisplayDouble("        Focus Angle: ", pCamera->FocusAngle.Get(), " degrees");
}


void DisplayDefaultAnimationValues(FbxCamera* pCamera)
{
    DisplayString("    Default Animation Values");

    DisplayDouble("        Default Field of View: ", pCamera->FieldOfView.Get());
    DisplayDouble("        Default Field of View X: ", pCamera->FieldOfViewX.Get());
    DisplayDouble("        Default Field of View Y: ", pCamera->FieldOfViewY.Get());
    DisplayDouble("        Default Optical Center X: ", pCamera->OpticalCenterX.Get());
    DisplayDouble("        Default Optical Center Y: ", pCamera->OpticalCenterY.Get());
    DisplayDouble("        Default Roll: ", pCamera->Roll.Get());
}


