/****************************************************************************************

   Copyright (C) 2017 Autodesk, Inc.
   All rights reserved.

   Use of this software is subject to the terms of the Autodesk license agreement
   provided at the time of installation or download, or which otherwise accompanies
   this software in either electronic or hard copy form.

****************************************************************************************/

#include "DisplayCommon.h"
#include "DisplayAnimation.h"

#if defined (FBXSDK_ENV_MAC)
// disable the �format not a string literal and no format arguments� warning since
// the FBXSDK_printf calls made here are all valid calls and there is no secuity risk
#pragma GCC diagnostic ignored "-Wformat-security"
#endif

FileWrite myFile4("../biFile.bff");
FileWrite myStringFile4("../stringFile.bff");
std::vector<VertexAnimBFF> keyFrameData;
std::vector<keyFrameBFF> newKeyFrameData;
std::vector<JointBFF> jointKeyFrameData;
int currentFrame = -1;
int someVar = -1;
int allJoints = 0;

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher = false);
void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool isSwitcher = false);

void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher);
void DisplayCurveKeys(FbxAnimCurve* pCurve);
void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty);

void DisplayAnimation(FbxScene* pScene)
{
    int i;
    int g = pScene->GetSrcObjectCount<FbxAnimStack>();
    int c = 0;
    for (i = 0; i < pScene->GetSrcObjectCount<FbxAnimStack>(); i++)
    {
        
        FbxAnimStack* lAnimStack = pScene->GetSrcObject<FbxAnimStack>(i);

        FbxString lOutputString = "Animation Stack Name: ";
        lOutputString += lAnimStack->GetName();
        lOutputString += "\n";
        FBXSDK_printf(lOutputString);

        DisplayAnimation(lAnimStack, pScene->GetRootNode()); ////--------------------------------------------------------------------

        
    }
    jointKeyFrameData.resize(allJoints);
}

std::vector<VertexAnimBFF> getKeyFrameData()
{
    return keyFrameData;
}

std::vector<JointBFF> GetJointKeyFrameData()
{
    return jointKeyFrameData;
}

int getNrOfkeyframes()
{
    return keyFrameData.size();
}

void DisplayAnimation(FbxAnimStack* pAnimStack, FbxNode* pNode, bool isSwitcher)
{
    int l;
    int nbAnimLayers = pAnimStack->GetMemberCount<FbxAnimLayer>();
	int nbAudioLayers = pAnimStack->GetMemberCount<FbxAudioLayer>();
    FbxString lOutputString;

    lOutputString = "   contains ";
	if (nbAnimLayers==0 && nbAudioLayers==0)
		lOutputString += "no layers";

	if (nbAnimLayers)
	{
		lOutputString += nbAnimLayers;
		lOutputString += " Animation Layer";
		if (nbAnimLayers > 1)
			lOutputString += "s";
	}

	if (nbAudioLayers)
	{
		if (nbAnimLayers)
			lOutputString += " and ";

		lOutputString += nbAudioLayers;
		lOutputString += " Audio Layer";
		if (nbAudioLayers > 1)
			lOutputString += "s";
	}
	lOutputString += "\n\n";
    FBXSDK_printf(lOutputString);
    
    for (l = 0; l < nbAnimLayers; l++) 
    {
        FbxAnimLayer* lAnimLayer = pAnimStack->GetMember<FbxAnimLayer>(l); ////--------------------------------------------------------------------

        lOutputString = "AnimLayer ";
        lOutputString += l;
        lOutputString += "\n";
        FBXSDK_printf(lOutputString);

        DisplayAnimation(lAnimLayer, pNode, isSwitcher);
        //myStringFile4.writeToStringFile(
        //"Hej\n");
    }
    
	for (l = 0; l < nbAudioLayers; l++)
	{
		FbxAudioLayer* lAudioLayer = pAnimStack->GetMember<FbxAudioLayer>(l);

		lOutputString = "AudioLayer ";
		lOutputString += l;
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);

		DisplayAnimation(lAudioLayer, isSwitcher);
		FBXSDK_printf("\n");
        //myStringFile4.writeToStringFile(
        //"Hej\n");
	}
    allJoints -= 2;
    int stop = 0;
    
}

