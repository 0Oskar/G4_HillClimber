/****************************************************************************************

   Copyright (C) 2017 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayLight.h"

FileWrite myFile3("../biFile.bff");
FileWrite myStringFile3("../stringFile.bff");
LightBFF lightData;

void DisplayDefaultAnimationValues(FbxLight* pLight);
//Gammal kod

void DisplayLight(FbxNode* pNode)
{
    FbxLight* lLight = (FbxLight*) pNode->GetNodeAttribute();

    DisplayString("Light Name: ", (char *) pNode->GetName());
    char temp[64];
    strcpy_s(temp, _countof(temp), pNode->GetName());
    for (int i = 0; i < sizeof(temp); i++)
        lightData.name[i] = temp[i];
    DisplayMetaDataConnections(lLight);

    const char* lLightTypes[] = { "Point", "Directional", "Spot", "Area", "Volume" };

    DisplayString("    Type: ", lLightTypes[lLight->LightType.Get()]);

    strcpy_s(temp, _countof(temp), lLightTypes[lLight->LightType.Get()]);
    for (int i = 0; i < sizeof(temp); i++)
        lightData.type[i] = temp[i];

    DisplayBool("    Cast Light: ", lLight->CastLight.Get());

    if (!(lLight->FileName.Get().IsEmpty()))
    {
        DisplayString("    Gobo");

        DisplayString("        File Name: \"", lLight->FileName.Get().Buffer(), "\"");
        DisplayBool("        Ground Projection: ", lLight->DrawGroundProjection.Get());
        DisplayBool("        Volumetric Projection: ", lLight->DrawVolumetricLight.Get());
        DisplayBool("        Front Volumetric Projection: ", lLight->DrawFrontFacingVolumetricLight.Get());
    }

    DisplayDefaultAnimationValues(lLight);

    myStringFile3.writeToStringFile("\n\n\n------------- Light\n\n");
    myStringFile3.writeToStringFile(
        "Name: " + (std::string)lightData.name + "\n" +
        "\n" +
        "Type: " + (std::string)lightData.type + "\n" +
        "\n" +
        "ColorR: " + std::to_string(lightData.color[0]) + "\n" +
        "ColorG: " + std::to_string(lightData.color[1]) + "\n" +
        "ColorB: " + std::to_string(lightData.color[2]) + "\n" +
        "\n" +
        "Dir: " + std::to_string(lightData.dir) + "\n" +
        "\n" +
        "Intencity: " + std::to_string(lightData.intencity) + "\n");
        
}


void DisplayDefaultAnimationValues(FbxLight* pLight)
{
    DisplayString("    Default Animation Values");

    FbxDouble3 c = pLight->Color.Get();
    FbxColor lColor(c[0], c[1], c[2]);
    DisplayColor("        Default Color: ", lColor);
    lightData.color[0] = lColor[0];
    lightData.color[1] = lColor[1];
    lightData.color[2] = lColor[2];
    DisplayDouble("        Default Intensity: ", pLight->Intensity.Get());
    lightData.intencity = pLight->Intensity.Get();
    DisplayDouble("        Default Outer Angle: ", pLight->OuterAngle.Get());
    lightData.dir = pLight->OuterAngle.Get();
    DisplayDouble("        Default Fog: ", pLight->Fog.Get());
}

