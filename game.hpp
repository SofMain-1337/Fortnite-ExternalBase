#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include <dwmapi.h>
#include <d3d9.h>
#include "includes.hpp"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
IDirect3D9Ex* p_object = NULL;
IDirect3DDevice9Ex* p_device = NULL;
D3DPRESENT_PARAMETERS p_params = { NULL };
MSG messager = { NULL };
HWND my_wnd = NULL;
HWND game_wnd = NULL;
DWORD processID;
HRESULT directx_init()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object))) exit(3);
	ZeroMemory(&p_params, sizeof(p_params));
	p_params.Windowed = TRUE;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.hDeviceWindow = my_wnd;
	p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferWidth = settings::width;
	p_params.BackBufferHeight = settings::height;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	p_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, my_wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device)))
	{
		p_object->Release();
		exit(4);
	}
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(my_wnd);
	ImGui_ImplDX9_Init(p_device);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = 0;


	ImFontConfig config;
	config.MergeMode = false;
	ImFont* impactFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 15.0f);
	config.MergeMode = true;
	config.PixelSnapH = true;
	io.Fonts->AddFontDefault();

	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 1.0f;
	style.WindowPadding = ImVec2(20.0f, 20.0f);
	style.WindowRounding = 11.5f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_None;
	style.ChildRounding = 20.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 17.39999961853027f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 3.400000095367432f);
	style.FrameRounding = 11.89999961853027f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.899999618530273f, 13.39999961853027f);
	style.ItemInnerSpacing = ImVec2(7.099999904632568f, 1.799999952316284f);
	style.CellPadding = ImVec2(12.10000038146973f, 9.199999809265137f);
	style.IndentSpacing = 0.0f;
	style.ColumnsMinSpacing = 8.699999809265137f;
	style.ScrollbarSize = 11.60000038146973f;
	style.ScrollbarRounding = 15.89999961853027f;
	style.GrabMinSize = 3.700000047683716f;
	style.GrabRounding = 20.0f;
	style.TabRounding = 9.800000190734863f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411764889955521f, 0.1019607856869698f, 0.1176470592617989f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1137254908680916f, 0.125490203499794f, 0.1529411822557449f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.6000000238418579f, 0.9647058844566345f, 0.0313725508749485f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1803921610116959f, 0.1882352977991104f, 0.196078434586525f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1529411822557449f, 0.1529411822557449f, 0.1529411822557449f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.1411764770746231f, 0.1647058874368668f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354080677f, 0.105882354080677f, 0.105882354080677f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.1294117718935013f, 0.1490196138620377f, 0.1921568661928177f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.125490203499794f, 0.2745098173618317f, 0.572549045085907f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0313725508749485f, 0.9490196108818054f, 0.843137264251709f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2666666805744171f, 0.2901960909366608f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 0.501960813999176f);
	p_object->Release();
	return S_OK;
}

