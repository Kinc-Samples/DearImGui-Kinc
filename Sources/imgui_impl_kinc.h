// dear imgui: Platform Binding for Kinc

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#pragma once

IMGUI_IMPL_API bool     ImGui_ImplKinc_InitForG4(int window);
IMGUI_IMPL_API void     ImGui_ImplKinc_Shutdown();
IMGUI_IMPL_API void     ImGui_ImplKinc_NewFrame(int window);
IMGUI_IMPL_API bool     ImGui_ImplKinc_ProcessEvent();
