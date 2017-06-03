// ImGui GLFW binding with OpenGL
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// If your context is GL3/GL3 then prefer using the code in opengl3_example.
// You *might* use this code with a GL3/GL4 context but make sure you disable the programmable pipeline by calling "glUseProgram(0)" before ImGui::Render().
// We cannot do that from GL2 code because the function doesn't exist. 

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

#include "ImGui_ImplOrx.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <gl/GL.h>

#include <vector>
#include <string>

extern "C" {
#include <orx.h>
    }

//! Structs that contains the mapping between orx and ImGUI mouse buttons and the current state
struct MouseButtonState
    {
    //! Mouse button code in orx sytem
    orxMOUSE_BUTTON		m_Button;
    //! Mouse button code in GWEN system
    unsigned char		m_ImGUIButton;
    //! Last read pressure state
    bool				m_Pressed;
    };


static orxBITMAP *                      g_FontTexture = 0;
static void *                           g_ClipboardUserData = NULL;
static std::string                      g_ClipboardText;
static int                              g_ClipboardSize = 0;

static double                           g_Time = 0.0f;
static bool                             g_MousePressed[3] = { false, false, false };
static float                            g_MouseWheel = 0.0f;

#define MAX_IMGUI_MOUSE_BUTTONS         5
static MouseButtonState                 g_ButtonStates[MAX_IMGUI_MOUSE_BUTTONS];

//! Updates mouse state
static void UpdateMouse(ImGuiIO & io);
//! Updates keyboard state
static void UpdateKeyboard(ImGuiIO & io);

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_Render(void * pvViewport, ImDrawData* draw_data)
{
    orxVIEWPORT * pstViewport = (orxVIEWPORT *)pvViewport;

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Gets screen bitmap
    orxBITMAP * pstScreen = orxDisplay_GetScreenBitmap();
    // Restores screen as destination bitmap
    orxDisplay_SetDestinationBitmaps(&pstScreen, 1);
    // Restores screen bitmap clipping
    orxDisplay_SetBitmapClipping(orxDisplay_GetScreenBitmap(), 0, 0, orxF2U(fb_width), orxF2U(fb_height));

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
        const ImDrawList * cmd_list = draw_data->CmdLists[n];
        const ImDrawVert * vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx * idx_buffer = cmd_list->IdxBuffer.Data;

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
                {
                pcmd->UserCallback(cmd_list, pcmd);
                }
            else
                {
                orxCUSTOM_MESH customMesh = { 0 };
                customMesh.pstBitmap = (orxBITMAP *)pcmd->TextureId;
                customMesh.u32BitmapClipTLX = (int)pcmd->ClipRect.x;
                customMesh.u32BitmapClipTLY = (int)(fb_height - pcmd->ClipRect.w);
                customMesh.u32BitmapClipBRX = (int)(pcmd->ClipRect.z - pcmd->ClipRect.x);
                customMesh.u32BitmapClipBRY = (int)(pcmd->ClipRect.w - pcmd->ClipRect.y);
                customMesh.eSmoothing = orxDISPLAY_SMOOTHING_ON;
                customMesh.eBlendMode = orxDISPLAY_BLEND_MODE_ALPHA;
                customMesh.eDrawMode = orxDISPLAY_DRAW_MODE_TRIANGLES;
                customMesh.u32VertexNumber = cmd_list->VtxBuffer.size();
                customMesh.astVertexList = (orxDISPLAY_VERTEX *)vtx_buffer;
                customMesh.u32IndexesCount = cmd_list->IdxBuffer.size();
                customMesh.au16IndexList = idx_buffer;
                customMesh.u32ElementCount = pcmd->ElemCount;

                orxDisplay_DrawCustomMesh(&customMesh);
                }

            idx_buffer += pcmd->ElemCount;
            }
        }
}

//////////////////////////////////////////////////////////////////////////
static const char* ImGui_ImplOrx_GetClipboardText(void* user_data)
{
    return g_ClipboardText.data();
}