void create_overlay()
{
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		0,
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		0,
		0,
		"SofMainFNBase",
		LoadIcon(0, IDI_APPLICATION)
	};
	ATOM rce = RegisterClassExA(&wcex);
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	my_wnd = gui::create_window_in_band(0, rce, L"SofMainFNBase", WS_POPUP, rect.left, rect.top, rect.right, rect.bottom, 0, 0, wcex.hInstance, 0, gui::ZBID_UIACCESS);
	SetWindowLong(my_wnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(my_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(my_wnd, &margin);
	ShowWindow(my_wnd, SW_SHOW);
	UpdateWindow(my_wnd);
}



void radar_range(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

void project_world_to_radar(Vector3 vOrigin, int& screenx, int& screeny, ImVec2 pos)
{
	Vector3 vAngle = cache::local_camera.rotation;
	auto fYaw = vAngle.y * M_PI / 180.0f;
	float dx = vOrigin.x - cache::local_camera.location.x;
	float dy = vOrigin.y - cache::local_camera.location.y;
	float fsin_yaw = sinf(fYaw);
	float fminus_cos_yaw = -cosf(fYaw);
	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	x = -x;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;
	float range = (float)(settings::radar::range / 5) * 1000.f;

	radar_range(&x, &y, range);

	ImVec2 DrawPos = ImVec2(pos.x, pos.y);
	ImVec2 DrawSize = ImVec2(200.0f, 200.0f);
	float radarRadius = 200.0f / 2;
	float distance = sqrt(pow(screenx - DrawPos.x, 2) + pow(screeny - DrawPos.y, 2));

	if (distance > radarRadius)
	{
		float angle = atan2(screeny - DrawPos.y, screenx - DrawPos.x);
		screenx = static_cast<int>(DrawPos.x + radarRadius * cos(angle));
		screeny = static_cast<int>(DrawPos.y + radarRadius * sin(angle));
	}

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;
	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;
	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}


void aimbot(uintptr_t target_mesh)
{
	if (settings::aimbot::visible_check)
	{
		if (!target_mesh || !is_visible(target_mesh)) // Clarified visibility check condition
			return;
	}
	else
	{

	}
	Vector3 head3d = get_entity_bone(target_mesh, 110);
	Vector2 head2d = project_world_to_screen(head3d);
	Vector2 target{};
	if (head2d.x != 0)
	{
		if (head2d.x > settings::screen_center_x)
		{
			target.x = -(settings::screen_center_x - head2d.x);
			target.x /= settings::aimbot::smoothness;
			if (target.x + settings::screen_center_x > settings::screen_center_x * 2) target.x = 0;
		}
		if (head2d.x < settings::screen_center_x)
		{
			target.x = head2d.x - settings::screen_center_x;
			target.x /= settings::aimbot::smoothness;
			if (target.x + settings::screen_center_x < 0) target.x = 0;
		}
	}
	if (head2d.y != 0)
	{
		if (head2d.y > settings::screen_center_y)
		{
			target.y = -(settings::screen_center_y - head2d.y);
			target.y /= settings::aimbot::smoothness;
			if (target.y + settings::screen_center_y > settings::screen_center_y * 2) target.y = 0;
		}
		if (head2d.y < settings::screen_center_y)
		{
			target.y = head2d.y - settings::screen_center_y;
			target.y /= settings::aimbot::smoothness;
			if (target.y + settings::screen_center_y < 0) target.y = 0;
		}
	}
	input::move_mouse(target.x, target.y);
}

void draw_cornered_box(int x, int y, int w, int h, const ImColor color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x, y + (h / 3)), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y), ImVec2(x + (w / 3), y), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3), y), ImVec2(x + w, y), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + (h / 3)), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h - (h / 3)), ImVec2(x, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x, y + h), ImVec2(x + (w / 3), y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w - (w / 3), y + h), ImVec2(x + w, y + h), color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(x + w, y + h - (h / 3)), ImVec2(x + w, y + h), color, thickness);
}



void Box(int X, int Y, int W, int H, const ImColor color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)X, (float)Y }, ImVec2{ (float)(X + W), (float)Y }, color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)(X + W), (float)Y }, ImVec2{ (float)(X + W), (float)(Y + H) }, color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)X, (float)(Y + H) }, ImVec2{ (float)(X + W), (float)(Y + H) }, color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)X, (float)Y }, ImVec2{ (float)X, (float)(Y + H) }, color, thickness);
}

void draw_filled_rect(int x, int y, int w, int h, const ImColor color)
{
	ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}

void draw_line(Vector2 target, const ImColor color)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(settings::screen_center_x, settings::height), ImVec2(target.x, target.y), color, 0.1f);
}

void draw_distance(Vector2 location, float distance, const ImColor color)
{
	char dist[64];
	sprintf_s(dist, "%.fm", distance);
	ImVec2 text_size = ImGui::CalcTextSize(dist);
	ImGui::GetForegroundDrawList()->AddText(ImVec2(location.x - text_size.x / 2, location.y - text_size.y / 2), color, dist);
}