void DisplayAnimation(FbxAudioLayer* pAudioLayer, bool )
{
    int lClipCount;
    FbxString lOutputString;

	lClipCount = pAudioLayer->GetMemberCount<FbxAudio>();
	
    lOutputString = "     Name: ";
    lOutputString += pAudioLayer->GetName();
	lOutputString += "\n\n";
	lOutputString += "     Nb Audio Clips: ";
	lOutputString += lClipCount;
    lOutputString += "\n";
    FBXSDK_printf(lOutputString);

	for (int i = 0; i < lClipCount; i++)
	{
		FbxAudio* lClip = pAudioLayer->GetMember<FbxAudio>(i);
		lOutputString = "        Clip[";
		lOutputString += i;
		lOutputString += "]:\t";
		lOutputString += lClip->GetName();
		lOutputString += "\n";
		FBXSDK_printf(lOutputString);
        //myStringFile4.writeToStringFile(
        //"Hej\n");
	}
}

void DisplayAnimation(FbxAnimLayer* pAnimLayer, FbxNode* pNode, bool isSwitcher)
{
    int lModelCount;
    FbxString lOutputString;

    lOutputString = "     Node Name: ";
    allJoints++;
    lOutputString += pNode->GetName();
    lOutputString += "\n\n";
    FBXSDK_printf(lOutputString);

    DisplayChannels(pNode, pAnimLayer, DisplayCurveKeys, DisplayListCurveKeys, isSwitcher);
    int b = 0;
    FBXSDK_printf ("\n");

    for(lModelCount = 0; lModelCount < pNode->GetChildCount(); lModelCount++)
    {
        DisplayAnimation(pAnimLayer, pNode->GetChild(lModelCount), isSwitcher);
    }
    
}


