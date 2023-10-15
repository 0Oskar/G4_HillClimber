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
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>
#include <string_view>
#include <sstream>
#include <ctime>
#include <thread>
#include <future>

#include <math.h>
#include <fstream>
#include <stack>
#include <random>
#include <algorithm>
#include <iterator>
#include <utility>

#define MODEL_IMPORT_THREAD_COUNT 4
#define INDENT_CHAR std::string("                                ")
static size_t INDENT_CHAR_LENGTH = INDENT_CHAR.length();

// Utilities
#include "pugixml.hpp"
#include "../Importer/ImporterBFF.h"
#include "PyramidMathTools.h"
#include "Timer.h"

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
	std::string name;
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