static ImVec2 radarSize = { 200, 200 };
static ImVec2 radarPos = { 10, 35 };
static int RedDotSize = 3;

void Radar()
{
	if (settings::radar::radar)
	{
		radarPos.x = 30;
		radarPos.y = (ImGui::GetIO().DisplaySize.y / 4) - (radarSize.y / 2) + 180;
		ImGui::SetNextWindowSize(radarSize);
		ImGui::SetNextWindowPos(radarPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(10, 10, 10, 155));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("##Radar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		{
			ImVec2 winpos = radarPos;
			ImVec2 winsize = radarSize;
			if (settings::radar::cross)
			{
				ImGui::GetWindowDrawList()->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y), ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y), ImGui::GetColorU32(ImGuiCol_Border), 1.0f);
				ImGui::GetWindowDrawList()->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5f), ImGui::GetColorU32(ImGuiCol_Border), 1.0f);
			}

			if (settings::radar::local_player)
			{

				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f), 2.0f, ImColor(255, 255, 255, 255), 64);
			}
			ImColor esp_col = ImColor(255, 0, 0, 255);
			float dotsize = RedDotSize;
			if (dotsize < 1.0f) dotsize = 1.0f;
			if (dotsize > 10.0f) dotsize = 10.0f;
			int ScreenX, ScreenY = 0;
			for (int i = 0; i < cache::player_count; ++i)
			{
				uintptr_t player_state = read<uintptr_t>(cache::player_array + (i * sizeof(uintptr_t)));
				int player_team_id = read<int>(player_state + TEAM_INDEX);
				if (player_team_id == cache::my_team_id) continue;

				uintptr_t pawn_private = read<uintptr_t>(player_state + PAWN_PRIVATE);
				if (!pawn_private || pawn_private == cache::local_pawn) continue;

				uintptr_t mesh = read<uintptr_t>(pawn_private + MESH);
				if (!mesh) continue;

				Vector3 bottom3d = get_entity_bone(mesh, 0);
				Vector2 bottom2d = project_world_to_screen(bottom3d);

				project_world_to_radar(bottom3d, ScreenX, ScreenY, winpos);

				if (ScreenX < winpos.x) ScreenX = static_cast<int>(winpos.x);
				if (ScreenX > winpos.x + winsize.x) ScreenX = static_cast<int>(winpos.x + winsize.x);
				if (ScreenY < winpos.y) ScreenY = static_cast<int>(winpos.y);
				if (ScreenY > winpos.y + winsize.y) ScreenY = static_cast<int>(winpos.y + winsize.y);

				ImGui::GetWindowDrawList()->AddCircleFilled(ImVec2(ScreenX, ScreenY), dotsize, esp_col, 64);
			}
		}


	}

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}


void Fpswtr() {

	constexpr float fontSize = 24.0f;
	constexpr ImVec2 watermarkPos = ImVec2(10.0f, 5.0f);

	static float lastTime = 0.0f;
	static int fps = 0;

	float currentTime = ImGui::GetTime();
	if (currentTime - lastTime >= 1.0f) {
		fps = static_cast<int>(ImGui::GetIO().Framerate); 
		lastTime = currentTime;
	}

	char fpsText[32];
	snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", ImGui::GetIO().Framerate);
	ImVec2 fpsSize = ImGui::CalcTextSize(fpsText);
	ImVec2 fpsPos = ImVec2(watermarkPos.x + 3.0f, watermarkPos.y + 3.0f);

}



#include "Keybinds.h"

#include <chrono>

using namespace std::chrono;

