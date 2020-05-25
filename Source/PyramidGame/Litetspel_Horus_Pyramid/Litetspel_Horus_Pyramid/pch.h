#pragma once

#ifndef UNICODE
#define UNICODE
#endif

// Standard
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <comdef.h>
#include <assert.h>
#include <memory>
#include <wrl/client.h>
#define NOMINMAX // std::min/std::max wont work without this
#include <algorithm>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include <sstream>
//#include <ctgmath>
#include <math.h>
#include <fstream>
#include <random>
#include <algorithm>
#include <iterator>
#include <utility>

// Utilities
#include "pugixml.hpp"
#include "../Importer/ImporterBFF.h"
#include"PyramidMathTools.h"

// DirectX 11
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <SimpleMath.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <CommonStates.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <VertexTypes.h>
#include <DirectXColors.h>
#include "DebugDraw.h"
#include "Audio.h"

using namespace DirectX;

// DirectX 11 Linking
#pragma comment (lib, "user32")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "gdi32")
#pragma comment (lib, "d3dcompiler.lib")

// Helper Struct
struct GameOptions
{
	int width;
	int height;
	float fov;
	float mouseSensitivity;
};

enum class Event
{
	Invalid,
	Pressed,
	Released,
	MouseLPressed,
	MouseLReleased,
	MouseRPressed,
	MouseRReleased,
	MouseMPressed,
	MouseMReleased,
	MouseWheelUp,
	MouseWheelDown,
	MouseMove,
	MouseRAW_MOVE
};