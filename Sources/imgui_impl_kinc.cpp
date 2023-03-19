// dear imgui: Platform Binding for Kinc

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#include <kinc/input/keyboard.h>
#include <kinc/input/mouse.h>
#include <kinc/system.h>
#include <kinc/window.h>

#include "imgui.h"
#include "imgui_impl_g4.h"
#include "imgui_impl_kinc.h"

// Data
static int g_Window = NULL;
static kinc_ticks_t g_Time = 0;
static bool g_MousePressed[5] = {false, false, false, false, false};
static bool g_MousePressedCurrently[5] = {false, false, false, false, false};
// static SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = {};
static char *g_ClipboardTextData = NULL;

static const char *ImGui_ImplKinc_GetClipboardText(void *) {
	return "";
}

static void ImGui_ImplKinc_SetClipboardText(void *, const char *text) {}

static void keyboard_key_down(int key_code, void *data) {
	ImGuiIO &io = ImGui::GetIO();
	switch (key_code) {
	case KINC_KEY_SHIFT:
		io.KeyShift = true;
		break;
	case KINC_KEY_CONTROL:
		io.KeyCtrl = true;
		break;
	case KINC_KEY_ALT:
	case KINC_KEY_ALT_GR:
		io.KeyAlt = true;
		break;
	default:
		IM_ASSERT(key_code >= 0 && key_code < IM_ARRAYSIZE(io.KeysDown));
		io.KeysDown[key_code] = true;
		break;
	}
}

static void keyboard_key_up(int key_code, void *data) {
	ImGuiIO &io = ImGui::GetIO();
	switch (key_code) {
	case KINC_KEY_SHIFT:
		io.KeyShift = false;
		break;
	case KINC_KEY_CONTROL:
		io.KeyCtrl = false;
		break;
	case KINC_KEY_ALT:
	case KINC_KEY_ALT_GR:
		io.KeyAlt = false;
		break;
	default:
		IM_ASSERT(key_code >= 0 && key_code < IM_ARRAYSIZE(io.KeysDown));
		io.KeysDown[key_code] = false;
		break;
	}
}

static void keyboard_key_press(unsigned character, void *data) {
	ImGuiIO &io = ImGui::GetIO();
	char text[2];
	text[0] = character;
	text[1] = 0;
	io.AddInputCharactersUTF8(text);
}

static void mouse_move(int window, int x, int y, int movement_x, int movement_y, void *data) {
	ImGuiIO &io = ImGui::GetIO();
	io.MousePos = ImVec2((float)x, (float)y);
}

static void mouse_press(int window, int button, int x, int y, void *data) {
	if (button < 5) {
		ImGuiIO &io = ImGui::GetIO();
		g_MousePressed[button] = true;
		g_MousePressedCurrently[button] = true;
	}
}

static void mouse_release(int window, int button, int x, int y, void *data) {
	if (button < 5) {
		ImGuiIO &io = ImGui::GetIO();
		g_MousePressedCurrently[button] = false;
	}
}

static void mouse_scroll(int window, int delta, void *data) {
	ImGuiIO &io = ImGui::GetIO();
	io.MouseWheel += delta;
}

static bool ImGui_ImplKinc_Init(int window) {
	g_Window = window;

	// Setup back-end capabilities flags
	ImGuiIO &io = ImGui::GetIO();
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;  // We can honor io.WantSetMousePos requests (optional, rarely used)
	io.BackendPlatformName = "imgui_impl_sdl";

	// Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
	io.KeyMap[ImGuiKey_Tab] = KINC_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = KINC_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = KINC_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = KINC_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = KINC_KEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = KINC_KEY_PAGE_UP;
	io.KeyMap[ImGuiKey_PageDown] = KINC_KEY_PAGE_DOWN;
	io.KeyMap[ImGuiKey_Home] = KINC_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = KINC_KEY_END;
	io.KeyMap[ImGuiKey_Insert] = KINC_KEY_INSERT;
	io.KeyMap[ImGuiKey_Delete] = KINC_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = KINC_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = KINC_KEY_SPACE;
	io.KeyMap[ImGuiKey_Enter] = KINC_KEY_RETURN;
	io.KeyMap[ImGuiKey_Escape] = KINC_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_KeyPadEnter] = KINC_KEY_RETURN;
	io.KeyMap[ImGuiKey_A] = KINC_KEY_A;
	io.KeyMap[ImGuiKey_C] = KINC_KEY_C;
	io.KeyMap[ImGuiKey_V] = KINC_KEY_V;
	io.KeyMap[ImGuiKey_X] = KINC_KEY_X;
	io.KeyMap[ImGuiKey_Y] = KINC_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = KINC_KEY_Z;

	io.SetClipboardTextFn = ImGui_ImplKinc_SetClipboardText;
	io.GetClipboardTextFn = ImGui_ImplKinc_GetClipboardText;
	io.ClipboardUserData = NULL;

	kinc_keyboard_set_key_down_callback(keyboard_key_down, NULL);
	kinc_keyboard_set_key_up_callback(keyboard_key_up, NULL);
	kinc_keyboard_set_key_press_callback(keyboard_key_press, NULL);

	kinc_mouse_set_move_callback(mouse_move, NULL);
	kinc_mouse_set_press_callback(mouse_press, NULL);
	kinc_mouse_set_release_callback(mouse_release, NULL);
	kinc_mouse_set_scroll_callback(mouse_scroll, NULL);

	/*g_MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	g_MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	g_MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
	g_MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
	g_MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
	g_MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
	g_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
	g_MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);*/

	(void)window;

	return true;
}

