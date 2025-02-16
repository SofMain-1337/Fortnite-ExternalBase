//#pragma once
//
//#include "../imgui/imgui_impl_dx9.h"
//#include "../imgui/imgui_impl_win32.h"
//#include "image.h"
//#include "icons.h"
//#include "fonts.h"
//
//#include <d3d9.h>
//#include <D3DX9.h>
//#include <tchar.h>
//#include <string>
//#include "../imgui/imgui.h"
//
//#pragma comment (lib, "d3dx9.lib")
//
//static LPDIRECT3D9              g_pD3D = nullptr;
//static LPDIRECT3DDEVICE9        g_pd3dDevice = nullptr;
//static bool                     g_DeviceLost = false;
//static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
//static D3DPRESENT_PARAMETERS    g_d3dpp = {};
//
//bool CreateDeviceD3D(HWND hWnd);
//void CleanupDeviceD3D();
//void ResetDevice();
//LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//
//namespace config {
//
//	int tab = 0;
//
//	namespace aimbot {
//		bool enable = false;
//		bool auto_fire = false;
//		bool friendly_fire = false;
//		float fov = 0;
//		float smooth = 0;
//	}
//	namespace silent {
//		bool enable = false;
//	}
//	namespace backtrack {
//		bool enable = false;
//		float max_backtrack = 0;
//	}
//	namespace ignores {
//		bool enable = false;
//		bool smoke = false;
//		bool flash = false;
//		bool jump = false;
//		bool walls = false;
//	}
//
//	const char* mode[] = {"Safety", "Normal", "Untrusted"};
//	static const char* current_mode = "Normal";
//
//}
//
//namespace self {
//
//	void load_image();
//
//}
//
//namespace fonts {
//
//	ImFont* zona = nullptr;
//	ImFont* bigzona = nullptr;
//	ImFont* smallzona = nullptr;
//
//}
//
//namespace img {
//
//	IDirect3DTexture9* aimbot = nullptr;
//	IDirect3DTexture9* visuals = nullptr;
//	IDirect3DTexture9* misc = nullptr;
//	IDirect3DTexture9* configs = nullptr;
//	IDirect3DTexture9* settings = nullptr;
//	IDirect3DTexture9* telegram = nullptr;
//	IDirect3DTexture9* background = nullptr;
//
//}