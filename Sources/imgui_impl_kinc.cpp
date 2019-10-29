// dear imgui: Platform Binding for Kinc

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#include <kinc/pch.h>

#include <kinc/input/keyboard.h>
#include <kinc/system.h>
#include <kinc/window.h>

#include "imgui.h"
#include "imgui_impl_kinc.h"
#include "imgui_impl_g4.h"

// Data
static int  g_Window = NULL;
static kinc_ticks_t g_Time = 0;
static bool         g_MousePressed[3] = { false, false, false };
//static SDL_Cursor*  g_MouseCursors[ImGuiMouseCursor_COUNT] = {};
static char*        g_ClipboardTextData = NULL;

static const char* ImGui_ImplSDL2_GetClipboardText(void*)
{
	return "";
}

static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
{
    
}

// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
// If you have multiple SDL events and some of them are not meant to be used by dear imgui, you may need to filter events based on their windowID field.
bool ImGui_ImplSDL2_ProcessEvent()
{
    /*ImGuiIO& io = ImGui::GetIO();
    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
        {
            if (event->wheel.x > 0) io.MouseWheelH += 1;
            if (event->wheel.x < 0) io.MouseWheelH -= 1;
            if (event->wheel.y > 0) io.MouseWheel += 1;
            if (event->wheel.y < 0) io.MouseWheel -= 1;
            return true;
        }
    case SDL_MOUSEBUTTONDOWN:
        {
            if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
            if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
            if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
            return true;
        }
    case SDL_TEXTINPUT:
        {
            io.AddInputCharactersUTF8(event->text.text);
            return true;
        }
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        {
            int key = event->key.keysym.scancode;
            IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
            io.KeysDown[key] = (event->type == SDL_KEYDOWN);
            io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
            io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
            io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
            io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
            return true;
        }
    }*/
    return false;
}

static bool ImGui_ImplSDL2_Init(int window)
{
    g_Window = window;

    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
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

    io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
    io.ClipboardUserData = NULL;

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

bool ImGui_ImplKinc_InitForG4(int window)
{
    return ImGui_ImplG4_Init(window);
}

void ImGui_ImplKinc_Shutdown()
{
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

static void ImGui_ImplSDL2_UpdateMousePosAndButtons()
{
    ImGuiIO& io = ImGui::GetIO();

    // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
    /*if (io.WantSetMousePos)
        SDL_WarpMouseInWindow(g_Window, (int)io.MousePos.x, (int)io.MousePos.y);
    else*/
        io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

    /*int mx, my;
    Uint32 mouse_buttons = SDL_GetMouseState(&mx, &my);
    io.MouseDown[0] = g_MousePressed[0] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
    io.MouseDown[1] = g_MousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
    io.MouseDown[2] = g_MousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
    g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;*/

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

static void ImGui_ImplSDL2_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;

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

static void ImGui_ImplSDL2_UpdateGamepads()
{
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
    #define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
    MAP_BUTTON(ImGuiNavInput_Activate,      SDL_CONTROLLER_BUTTON_A);               // Cross / A
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

void ImGui_ImplSDL2_NewFrame(int window)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
	display_w = w = kinc_window_width(window);
	display_h = h = kinc_window_height(window);
    io.DisplaySize = ImVec2((float)w, (float)h);
    if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

    // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
    static double frequency = kinc_frequency();
	kinc_ticks_t current_time = kinc_timestamp();
    io.DeltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    ImGui_ImplSDL2_UpdateMousePosAndButtons();
    ImGui_ImplSDL2_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    ImGui_ImplSDL2_UpdateGamepads();
}