bool ImGui_ImplKinc_InitForG4(int window) {
	return ImGui_ImplKinc_Init(window);
}

void ImGui_ImplKinc_Shutdown() {
	g_Window = NULL;

	// Destroy last known clipboard data
	/*if (g_ClipboardTextData)
	    SDL_free(g_ClipboardTextData);
	g_ClipboardTextData = NULL;*/

	// Destroy SDL mouse cursors
	/*for (ImGuiMouseCursor cursor_n = 0; cursor_n < ImGuiMouseCursor_COUNT; cursor_n++)
	    SDL_FreeCursor(g_MouseCursors[cursor_n]);
	memset(g_MouseCursors, 0, sizeof(g_MouseCursors));*/
}

static void ImGui_ImplKinc_UpdateMousePosAndButtons() {
	ImGuiIO &io = ImGui::GetIO();

	// Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
	/*if (io.WantSetMousePos)
	    SDL_WarpMouseInWindow(g_Window, (int)io.MousePos.x, (int)io.MousePos.y);
	else
	    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);*/

	for (int i = 0; i < 5; ++i) {
		io.MouseDown[i] = g_MousePressed[i] || g_MousePressedCurrently[i]; // If a mouse press event came, always pass it as "mouse held this frame", so we
		                                                                   // don't miss click-release events that are shorter than 1 frame.
		g_MousePressed[i] = false;
	}

	/*#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
	    SDL_Window* focused_window = SDL_GetKeyboardFocus();
	    if (g_Window == focused_window)
	    {
	        // SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
	        // The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
	        int wx, wy;
	        SDL_GetWindowPosition(focused_window, &wx, &wy);
	        SDL_GetGlobalMouseState(&mx, &my);
	        mx -= wx;
	        my -= wy;
	        io.MousePos = ImVec2((float)mx, (float)my);
	    }

	    // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
	    // The function is only supported from SDL 2.0.4 (released Jan 2016)
	    bool any_mouse_button_down = ImGui::IsAnyMouseDown();
	    SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
	#else
	    if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
	        io.MousePos = ImVec2((float)mx, (float)my);
	#endif*/
}

static void ImGui_ImplKinc_UpdateMouseCursor() {
	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) return;

	/*ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
	{
	    // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
	    SDL_ShowCursor(SDL_FALSE);
	}
	else
	{
	    // Show OS mouse cursor
	    SDL_SetCursor(g_MouseCursors[imgui_cursor] ? g_MouseCursors[imgui_cursor] : g_MouseCursors[ImGuiMouseCursor_Arrow]);
	    SDL_ShowCursor(SDL_TRUE);
	}*/
}

static void ImGui_ImplKinc_UpdateGamepads() {
	/*ImGuiIO& io = ImGui::GetIO();
	memset(io.NavInputs, 0, sizeof(io.NavInputs));
	if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
	    return;

	// Get gamepad
	SDL_GameController* game_controller = SDL_GameControllerOpen(0);
	if (!game_controller)
	{
	    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
	    return;
	}

	// Update gamepad inputs
	#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (SDL_GameControllerGetButton(game_controller, BUTTON_NO) != 0) ? 1.0f : 0.0f; }
	#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn
	> 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; } const int thumb_dead_zone = 8000;           //
	SDL_gamecontroller.h suggests using this value. MAP_BUTTON(ImGuiNavInput_Activate,      SDL_CONTROLLER_BUTTON_A);               // Cross / A
	MAP_BUTTON(ImGuiNavInput_Cancel,        SDL_CONTROLLER_BUTTON_B);               // Circle / B
	MAP_BUTTON(ImGuiNavInput_Menu,          SDL_CONTROLLER_BUTTON_X);               // Square / X
	MAP_BUTTON(ImGuiNavInput_Input,         SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
	MAP_BUTTON(ImGuiNavInput_DpadLeft,      SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
	MAP_BUTTON(ImGuiNavInput_DpadRight,     SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
	MAP_BUTTON(ImGuiNavInput_DpadUp,        SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
	MAP_BUTTON(ImGuiNavInput_DpadDown,      SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
	MAP_BUTTON(ImGuiNavInput_FocusPrev,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
	MAP_BUTTON(ImGuiNavInput_FocusNext,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
	MAP_BUTTON(ImGuiNavInput_TweakSlow,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
	MAP_BUTTON(ImGuiNavInput_TweakFast,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
	MAP_ANALOG(ImGuiNavInput_LStickLeft,    SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
	MAP_ANALOG(ImGuiNavInput_LStickRight,   SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
	MAP_ANALOG(ImGuiNavInput_LStickUp,      SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
	MAP_ANALOG(ImGuiNavInput_LStickDown,    SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);

	io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
	#undef MAP_BUTTON
	#undef MAP_ANALOG*/
}

void ImGui_ImplKinc_NewFrame(int window) {
	ImGuiIO &io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? "
	                                 "e.g. ImGui_ImplOpenGL3_NewFrame().");

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	display_w = w = kinc_window_width(window);
	display_h = h = kinc_window_height(window);
	io.DisplaySize = ImVec2((float)w, (float)h);
	if (w > 0 && h > 0) io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

	// Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
	static double frequency = kinc_frequency();
	kinc_ticks_t current_time = kinc_timestamp();
	io.DeltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	ImGui_ImplKinc_UpdateMousePosAndButtons();
	ImGui_ImplKinc_UpdateMouseCursor();

	// Update game controllers (if enabled and available)
	ImGui_ImplKinc_UpdateGamepads();
}
