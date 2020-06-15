/****************************************************************************************

   Copyright (C) 2015 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.
 
****************************************************************************************/

#include <fbxsdk.h>

#include "DisplayCommon.h"
#include "DisplayMaterial.h"
std::vector<MaterialBFF> material;
int nrOfMaterials;

std::vector<MaterialBFF> GetMaterialData()
{
	return material;
}

int GetNrOfMaterials()
{
    return nrOfMaterials;
}



void DisplayMaterial(FbxGeometry* pGeometry)
{	
	//FileWrite myFile("../biFile.bff");
	//FileWrite myStringFile("../stringFile.bff");
	//myStringFile.writeToStringFile("HejHej");
	
    int lMaterialCount = 0;
    FbxNode* lNode = NULL;
    if(pGeometry){
        lNode = pGeometry->GetNode();
        if(lNode)
            lMaterialCount = lNode->GetMaterialCount();    
    }
    nrOfMaterials = lMaterialCount;
    material.resize(lMaterialCount);

    if (lMaterialCount > 0)
    {
        FbxPropertyT<FbxDouble3> lKFbxDouble3;
        FbxPropertyT<FbxDouble> lKFbxDouble1;
        FbxColor theColor;

        for (int lCount = 0; lCount < lMaterialCount; lCount ++)
        {
            DisplayInt("        Material ", lCount);

            FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(lCount);

            DisplayString("            Name: \"", (char *) lMaterial->GetName(), "\""); 

            //Get the implementation to see if it's a hardware shader.
            const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
            FbxString lImplemenationType = "HLSL";
            if(!lImplementation)
            {
                lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
                lImplemenationType = "CGFX";
            }
            if(lImplementation)
            {
                //Now we have a hardware shader, let's read it
                FBXSDK_printf("            Hardware Shader Type: %s\n", lImplemenationType.Buffer());
                const FbxBindingTable* lRootTable = lImplementation->GetRootTable();
                FbxString lFileName = lRootTable->DescAbsoluteURL.Get();
                FbxString lTechniqueName = lRootTable->DescTAG.Get(); 


                const FbxBindingTable* lTable = lImplementation->GetRootTable();
                size_t lEntryNum = lTable->GetEntryCount();

                for(int i=0;i <(int)lEntryNum; ++i)
                {
                    const FbxBindingTableEntry& lEntry = lTable->GetEntry(i);
                    const char* lEntrySrcType = lEntry.GetEntryType(true); 
                    FbxProperty lFbxProp;


                    FbxString lTest = lEntry.GetSource();
                    FBXSDK_printf("            Entry: %s\n", lTest.Buffer());


                    if ( strcmp( FbxPropertyEntryView::sEntryType, lEntrySrcType ) == 0 )
                    {   
                        lFbxProp = lMaterial->FindPropertyHierarchical(lEntry.GetSource()); 
                        if(!lFbxProp.IsValid())
                        {
                            lFbxProp = lMaterial->RootProperty.FindHierarchical(lEntry.GetSource());
                        }


                    }
                    else if( strcmp( FbxConstantEntryView::sEntryType, lEntrySrcType ) == 0 )
                    {
                        lFbxProp = lImplementation->GetConstants().FindHierarchical(lEntry.GetSource());
                    }
                    if(lFbxProp.IsValid())
                    {
                        if( lFbxProp.GetSrcObjectCount<FbxTexture>() > 0 )
                        {
                            //do what you want with the textures
                            for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxFileTexture>(); ++j)
                            {
                                FbxFileTexture *lTex = lFbxProp.GetSrcObject<FbxFileTexture>(j);
                                FBXSDK_printf("           File Texture: %s\n", lTex->GetFileName());
                            }
                            for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxLayeredTexture>(); ++j)
                            {
                                FbxLayeredTexture *lTex = lFbxProp.GetSrcObject<FbxLayeredTexture>(j);
                                FBXSDK_printf("        Layered Texture: %s\n", lTex->GetName());
                            }
                            for(int j=0; j<lFbxProp.GetSrcObjectCount<FbxProceduralTexture>(); ++j)
                            {
                                FbxProceduralTexture *lTex = lFbxProp.GetSrcObject<FbxProceduralTexture>(j);
                                FBXSDK_printf("     Procedural Texture: %s\n", lTex->GetName());
                            }
                        }
                        else
                        {
                            FbxDataType lFbxType = lFbxProp.GetPropertyDataType();
                            FbxString blah = lFbxType.GetName();
                            if(FbxBoolDT == lFbxType)
                            {
                                DisplayBool("                Bool: ", lFbxProp.Get<FbxBool>() );
                            }
                            else if ( FbxIntDT == lFbxType ||  FbxEnumDT  == lFbxType )
                            {
                                DisplayInt("                Int: ", lFbxProp.Get<FbxInt>());
                            }
                            else if ( FbxFloatDT == lFbxType)
                            {
                                DisplayDouble("                Float: ", lFbxProp.Get<FbxFloat>());

                            }
                            else if ( FbxDoubleDT == lFbxType)
                            {
                                DisplayDouble("                Double: ", lFbxProp.Get<FbxDouble>());
                            }
                            else if ( FbxStringDT == lFbxType
                                ||  FbxUrlDT  == lFbxType
                                ||  FbxXRefUrlDT  == lFbxType )
                            {
                                DisplayString("                String: ", lFbxProp.Get<FbxString>().Buffer());
                            }
                            else if ( FbxDouble2DT == lFbxType)
                            {
                                FbxDouble2 lDouble2 = lFbxProp.Get<FbxDouble2>();
                                FbxVector2 lVect;
                                lVect[0] = lDouble2[0];
                                lVect[1] = lDouble2[1];

                                Display2DVector("                2D vector: ", lVect);
                            }
                            else if ( FbxDouble3DT == lFbxType || FbxColor3DT == lFbxType)
                            {
                                FbxDouble3 lDouble3 = lFbxProp.Get<FbxDouble3>();


                                FbxVector4 lVect;
                                lVect[0] = lDouble3[0];
                                lVect[1] = lDouble3[1];
                                lVect[2] = lDouble3[2];
                                Display3DVector("                3D vector: ", lVect);
                            }

                            else if ( FbxDouble4DT == lFbxType || FbxColor4DT == lFbxType)
                            {
                                FbxDouble4 lDouble4 = lFbxProp.Get<FbxDouble4>();
                                FbxVector4 lVect;
                                lVect[0] = lDouble4[0];
                                lVect[1] = lDouble4[1];
                                lVect[2] = lDouble4[2];
                                lVect[3] = lDouble4[3];
                                Display4DVector("                4D vector: ", lVect);
                            }
                            else if ( FbxDouble4x4DT == lFbxType)
                            {
                                FbxDouble4x4 lDouble44 = lFbxProp.Get<FbxDouble4x4>();
                                for(int j=0; j<4; ++j)
                                {

                                    FbxVector4 lVect;
                                    lVect[0] = lDouble44[j][0];
                                    lVect[1] = lDouble44[j][1];
                                    lVect[2] = lDouble44[j][2];
                                    lVect[3] = lDouble44[j][3];
                                    Display4DVector("                4x4D vector: ", lVect);
                                }

                            }
                        }

                    }   
                }
            }
            else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
            {
                // We found a Phong material.  Display its properties.

                // Display the Ambient Color
                lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Ambient;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Ambient: ", theColor);

                // Display the Diffuse Color
                lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Diffuse;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Diffuse: ", theColor);

                // Display the Specular Color (unique to Phong materials)
                lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Specular;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Specular: ", theColor);

                // Display the Emissive Color
                lKFbxDouble3 =((FbxSurfacePhong *) lMaterial)->Emissive;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Emissive: ", theColor);

                //Opacity is Transparency factor now
                lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->TransparencyFactor;
                DisplayDouble("            Opacity: ", 1.0-lKFbxDouble1.Get());

                // Display the Shininess
                lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->Shininess;
                DisplayDouble("            Shininess: ", lKFbxDouble1.Get());

                // Display the Reflectivity
                lKFbxDouble1 =((FbxSurfacePhong *) lMaterial)->ReflectionFactor;
                DisplayDouble("            Reflectivity: ", lKFbxDouble1.Get());
            }
            else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
            {
                // We found a Lambert material. Display its properties.
                // Display the Ambient Color
                lKFbxDouble3=((FbxSurfaceLambert *)lMaterial)->Ambient;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Ambient: ", theColor);
				material[lCount].Ambient[0] = lKFbxDouble3.Get()[0];
				material[lCount].Ambient[1] = lKFbxDouble3.Get()[1];
				material[lCount].Ambient[2] = lKFbxDouble3.Get()[2];

                // Display the Ambient Factor
                lKFbxDouble1 = ((FbxSurfaceLambert*)lMaterial)->AmbientFactor;
                DisplayDouble("            Ambient Factor: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].AmbientFactor = 1.0 - lKFbxDouble1.Get();

                // Display the Diffuse Color
                lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Diffuse;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Diffuse: ", theColor);
				material[lCount].Diffuse[0] = lKFbxDouble3.Get()[0];
				material[lCount].Diffuse[1] = lKFbxDouble3.Get()[1];
				material[lCount].Diffuse[2] = lKFbxDouble3.Get()[2];

                // Display the Diffuse Factor
                lKFbxDouble1 = ((FbxSurfaceLambert*)lMaterial)->DiffuseFactor;
                DisplayDouble("            Diffuse Factor: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].DiffuseFactor = 1.0 - lKFbxDouble1.Get();

                // Display the Emissive
                lKFbxDouble3 =((FbxSurfaceLambert *)lMaterial)->Emissive;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Emissive: ", theColor);
                material[lCount].Emissive[0] = lKFbxDouble3.Get()[0];
                material[lCount].Emissive[1] = lKFbxDouble3.Get()[1];
                material[lCount].Emissive[2] = lKFbxDouble3.Get()[2];

                // Display Opacity Color
                lKFbxDouble3 = ((FbxSurfaceLambert*)lMaterial)->TransparentColor;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayColor("            Opacity Color:", theColor);
                material[lCount].TransparencyColor[0] = lKFbxDouble3.Get()[0];
                material[lCount].TransparencyColor[1] = lKFbxDouble3.Get()[1];
                material[lCount].TransparencyColor[2] = lKFbxDouble3.Get()[2];
                
                // Display the Transparency
                lKFbxDouble1 =((FbxSurfaceLambert *)lMaterial)->TransparencyFactor;
                DisplayDouble("            Transparency: ", 1.0-lKFbxDouble1.Get());
				material[lCount].Transparency = 1.0 - lKFbxDouble1.Get();

                // Diasplay Normal Map
                lKFbxDouble3 = ((FbxSurfaceLambert*)lMaterial)->NormalMap;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayDouble("            Normal Map: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].NormalMap[0] = lKFbxDouble3.Get()[0];
                material[lCount].NormalMap[1] = lKFbxDouble3.Get()[1];
                material[lCount].NormalMap[2] = lKFbxDouble3.Get()[2];

                // Display the Bump
                lKFbxDouble1 = ((FbxSurfaceLambert*)lMaterial)->Bump;
                DisplayDouble("            Bump: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].Bump = 1.0 - lKFbxDouble1.Get();

                // Display the Bump Factor
                lKFbxDouble1 = ((FbxSurfaceLambert*)lMaterial)->BumpFactor;
                DisplayDouble("            Bump Factor: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].BumpFactor = 1.0 - lKFbxDouble1.Get();

                // Diasplay Displacement Color
                lKFbxDouble3 = ((FbxSurfaceLambert*)lMaterial)->DisplacementColor;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayDouble("            Displacement Color: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].DisplacementColor[0] = lKFbxDouble3.Get()[0];
                material[lCount].DisplacementColor[1] = lKFbxDouble3.Get()[1];
                material[lCount].DisplacementColor[2] = lKFbxDouble3.Get()[2];

                // Display the Displacement Factor
                lKFbxDouble1 = ((FbxSurfaceLambert*)lMaterial)->BumpFactor;
                DisplayDouble("            Displacement Factor: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].DisplacementFactor = 1.0 - lKFbxDouble1.Get();

                // Diasplay Vector Displacement Color
                lKFbxDouble3 = ((FbxSurfaceLambert*)lMaterial)->VectorDisplacementColor;
                theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
                DisplayDouble("            Vector Displacement Color: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].VectorDisplacementColor[0] = lKFbxDouble3.Get()[0];
                material[lCount].VectorDisplacementColor[1] = lKFbxDouble3.Get()[1];
                material[lCount].VectorDisplacementColor[2] = lKFbxDouble3.Get()[2];

                // Display the Vector Displacement Factor
                lKFbxDouble1 = ((FbxSurfaceLambert*)lMaterial)->VectorDisplacementFactor;
                DisplayDouble("            Vector Displacement Factor: ", 1.0 - lKFbxDouble1.Get());
                material[lCount].VectorDisplacementFactor = 1.0 - lKFbxDouble1.Get();
            }
            else
                DisplayString("Unknown type of Material");

            FbxPropertyT<FbxString> lString;
            lString = lMaterial->ShadingModel;
            DisplayString("            Shading Model: ", lString.Get().Buffer());
            DisplayString("");
        }
    }
}