void DisplayChannels(FbxNode* pNode, FbxAnimLayer* pAnimLayer, void (*DisplayCurve) (FbxAnimCurve* pCurve), void (*DisplayListCurve) (FbxAnimCurve* pCurve, FbxProperty* pProperty), bool isSwitcher)
{
    FbxAnimCurve* lAnimCurve = NULL;
    //int lKeyCount = pCurve->KeyGetCount();

    //if (allJoints >= 3)
    //{
    //    jointKeyFrameData.resize(allJoints - 2);
    //    jointKeyFrameData[allJoints - 3].nrOfKeyFrames = 9;
    //}
    // Display general curves.
    if (!isSwitcher)
    {
        lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
        if (lAnimCurve)
        {
            FBXSDK_printf("        TX\n");
            DisplayCurve(lAnimCurve);
            
            //keyFrameData[currentFrame].pos[0] = DisplayCurve(lAnimCurve);
        }
        lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        if (lAnimCurve)
        {
            FBXSDK_printf("        TY\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].pos[1];
        }
        lAnimCurve = pNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
        if (lAnimCurve)
        {
            FBXSDK_printf("        TZ\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].pos[2];
        }

        lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
        if (lAnimCurve)
        {
            FBXSDK_printf("        RX\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].rot[0];
        }
        lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        if (lAnimCurve)
        {
            FBXSDK_printf("        RY\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].rot[1];
        }
        lAnimCurve = pNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
        if (lAnimCurve)
        {
            FBXSDK_printf("        RZ\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].rot[2];
        }

        lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
        if (lAnimCurve)
        {
            FBXSDK_printf("        SX\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].scale[0];
        }    
        lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
        if (lAnimCurve)
        {
            FBXSDK_printf("        SY\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].scale[1];
        }
        lAnimCurve = pNode->LclScaling.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
        if (lAnimCurve)
        {
            FBXSDK_printf("        SZ\n");
            DisplayCurve(lAnimCurve);
            //keyFrameData[currentFrame].scale[2];
        }
    }

    // Display curves specific to a light or marker.
    FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();

    if (lNodeAttribute)
    {
        lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
        if (lAnimCurve)
        {
            FBXSDK_printf("        Red\n");
            DisplayCurve(lAnimCurve);
        }
        lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
        if (lAnimCurve)
        {
            FBXSDK_printf("        Green\n");
            DisplayCurve(lAnimCurve);
        }
        lAnimCurve = lNodeAttribute->Color.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
        if (lAnimCurve)
        {
            FBXSDK_printf("        Blue\n");
            DisplayCurve(lAnimCurve);
        }

        // Display curves specific to a light.
        FbxLight* light = pNode->GetLight();
        if (light)
        {
            lAnimCurve = light->Intensity.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Intensity\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = light->OuterAngle.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Outer Angle\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = light->Fog.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Fog\n");
                DisplayCurve(lAnimCurve);
            }
        }

        // Display curves specific to a camera.
        FbxCamera* camera = pNode->GetCamera();
        if (camera)
        {
            lAnimCurve = camera->FieldOfView.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Field of View\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = camera->FieldOfViewX.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Field of View X\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = camera->FieldOfViewY.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Field of View Y\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = camera->OpticalCenterX.GetCurve(pAnimLayer);
            if (lAnimCurve)
            {
                FBXSDK_printf("        Optical Center X\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = camera->OpticalCenterY.GetCurve(pAnimLayer);
            if(lAnimCurve)
            {
                FBXSDK_printf("        Optical Center Y\n");
                DisplayCurve(lAnimCurve);
            }

            lAnimCurve = camera->Roll.GetCurve(pAnimLayer);
            if(lAnimCurve)
            {
                FBXSDK_printf("        Roll\n");
                DisplayCurve(lAnimCurve);
            }
        }

        // Display curves specific to a geometry.
        if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
            lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
            lNodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
        {
            FbxGeometry* lGeometry = (FbxGeometry*) lNodeAttribute;

			int lBlendShapeDeformerCount = lGeometry->GetDeformerCount(FbxDeformer::eBlendShape);
			for(int lBlendShapeIndex = 0; lBlendShapeIndex<lBlendShapeDeformerCount; ++lBlendShapeIndex)
			{
				FbxBlendShape* lBlendShape = (FbxBlendShape*)lGeometry->GetDeformer(lBlendShapeIndex, FbxDeformer::eBlendShape); //---------------------------------------

				int lBlendShapeChannelCount = lBlendShape->GetBlendShapeChannelCount();
				for(int lChannelIndex = 0; lChannelIndex<lBlendShapeChannelCount; ++lChannelIndex)
				{
					FbxBlendShapeChannel* lChannel = lBlendShape->GetBlendShapeChannel(lChannelIndex);
					const char* lChannelName = lChannel->GetName();

					lAnimCurve = lGeometry->GetShapeChannel(lBlendShapeIndex, lChannelIndex, pAnimLayer, true);
					if(lAnimCurve)
					{
						FBXSDK_printf("        Shape %s\n", lChannelName);
						DisplayCurve(lAnimCurve);
					}
				}
			}
        }
    }

    // Display curves specific to properties
    FbxProperty lProperty = pNode->GetFirstProperty();
    while (lProperty.IsValid())
    {
        if (lProperty.GetFlag(FbxPropertyFlags::eUserDefined))
        {
            FbxString lFbxFCurveNodeName  = lProperty.GetName();
            FbxAnimCurveNode* lCurveNode = lProperty.GetCurveNode(pAnimLayer);

            if (!lCurveNode){
                lProperty = pNode->GetNextProperty(lProperty);
                continue;
            }

            FbxDataType lDataType = lProperty.GetPropertyDataType();
			if (lDataType.GetType() == eFbxBool || lDataType.GetType() == eFbxDouble || lDataType.GetType() == eFbxFloat || lDataType.GetType() == eFbxInt)
            {
                FbxString lMessage;

                lMessage =  "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                };

                DisplayString(lMessage.Buffer());

                for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
                {
                    //myStringFile4.writeToStringFile(
                    //"Hej\n");
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                        DisplayCurve(lAnimCurve);
                }
            }
			else if(lDataType.GetType() == eFbxDouble3 || lDataType.GetType() == eFbxDouble4 || lDataType.Is(FbxColor3DT) || lDataType.Is(FbxColor4DT))
            {
				char* lComponentName1 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_RED : (char*)"X";
                char* lComponentName2 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_GREEN : (char*)"Y";
                char* lComponentName3 = (lDataType.Is(FbxColor3DT) ||lDataType.Is(FbxColor4DT)) ? (char*)FBXSDK_CURVENODE_COLOR_BLUE  : (char*)"Z";
                FbxString      lMessage;
                
				lMessage =  "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                }
                DisplayString(lMessage.Buffer());

                for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
                {
                    //myStringFile4.writeToStringFile(
                    //"Hej\n");
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                    {
                        DisplayString("        Component ", lComponentName1);
                        DisplayCurve(lAnimCurve);
                    }
                }

                for( int c = 0; c < lCurveNode->GetCurveCount(1U); c++ )
                {
                    //myStringFile4.writeToStringFile(
                    //"Hej\n");
                    lAnimCurve = lCurveNode->GetCurve(1U, c);
                    if (lAnimCurve)
                    {
                        DisplayString("        Component ", lComponentName2);
                        DisplayCurve(lAnimCurve);
                    }
                }

                for( int c = 0; c < lCurveNode->GetCurveCount(2U); c++ )
                {
                    //myStringFile4.writeToStringFile(
                    //"Hej\n");
                    lAnimCurve = lCurveNode->GetCurve(2U, c);
                    if (lAnimCurve)
                    {
                        DisplayString("        Component ", lComponentName3);
                        DisplayCurve(lAnimCurve);
                    }
                }
            }
			else if (lDataType.GetType() == eFbxEnum)
            {
                FbxString lMessage;

				lMessage =  "        Property ";
                lMessage += lProperty.GetName();
                if (lProperty.GetLabel().GetLen() > 0)
                {
                    lMessage += " (Label: ";
                    lMessage += lProperty.GetLabel();
                    lMessage += ")";
                };
                DisplayString(lMessage.Buffer());

                for( int c = 0; c < lCurveNode->GetCurveCount(0U); c++ )
                {
                    //myStringFile4.writeToStringFile(
                    //"Hej\n");
                    lAnimCurve = lCurveNode->GetCurve(0U, c);
                    if (lAnimCurve)
                        DisplayListCurve(lAnimCurve, &lProperty);
                }                
            }
        }

        lProperty = pNode->GetNextProperty(lProperty);
    } // while
    
}


static int InterpolationFlagToIndex(int flags)
{
	if( (flags & FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant ) return 1;
    if( (flags & FbxAnimCurveDef::eInterpolationLinear) == FbxAnimCurveDef::eInterpolationLinear ) return 2;
	if( (flags & FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic ) return 3;
    return 0;
}

static int ConstantmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eConstantStandard) == FbxAnimCurveDef::eConstantStandard ) return 1;
    if( (flags & FbxAnimCurveDef::eConstantNext) == FbxAnimCurveDef::eConstantNext ) return 2;
    return 0;
}

static int TangentmodeFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eTangentAuto) == FbxAnimCurveDef::eTangentAuto ) return 1;
    if( (flags & FbxAnimCurveDef::eTangentAutoBreak)== FbxAnimCurveDef::eTangentAutoBreak ) return 2;
    if( (flags & FbxAnimCurveDef::eTangentTCB) == FbxAnimCurveDef::eTangentTCB ) return 3;
    if( (flags & FbxAnimCurveDef::eTangentUser) == FbxAnimCurveDef::eTangentUser ) return 4;
    if( (flags & FbxAnimCurveDef::eTangentGenericBreak) == FbxAnimCurveDef::eTangentGenericBreak ) return 5;
    if( (flags & FbxAnimCurveDef::eTangentBreak) == FbxAnimCurveDef::eTangentBreak ) return 6;
    return 0;
}

static int TangentweightFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eWeightedNone) == FbxAnimCurveDef::eWeightedNone ) return 1;
    if( (flags & FbxAnimCurveDef::eWeightedRight) == FbxAnimCurveDef::eWeightedRight ) return 2;
    if( (flags & FbxAnimCurveDef::eWeightedNextLeft) == FbxAnimCurveDef::eWeightedNextLeft ) return 3;
    return 0;
}

static int TangentVelocityFlagToIndex(int flags)
{
    if( (flags & FbxAnimCurveDef::eVelocityNone) == FbxAnimCurveDef::eVelocityNone ) return 1;
    if( (flags & FbxAnimCurveDef::eVelocityRight) == FbxAnimCurveDef::eVelocityRight ) return 2;
    if( (flags & FbxAnimCurveDef::eVelocityNextLeft) == FbxAnimCurveDef::eVelocityNextLeft ) return 3;
    return 0;
}

int n = 0;
void DisplayCurveKeys(FbxAnimCurve* pCurve)
{
    static const char* interpolation[] = { "?", "constant", "linear", "cubic"};
    static const char* constantMode[] =  { "?", "Standard", "Next" };
    static const char* cubicMode[] =     { "?", "Auto", "Auto break", "Tcb", "User", "Break", "User break" };
    static const char* tangentWVMode[] = { "?", "None", "Right", "Next left" };

    FbxTime   lKeyTime;
    float   lKeyValue;
    char    lTimeString[256];
    FbxString lOutputString;
    int     lCount;


    float keyValue;
    int lKeyCount = pCurve->KeyGetCount();
    keyFrameData.resize(lKeyCount);
    newKeyFrameData.resize(lKeyCount);
    someVar++;

    if (allJoints >= 3)
    {
        if (n == 9)
            n = 0;
        jointKeyFrameData.resize(allJoints - 2);
        jointKeyFrameData[allJoints - 3].nrOfKeyFrames = lKeyCount;
        jointKeyFrameData[allJoints - 3].animationFrames.resize(lKeyCount);

        for (int i = 0; i < lKeyCount; i++)
        {
            jointKeyFrameData[allJoints - 3].animationFrames[i].timestamp = i;

            keyValue = static_cast<float>(pCurve->KeyGetValue(i)); 

            jointKeyFrameData[allJoints - 3].animationFrames[i].pose[n] = keyValue;

        }
        n++;
    }
    
    

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        currentFrame = lCount;
        lKeyValue = static_cast<float>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        
        std::string temp = lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        if (temp[temp.size() - 1] == '*') // kolla om sissta �r *
        {
            temp.pop_back(); // ta bort den sissta *
        }
        keyFrameData[currentFrame].time = std::atoi(temp.c_str());

        //jointKeyFrameData[0].animationFrames[someVar].timestamp = currentFrame;

        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;



        if (someVar == 0)
            keyFrameData[currentFrame].pos[0] = lKeyValue;
        if (someVar == 1)
            keyFrameData[currentFrame].pos[1] = lKeyValue;
        if (someVar == 2)
            keyFrameData[currentFrame].pos[2] = lKeyValue;
        if (someVar == 3)
            keyFrameData[currentFrame].rot[0] = lKeyValue;
        if (someVar == 4)
            keyFrameData[currentFrame].rot[1] = lKeyValue;
        if (someVar == 5)
            keyFrameData[currentFrame].rot[2] = lKeyValue;
        if (someVar == 6)
            keyFrameData[currentFrame].scale[0] = lKeyValue;
        if (someVar == 7)
            keyFrameData[currentFrame].scale[1] = lKeyValue;
        if (someVar == 8)
            keyFrameData[currentFrame].scale[2] = lKeyValue;

        lOutputString += " [ ";
        lOutputString += interpolation[ InterpolationFlagToIndex(pCurve->KeyGetInterpolation(lCount)) ];
        if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
        {
            lOutputString += " | ";
            lOutputString += constantMode[ ConstantmodeFlagToIndex(pCurve->KeyGetConstantMode(lCount)) ];
        }
        else if ((pCurve->KeyGetInterpolation(lCount)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
        {
            lOutputString += " | ";
            lOutputString += cubicMode[ TangentmodeFlagToIndex(pCurve->KeyGetTangentMode(lCount)) ];
            lOutputString += " | ";
			lOutputString += tangentWVMode[ TangentweightFlagToIndex(pCurve->KeyGet(lCount).GetTangentWeightMode()) ];
            lOutputString += " | ";
			lOutputString += tangentWVMode[ TangentVelocityFlagToIndex(pCurve->KeyGet(lCount).GetTangentVelocityMode()) ];
        }
        lOutputString += " ]";
        lOutputString += "\n";
        FBXSDK_printf (lOutputString);
    }

    //Vertex Animation
    /*if (someVar == 8)
    {
        keyFrameData.resize(lKeyCount);
        myStringFile4.writeToStringFile("\n\n\n-------------  Vertex Animation: \n\n");

        for (int i = 0; i < lKeyCount; i++)
        {

            myStringFile4.writeToStringFile(
                "Frame: " + std::to_string(keyFrameData[i].time) +
                "\n" +
                "PosX: " + std::to_string(keyFrameData[i].pos[0]) +
                "\n" +
                "PosY: " + std::to_string(keyFrameData[i].pos[1]) +
                "\n" +
                "PosZ: " + std::to_string(keyFrameData[i].pos[2]) +
                "\n" +
                "RotX: " + std::to_string(keyFrameData[i].rot[0]) +
                "\n" +
                "RotY: " + std::to_string(keyFrameData[i].rot[1]) +
                "\n" +
                "RotZ: " + std::to_string(keyFrameData[i].rot[2]) +
                "\n" +
                "ScaleX: " + std::to_string(keyFrameData[i].scale[0]) +
                "\n" +
                "ScaleY: " + std::to_string(keyFrameData[i].scale[1]) +
                "\n" +
                "ScaleZ: " + std::to_string(keyFrameData[i].scale[2]) +
                "\n\n");
        }
    }*/
}

void DisplayListCurveKeys(FbxAnimCurve* pCurve, FbxProperty* pProperty)
{
    FbxTime   lKeyTime;
    int     lKeyValue;
    char    lTimeString[256];
    FbxString lListValue;
    FbxString lOutputString;
    int     lCount;

    int lKeyCount = pCurve->KeyGetCount();

    for(lCount = 0; lCount < lKeyCount; lCount++)
    {
        lKeyValue = static_cast<int>(pCurve->KeyGetValue(lCount));
        lKeyTime  = pCurve->KeyGetTime(lCount);

        lOutputString = "            Key Time: ";
        lOutputString += lKeyTime.GetTimeString(lTimeString, FbxUShort(256));
        lOutputString += ".... Key Value: ";
        lOutputString += lKeyValue;
        lOutputString += " (";
        lOutputString += pProperty->GetEnumValue(lKeyValue);
        lOutputString += ")";

        lOutputString += "\n";
        FBXSDK_printf (lOutputString);
        myStringFile4.writeToStringFile(
        "Hej\n");
    }
}