//////////////////////////////////////////////////////////////////////////
static void ImGui_ImplOrx_SetClipboardText(void* user_data, const char* text)
{
    g_ClipboardUserData = user_data;
    g_ClipboardText = text;
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplGlFw_KeyCallback(int key, int, int action, int mods)
{
/*
    ImGuiIO& io = ImGui::GetIO();
    if (action == GLFW_PRESS)
        io.KeysDown[key] = true;
    if (action == GLFW_RELEASE)
        io.KeysDown[key] = false;

    (void)mods; // Modifiers are not reliable across systems
    io.KeyCtrl = io.KeysDown[GLFW_KEY_LEFT_CONTROL] || io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
    io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT] || io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
    io.KeyAlt = io.KeysDown[GLFW_KEY_LEFT_ALT] || io.KeysDown[GLFW_KEY_RIGHT_ALT];
    io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER] || io.KeysDown[GLFW_KEY_RIGHT_SUPER];
*/
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_CharCallback(unsigned int c)
{
/*
    ImGuiIO& io = ImGui::GetIO();
    if (c > 0 && c < 0x10000)
        io.AddInputCharacter((unsigned short)c);
*/
}

//////////////////////////////////////////////////////////////////////////
orxBITMAP * InitFontTexture(ImGuiIO & io)
    {
    orxTEXTURE *pstTexture;
    orxBITMAP  *pstBitmap;
    
    unsigned char* pixels;
    int u32Width, u32Height;

    // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. 
    // If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
    io.Fonts->GetTexDataAsRGBA32(&pixels, &u32Width, &u32Height);
    
    // Creates background texture
    pstTexture = orxTexture_Create();
    pstBitmap = orxDisplay_CreateBitmap(u32Width, u32Height);
    orxDisplay_SetBitmapData(pstBitmap, pixels, u32Width * u32Height * sizeof(orxRGBA));
    orxTexture_LinkBitmap(pstTexture, pstBitmap, "Texture", orxTRUE);

    return pstBitmap;
    }

//////////////////////////////////////////////////////////////////////////
bool ImGui_ImplOrx_CreateDeviceObjects()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();

    // Store our identifier
    g_FontTexture = InitFontTexture(io);
    io.Fonts->TexID = (void *)g_FontTexture;

    return true;
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_InvalidateDeviceObjects()
{
    if (g_FontTexture)
    {
        orxDisplay_DeleteBitmap(g_FontTexture);
        ImGui::GetIO().Fonts->TexID = 0;
        g_FontTexture = 0;
    }
}

//////////////////////////////////////////////////////////////////////////
bool ImGui_ImplOrx_Init()
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeyMap[ImGuiKey_Tab] = orxKEYBOARD_KEY_TAB;                     // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
    io.KeyMap[ImGuiKey_LeftArrow] = orxKEYBOARD_KEY_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = orxKEYBOARD_KEY_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = orxKEYBOARD_KEY_UP;
    io.KeyMap[ImGuiKey_DownArrow] = orxKEYBOARD_KEY_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = orxKEYBOARD_KEY_PAGEUP;
    io.KeyMap[ImGuiKey_PageDown] = orxKEYBOARD_KEY_PAGEDOWN;
    io.KeyMap[ImGuiKey_Home] = orxKEYBOARD_KEY_HOME;
    io.KeyMap[ImGuiKey_End] = orxKEYBOARD_KEY_END;
    io.KeyMap[ImGuiKey_Delete] = orxKEYBOARD_KEY_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = orxKEYBOARD_KEY_BACKSPACE;
    io.KeyMap[ImGuiKey_Enter] = orxKEYBOARD_KEY_RETURN;
    io.KeyMap[ImGuiKey_Escape] = orxKEYBOARD_KEY_ESCAPE;
    io.KeyMap[ImGuiKey_A] = orxKEYBOARD_KEY_A;
    io.KeyMap[ImGuiKey_C] = orxKEYBOARD_KEY_C;
    io.KeyMap[ImGuiKey_V] = orxKEYBOARD_KEY_V;
    io.KeyMap[ImGuiKey_X] = orxKEYBOARD_KEY_X;
    io.KeyMap[ImGuiKey_Y] = orxKEYBOARD_KEY_Y;
    io.KeyMap[ImGuiKey_Z] = orxKEYBOARD_KEY_Z;

    io.RenderDrawListsFn = NULL/*ImGui_ImplOrx_RenderDrawLists*/;      // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
    io.SetClipboardTextFn = ImGui_ImplOrx_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplOrx_GetClipboardText;
    io.ClipboardUserData = /*g_Window*/NULL;

    g_ButtonStates[0] = { orxMOUSE_BUTTON_LEFT		, 0		, false };
    g_ButtonStates[1] = { orxMOUSE_BUTTON_RIGHT		, 1		, false };
    g_ButtonStates[2] = { orxMOUSE_BUTTON_MIDDLE	, 2		, false };
    g_ButtonStates[3] = { orxMOUSE_BUTTON_EXTRA_1	, 3		, false };
    g_ButtonStates[4] = { orxMOUSE_BUTTON_EXTRA_2	, 4		, false };


#ifdef _WIN32
    io.ImeWindowHandle = /*glfwGetWin32Window(g_Window)*/NULL;
#endif

    return true;
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_Shutdown()
{
    ImGui_ImplOrx_InvalidateDeviceObjects();
    ImGui::Shutdown();
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_NewFrame()
{
    if (!g_FontTexture)
        ImGui_ImplOrx_CreateDeviceObjects();

    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    orxFLOAT display_w, display_h;

    /* get screen size */
    orxDisplay_GetScreenSize(&display_w, &display_h);

    io.DisplaySize = ImVec2(display_w, display_h);
    io.DisplayFramebufferScale = ImVec2(1, 1);

    // Setup time step
    double current_time = orxSystem_GetSystemTime();
    io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
    g_Time = current_time;

    UpdateMouse(io);
    UpdateKeyboard(io);

    // Start the frame
    ImGui::NewFrame();
}

//////////////////////////////////////////////////////////////////////////
void UpdateMouse(ImGuiIO & io)
    {
    // Get current mouse position
    orxVECTOR pos;
    orxMouse_GetPosition(&pos);
    io.MousePos = ImVec2(pos.fX, pos.fY);

    // get delta from last orxMouse_GetPosition call
    orxVECTOR delta;
    orxMouse_GetMoveDelta(&delta);

    // test the wheel
//    if ((state.m_Button == orxMOUSE_BUTTON_WHEEL_UP) || (state.m_Button == orxMOUSE_BUTTON_WHEEL_DOWN))
        io.MouseWheel = orxMouse_GetWheelDelta();

    // poll for special keys
    for (int a=0; a<MAX_IMGUI_MOUSE_BUTTONS; a++)
        {
        MouseButtonState & state = g_ButtonStates[a];
        // check if it's presses
        bool pressed = orxMouse_IsButtonPressed(state.m_Button) == orxTRUE;
        // check if the state changed from last time
        if (pressed != state.m_Pressed)
            {
            // yes, then update new state...
            state.m_Pressed = pressed;
            // ...and tell it to the canvas!
            io.MouseDown[state.m_ImGUIButton] = state.m_Pressed;
            }
        }
    }

//////////////////////////////////////////////////////////////////////////
void UpdateKeyboard(ImGuiIO & io)
    {
    for (int key = orxKEYBOARD_KEY_A; key < orxKEYBOARD_KEY_NUMBER; key++)
        {
        bool pressed = orxKeyboard_IsKeyPressed((orxKEYBOARD_KEY)key) == orxTRUE;
        io.KeysDown[key] = pressed;
        }

    io.KeyCtrl = io.KeysDown[orxKEYBOARD_KEY_LCTRL] || io.KeysDown[orxKEYBOARD_KEY_RCTRL];
    io.KeyShift = io.KeysDown[orxKEYBOARD_KEY_LSHIFT] || io.KeysDown[orxKEYBOARD_KEY_RSHIFT];
    io.KeyAlt = io.KeysDown[orxKEYBOARD_KEY_LALT] || io.KeysDown[orxKEYBOARD_KEY_RALT];
    io.KeySuper = io.KeysDown[orxKEYBOARD_KEY_LSYSTEM] || io.KeysDown[orxKEYBOARD_KEY_RSYSTEM];
    }

