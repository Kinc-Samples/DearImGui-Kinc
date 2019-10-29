// dear imgui: Renderer for G4

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#pragma once

IMGUI_IMPL_API bool     ImGui_ImplG4_Init(int window);
IMGUI_IMPL_API void     ImGui_ImplG4_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplG4_NewFrame();
IMGUI_IMPL_API void     ImGui_ImplG4_RenderDrawData(ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_IMPL_API void     ImGui_ImplG4_InvalidateDeviceObjects();
IMGUI_IMPL_API bool     ImGui_ImplG4_CreateDeviceObjects();
