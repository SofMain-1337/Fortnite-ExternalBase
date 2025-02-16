//SIMPLE ASS SETTINGS HERE
#pragma once
namespace settings
{
	inline int width = GetSystemMetrics(SM_CXSCREEN);
	inline int height = GetSystemMetrics(SM_CYSCREEN);
	inline int screen_center_x = width / 2;
	inline int screen_center_y = height / 2;
	inline bool show_menu = true;
	inline int tab = 0;
	namespace aimbot
	{
		inline bool enable = false;
		inline bool show_fov = false;
		inline float fov = 160;
		inline float smoothness = 6;
		inline bool visible_check = false;
		inline static int current_key = VK_RBUTTON;
	}
	namespace visuals
	{
		inline bool enable = false;
		inline bool CorneredBox = false;
		inline bool fill_box = false;
		inline bool fill_wcbox = false;
		inline bool line = false;
		inline bool distance = false;
		inline bool Box = false;
		inline bool skeleton = false;
	}
	namespace Features
	{
		inline bool Watermark = true;
		inline bool info = false;
		inline bool crosshair = false;
		inline bool fpswater = true;
	}
	namespace radar
	{
		inline bool radar = false;
		inline bool cross = true;
		inline bool local_player = true;
		inline float range = 50.0f;
	}
	namespace Exploits
	{
		inline bool spinbot = false;
		inline float fovchanger_value = false;
		inline bool fovchanger = false;
		inline bool playerbackwards = false;
	}
	namespace trigger
	{
		inline static int current_key = VK_LBUTTON;
		inline bool Triggerbot = false;
		inline float TriggerDelay = 70;
	}
}