void infoo()
{
	static auto lastTime = std::chrono::high_resolution_clock::now();
	static int frameCount = 0;

	auto currentTime = std::chrono::high_resolution_clock::now();
	frameCount++;

	static float fps = 0.0f;
	if (std::chrono::duration<float>(currentTime - lastTime).count() >= 1.0f) {
		fps = static_cast<float>(frameCount) / std::chrono::duration<float>(currentTime - lastTime).count();
		frameCount = 0;
		lastTime = currentTime;
	}
	const char* watermarkText = "[Status: Undetected] - [Developer: SofMain] - [Free / Public ]";
	char fullText[512];
	snprintf(fullText, sizeof(fullText), "%s | FPS: %.2f", watermarkText, fps);
	ImGui::Begin("Info", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImVec2 position = ImVec2(35, 30);
	ImGui::SetCursorPos(position);
	ImGui::TextUnformatted(fullText);
}

ImVec2 GetWatermarkSize() {
	ImVec2 position = ImVec2(45.0f, 60.0f);
	constexpr const char* text = "SofMainFNBase [Free] [Public]";

	ImVec2 textSize = ImGui::CalcTextSize(text);
	return ImVec2(position.x, position.y + textSize.y);
}

void DrawCrosshair(ImVec2 center, ImVec4 color, float size) {
	ImGui::GetForegroundDrawList()->AddLine(
		ImVec2(center.x - size, center.y), ImVec2(center.x + size, center.y),
		ImGui::ColorConvertFloat4ToU32(color), 2.0f
	);
	ImGui::GetForegroundDrawList()->AddLine(
		ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size),
		ImGui::ColorConvertFloat4ToU32(color), 2.0f
	);
}


