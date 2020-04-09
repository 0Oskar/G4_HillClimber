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
#include<map>

// DirectX 11
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include "pugixml.hpp"


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
};