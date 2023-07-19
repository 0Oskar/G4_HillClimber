#pragma once
#include <d3d11.h>

struct RenderTexture
{
	ID3D11Texture2D* rtt;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* srv;
	ID3D11UnorderedAccessView* uav;
	DXGI_FORMAT format;
	char name[256];

	RenderTexture()
	{
		rtt = nullptr;
		rtv = nullptr;
		srv = nullptr;
		uav = nullptr;
		format = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	~RenderTexture()
	{
		if (rtt)
			rtt->Release();
		if (rtv)
			rtv->Release();
		if (srv)
			srv->Release();
		if (uav)
			uav->Release();
	}
};