void game_loop()
{
	cache::uworld = read<uintptr_t>(sofmainisud + UWORLD);
	cache::game_instance = read<uintptr_t>(cache::uworld + GAME_INSTANCE);
	cache::local_players = read<uintptr_t>(read<uintptr_t>(cache::game_instance + LOCAL_PLAYERS));
	cache::player_controller = read<uintptr_t>(cache::local_players + PLAYER_CONTROLLER);
	cache::local_pawn = read<uintptr_t>(cache::player_controller + LOCAL_PAWN);
	if (cache::local_pawn != 0)
	{
		cache::root_component = read<uintptr_t>(cache::local_pawn + ROOT_COMPONENT);
		cache::relative_location = read<Vector3>(cache::root_component + RELATIVE_LOCATION);
		cache::player_state = read<uintptr_t>(cache::local_pawn + PLAYER_STATE);
		cache::my_team_id = read<int>(cache::player_state + TEAM_INDEX);
	}
	cache::game_state = read<uintptr_t>(cache::uworld + GAME_STATE);
	cache::player_array = read<uintptr_t>(cache::game_state + PLAYER_ARRAY);
	cache::player_count = read<int>(cache::game_state + (PLAYER_ARRAY + sizeof(uintptr_t)));
	cache::closest_distance = FLT_MAX;
	cache::closest_mesh = NULL;
	for (int i = 0; i < cache::player_count; i++)
	{
		uintptr_t player_state = read<uintptr_t>(cache::player_array + (i * sizeof(uintptr_t)));
		if (!player_state) continue;
		int player_team_id = read<int>(player_state + TEAM_INDEX);
		if (player_team_id == cache::my_team_id) continue;
		uintptr_t pawn_private = read<uintptr_t>(player_state + PAWN_PRIVATE);
		if (!pawn_private) continue;
		if (pawn_private == cache::local_pawn) continue;
		uintptr_t mesh = read<uintptr_t>(pawn_private + MESH);
		if (!mesh) continue;
		Vector3 head3d = get_entity_bone(mesh, 110);
		Vector2 head2d = project_world_to_screen(Vector3(head3d.x, head3d.y, head3d.z + 20)); //fixed the box
		Vector3 bottom3d = get_entity_bone(mesh, 0);
		Vector2 bottom2d = project_world_to_screen(bottom3d);
		float box_height = abs(head2d.y - bottom2d.y);
		float box_width = box_height * 0.50f;
		float distance = cache::relative_location.distance(bottom3d) / 100;
		if (settings::visuals::enable)
		{
			if (settings::visuals::Box)
			{
				if (is_visible(mesh))
				{
					Box(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(36, 207, 2, 250), 1);
				}
				else
				{
					Box(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(250, 0, 0, 250), 1);
				}
				if (settings::visuals::fill_box) draw_filled_rect(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(0, 0, 0, 50));
			}
		}
		if (settings::visuals::enable)
		{
			if (settings::visuals::CorneredBox)
			{
				if (is_visible(mesh))
				{
					draw_cornered_box(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(36, 207, 2, 250), 1);
				}
				else
				{
					draw_cornered_box(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(250, 0, 0, 250), 1);
				}
				if (settings::visuals::fill_box) draw_filled_rect(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(0, 0, 0, 50));
			}
			if (settings::visuals::line)
			{
				if (is_visible(mesh))
				{
					draw_line(bottom2d, ImColor(36, 207, 2, 250));
				}
				else
				{
					draw_line(bottom2d, ImColor(250, 0, 0, 250));
				}
			}
			if (settings::visuals::distance)
			{
				draw_distance(bottom2d, distance, ImColor(250, 250, 250, 250));
			}
			if (settings::visuals::fill_wcbox)
			{
				if (is_visible(mesh))
				{
					draw_filled_rect(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(36, 207, 2, 100));
				}
				else
				{
					draw_filled_rect(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(250, 0, 0, 100));
				}
			}	
			if (settings::radar::radar)
			{
				Radar();
			}
			if (settings::visuals::skeleton) {

				std::vector<Vector3> bones = {
			get_entity_bone(mesh, 66),  // neck
			get_entity_bone(mesh, 9),   // left shoulder
			get_entity_bone(mesh, 10),  // left elbow
			get_entity_bone(mesh, 11),  // left hand
			get_entity_bone(mesh, 38),  // right shoulder
			get_entity_bone(mesh, 39),  // right elbow
			get_entity_bone(mesh, 40),  // right hand
			get_entity_bone(mesh, 2),   // pelvis
			get_entity_bone(mesh, 71),  // left hip
			get_entity_bone(mesh, 72),  // left knee
			get_entity_bone(mesh, 78),  // right hip
			get_entity_bone(mesh, 79),  // right knee
			get_entity_bone(mesh, 75),  // left foot
			get_entity_bone(mesh, 82),  // right foot
			get_entity_bone(mesh, 67)   // head
				};

				std::vector<Vector2> screenPositions(bones.size());
				for (size_t i = 0; i < bones.size(); ++i) {
					screenPositions[i] = project_world_to_screen(bones[i]);
				}

				if (is_visible(mesh))
				{
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[4].x, screenPositions[4].y), ImColor(36, 207, 2, 250), 2.f);   // left shoulder to right shoulder
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[2].x, screenPositions[2].y), ImColor(36, 207, 2, 250), 2.f);   // left shoulder to left elbow
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[2].x, screenPositions[2].y), ImVec2(screenPositions[3].x, screenPositions[3].y), ImColor(36, 207, 2, 250), 2.f);   // left elbow to left hand
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[4].x, screenPositions[4].y), ImVec2(screenPositions[5].x, screenPositions[5].y), ImColor(36, 207, 2, 250), 2.f);   // right shoulder to right elbow
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[5].x, screenPositions[5].y), ImVec2(screenPositions[6].x, screenPositions[6].y), ImColor(36, 207, 2, 250), 2.f);   // right elbow to right hand
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[0].x, screenPositions[0].y), ImVec2(screenPositions[7].x, screenPositions[7].y), ImColor(36, 207, 2, 250), 2.f);   // neck to pelvis
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[8].x, screenPositions[8].y), ImColor(36, 207, 2, 250), 2.f);   // pelvis to left hip
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[8].x, screenPositions[8].y), ImVec2(screenPositions[9].x, screenPositions[9].y), ImColor(36, 207, 2, 250), 2.f);   // left hip to left knee
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[9].x, screenPositions[9].y), ImVec2(screenPositions[12].x, screenPositions[12].y), ImColor(36, 207, 2, 250), 2.f);  // left knee to left foot
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[10].x, screenPositions[10].y), ImColor(36, 207, 2, 250), 2.f);  // pelvis to right hip
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[10].x, screenPositions[10].y), ImVec2(screenPositions[11].x, screenPositions[11].y), ImColor(36, 207, 2, 250), 2.f); // right hip to right knee
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[11].x, screenPositions[11].y), ImVec2(screenPositions[13].x, screenPositions[13].y), ImColor(36, 207, 2, 250), 2.f); // right knee to right foot

				}
				else
				{
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[4].x, screenPositions[4].y), ImColor(250, 0, 0, 250), 2.f);   // left shoulder to right shoulder
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[2].x, screenPositions[2].y), ImColor(250, 0, 0, 250), 2.f);   // left shoulder to left elbow
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[2].x, screenPositions[2].y), ImVec2(screenPositions[3].x, screenPositions[3].y), ImColor(250, 0, 0, 250), 2.f);   // left elbow to left hand
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[4].x, screenPositions[4].y), ImVec2(screenPositions[5].x, screenPositions[5].y), ImColor(250, 0, 0, 250), 2.f);   // right shoulder to right elbow
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[5].x, screenPositions[5].y), ImVec2(screenPositions[6].x, screenPositions[6].y), ImColor(250, 0, 0, 250), 2.f);   // right elbow to right hand
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[0].x, screenPositions[0].y), ImVec2(screenPositions[7].x, screenPositions[7].y), ImColor(250, 0, 0, 250), 2.f);   // neck to pelvis
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[8].x, screenPositions[8].y), ImColor(250, 0, 0, 250), 2.f);   // pelvis to left hip
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[8].x, screenPositions[8].y), ImVec2(screenPositions[9].x, screenPositions[9].y), ImColor(250, 0, 0, 250), 2.f);   // left hip to left knee
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[9].x, screenPositions[9].y), ImVec2(screenPositions[12].x, screenPositions[12].y), ImColor(250, 0, 0, 250), 2.f);  // left knee to left foot
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[10].x, screenPositions[10].y), ImColor(250, 0, 0, 250), 2.f);  // pelvis to right hip
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[10].x, screenPositions[10].y), ImVec2(screenPositions[11].x, screenPositions[11].y), ImColor(250, 0, 0, 250), 2.f); // right hip to right knee
					ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[11].x, screenPositions[11].y), ImVec2(screenPositions[13].x, screenPositions[13].y), ImColor(250, 0, 0, 250), 2.f); // right knee to right foot
				}

			}
		}
		double dx = head2d.x - settings::screen_center_x;
		double dy = head2d.y - settings::screen_center_y;
		float dist = sqrtf(dx * dx + dy * dy);
		if (dist <= settings::aimbot::fov && dist < cache::closest_distance)
		{
			cache::closest_distance = dist;
			cache::closest_mesh = mesh;
		}
	}
	if (settings::aimbot::enable)
	{
		if (GetAsyncKeyState(settings::aimbot::current_key)) aimbot(cache::closest_mesh);
	}
	if (settings::Features::crosshair)
	{
		ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float size = 10.0f;
		DrawCrosshair(center, color, size);
	}
	if (settings::Features::info) {


		infoo();

	}
	if (settings::Exploits::spinbot)

	{

		auto Mesh = read<uintptr_t>(cache::local_pawn + MESH);

		static auto Cached = read<uintptr_t>(Mesh + 0x150);

		if (GetAsyncKeyState(VK_LBUTTON)) {

			write(Mesh + 0x150, Vector3(1, rand() % 361, 1));

		}

		else write(Mesh + 0x150, Cached);

	}

	if (settings::Exploits::playerbackwards)
	{
		auto Mesh = read<uintptr_t>(cache::local_pawn + MESH);

		static bool hasTurned = false;
		static auto Cached = read<Vector3>(Mesh + 0x150);

		if (!hasTurned)
		{
			Vector3 currentRotation = read<Vector3>(Mesh + 0x150);
			currentRotation.y += 180;
			if (currentRotation.y > 360)
				currentRotation.y -= 360;
			else if (currentRotation.y < 0)
				currentRotation.y += 360;

			write(Mesh + 0x150, currentRotation);
			hasTurned = true;
		}
	}
	bool fovreset = true;
	if (settings::Exploits::fovchanger)

	{
		fovreset = false;

		write(cache::player_controller + 0x2850, settings::Exploits::fovchanger_value);

		write(cache::player_controller + 0x2854, settings::Exploits::fovchanger_value);

	}

	else if (!fovreset)

	{

		write(cache::player_controller + 0x2800, settings::Exploits::fovchanger_value);

		write(cache::player_controller + 0x2804, settings::Exploits::fovchanger_value);

		fovreset = true;
	}
	if (settings::Features::fpswater) {


		Fpswtr();

	}
	if (settings::Features::fpswater)
	{
		char fpsText[32];
		snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", ImGui::GetIO().Framerate);
		ImVec2 position = ImVec2(45.0f, 60.0f);
		ImColor fpsColor = ImColor(0.2f, 0.6f, 1.0f, 1.0f);
		ImGui::GetForegroundDrawList()->AddText(position, fpsColor, fpsText);
	}

	if (settings::Features::Watermark) {
		ImVec2 position = ImVec2(30, 30);
		ImColor color = ImColor(0.2f, 0.6f, 1.0f, 1.0f);
		const char* text = "SofMainFNBase [Free] [Public]";
		ImGui::GetForegroundDrawList()->AddText(position, color, text);
	}
}





