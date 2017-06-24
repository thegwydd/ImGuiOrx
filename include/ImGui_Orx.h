// ImGui Orx binding with OpenGL

#ifndef __IMGUI_ORX_HEADER__
#define __IMGUI_ORX_HEADER__

#include "../imgui/imgui.h"

// Initializes ImGui Orx system
IMGUI_API bool        ImGui_Orx_Init();

// Deinitializes ImGui Orx system
IMGUI_API void        ImGui_Orx_Shutdown();

// Initializes a new frame data. To call when starting rendering a new frame, before creating contents.
IMGUI_API void        ImGui_Orx_NewFrame();

// Renders the current frame To call after contents has been created
IMGUI_API void        ImGui_Orx_Render(void * pvViewport, ImDrawData* draw_data);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_Orx_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_Orx_CreateDeviceObjects();

#endif //__IMGUI_ORX_HEADER__