void render_menu()
{

	if (settings::aimbot::show_fov) ImGui::GetForegroundDrawList()->AddCircle(ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2), settings::aimbot::fov, ImColor(250, 250, 250, 250), 100, 1.0f);
	if (GetAsyncKeyState(VK_INSERT) & 1) settings::show_menu = !settings::show_menu;
	if (settings::show_menu)
	{
		ImGui::SetNextWindowSize({ 850, 650 });
		ImGui::Begin("SofMainFNBase [Public] - [Free] - [discord.gg/KCfyvNCZGA]", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		if (ImGui::Button("Aimbot", { 200, 25 })) settings::tab = 0;
		ImGui::SameLine();
		if (ImGui::Button("Visuals", { 200, 25 })) settings::tab = 1;
		ImGui::SameLine();
		if (ImGui::Button("Exploits", { 200, 25 })) settings::tab = 2;
		ImGui::SameLine();
		if (ImGui::Button("Misc", { 200, 25 })) settings::tab = 3;
		switch (settings::tab)
		{
			case 0:
			{
				ImGui::Checkbox("Enable Mouse Aimbot", &settings::aimbot::enable);
				if (settings::aimbot::enable)
				{
					//ImGui::Text("Mouse Smooth");
					ImGui::SliderFloat("..", &settings::aimbot::smoothness, 1, 10, "Smoothness: %.2f");


				}
				ImGui::Checkbox("Visible Check", &settings::aimbot::visible_check);
				ImGui::Checkbox("Show Fov", &settings::aimbot::show_fov);
				if (settings::aimbot::show_fov)
				{
					//ImGui::Text("Fov Size");
					ImGui::SliderFloat(".", &settings::aimbot::fov, 50, 300, "Fov: %.2f");
				}
				ImGui::Text("Aimbot Key");

				HotkeyButton(settings::aimbot::current_key, ChangeKey, keystatus);
				
				break;
			}
			case 1:
			{
				ImGui::Checkbox("Enable", &settings::visuals::enable);
				ImGui::Checkbox("Cornered Box", &settings::visuals::CorneredBox);
				//ImGui::SameLine();
				ImGui::Checkbox("Fill Box", &settings::visuals::fill_box);
				ImGui::Checkbox("Fill Box WColor", &settings::visuals::fill_wcbox);
				ImGui::Checkbox("Skeleton", &settings::visuals::skeleton);
				ImGui::Checkbox("Box", &settings::visuals::Box);
				ImGui::Checkbox("Line", &settings::visuals::line);
				ImGui::Checkbox("Distance", &settings::visuals::distance);
				break;
			}
			case 2:
			{
				ImGui::Text("Just to let you know lil paster, the following exploits are all detected as fuck! Have fun getting banned in less than 1 hour");
				ImGui::Checkbox("Spin Bot", &settings::Exploits::spinbot);
				ImGui::Checkbox("Fov Changer", &settings::Exploits::fovchanger);
				if (settings::Exploits::fovchanger)
				{
					ImGui::SliderFloat("##FovValue", &settings::Exploits::fovchanger_value, 400, 500, "FovValue: %.2f");
				}
				ImGui::Checkbox("Player Backwards", &settings::Exploits::playerbackwards);

				break;
			}
			case 3:
			{
				
				ImGui::Checkbox("WaterMark", &settings::Features::Watermark);
				ImGui::Checkbox("Overlay FPS", &settings::Features::fpswater);
				ImGui::Checkbox("Info", &settings::Features::info);
				ImGui::Checkbox("Crosshair", &settings::Features::crosshair);
				ImGui::Checkbox("Radar", &settings::radar::radar);
				if (ImGui::Button("Unload", { 120, 20 })) exit(0);
				break;
			}
		}
		ImGui::End();
	}
}

HWND get_process_wnd(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bresult = EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL
	{
		auto pparams = (std::pair<HWND, uint32_t>*)(lparam);
		uint32_t processid = 0;
		if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processid)) && processid == pparams->second)
		{
			SetLastError((uint32_t)-1);
			pparams->first = hwnd;
			return FALSE;
		}
		return TRUE;
	}, (LPARAM)&params);
	if (!bresult && GetLastError() == -1 && params.first) return params.first;
	return 0;
}

WPARAM render_loop()
{
	static RECT old_rc;
	ZeroMemory(&messager, sizeof(MSG));
	while (messager.message != WM_QUIT)
	{
		if (PeekMessage(&messager, my_wnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messager);
			DispatchMessage(&messager);
		}
		if (game_wnd == NULL) exit(0);
		HWND active_wnd = GetForegroundWindow();

		//THE SHIT THAT IS HIGHLIGHTED OUT IS WHEN U ALT + TAB FOR THE OVERLAY NOT TO LAG AND HAVE 30 FPS!

		//if (active_wnd == game_wnd)
		//{
		//	HWND target_wnd = GetWindow(active_wnd, GW_HWNDPREV);
		//	SetWindowPos(my_wnd, target_wnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//	}
	//	else
	//	{
	//		game_wnd = get_process_wnd(handler::process_id);
		//	Sleep(250);
	//	}
		RECT rc;
		POINT xy;
		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		rc.left = xy.x;
		rc.top = xy.y;
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;
		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			settings::width = rc.right;
			settings::height = rc.bottom;
			p_params.BackBufferWidth = settings::width;
			p_params.BackBufferHeight = settings::height;
			SetWindowPos(my_wnd, (HWND)0, xy.x, xy.y, settings::width, settings::height, SWP_NOREDRAW);
			p_device->Reset(&p_params);
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		game_loop();
		render_menu();
		ImGui::EndFrame();
		p_device->SetRenderState(D3DRS_ZENABLE, false);
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		p_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			p_device->EndScene();
		}
		HRESULT result = p_device->Present(0, 0, 0, 0);
		if (result == D3DERR_DEVICELOST && p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_device->Reset(&p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (p_device != 0)
	{
		p_device->EndScene();
		p_device->Release();
	}
	if (p_object != 0) p_object->Release();
	DestroyWindow(my_wnd);
	return messager.wParam;
}
