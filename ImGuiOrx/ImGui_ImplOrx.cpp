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

extern "C" {
#include <orx.h>
}


static orxBITMAP * g_FontTexture = 0;

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_Render_CmdList(orxVIEWPORT * pstViewport, const ImDrawList * cmd_list, int fb_width, int fb_height)
    {
    const ImDrawVert * vtx_buffer = cmd_list->VtxBuffer.Data;
    const ImDrawIdx * idx_buffer = cmd_list->IdxBuffer.Data;

    int last_used_index = 0;
    int num_vertices = cmd_list->IdxBuffer.size();

    // orxDisplay_DrawMesh internally uses GL_TRIANGLE_STRIP so organize texture mapping for a triangle strip VBO
    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
        const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
        if (pcmd->UserCallback)
            {
            pcmd->UserCallback(cmd_list, pcmd);
            }
        else
            {
            // build mesh vertex list
            std::vector<orxDISPLAY_VERTEX> vertexes;
            vertexes.resize(pcmd->ElemCount);

            for (unsigned int mv = 0; mv < pcmd->ElemCount; mv++)
                {
                int vertex_index = *(idx_buffer + mv);
                ImDrawVert * cur_vtx_buffer = cmd_list->VtxBuffer.Data + vertex_index;
                orxDISPLAY_VERTEX * pstVertex = &vertexes[mv];

                pstVertex->stRGBA.u32RGBA = cur_vtx_buffer->col;
                pstVertex->fX = cur_vtx_buffer->pos.x;
                pstVertex->fY = cur_vtx_buffer->pos.y;

//                ImGui_ImplOrx_Translate(pstViewport, pstVertex->fX, pstVertex->fY);

                pstVertex->fU = cur_vtx_buffer->uv.x;
                pstVertex->fV = cur_vtx_buffer->uv.y;
                }

            orxBITMAP * pstBitmap = (orxBITMAP *)pcmd->TextureId;
            orxDisplay_SetBitmapClipping(pstBitmap, (int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));

//            orxDisplay_DrawMesh(pstBitmap, orxDISPLAY_SMOOTHING_ON, orxDISPLAY_BLEND_MODE_ALPHA, pcmd->ElemCount, &vertexes[0]);
            orxDisplay_DrawCustomMesh(pstBitmap, orxDISPLAY_SMOOTHING_ON, orxDISPLAY_BLEND_MODE_ALPHA, orxDISPLAY_DRAW_MODE_TRIANGLES, pcmd->ElemCount, &vertexes[0]);
            }

        idx_buffer += pcmd->ElemCount;
        }
    }


//////////////////////////////////////////////////////////////////////////
// This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure)
// If text or lines are blurry when integrating ImGui in your engine:
// - in your Render function, try translating your projection matrix by (0.5f,0.5f) or (0.375f,0.375f)
void ImGui_ImplOrx_Render(void * pvViewport, ImDrawData* draw_data)
{
    orxVIEWPORT * pstViewport = (orxVIEWPORT *)pvViewport;
    // Gets screen bitmap
    orxBITMAP * pstScreen = orxDisplay_GetScreenBitmap();

    // Restores screen as destination bitmap
    orxDisplay_SetDestinationBitmaps(&pstScreen, 1);

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Restores screen bitmap clipping
    orxDisplay_SetBitmapClipping(orxDisplay_GetScreenBitmap(), 0, 0, orxF2U(fb_width), orxF2U(fb_height));

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
        const ImDrawList * cmd_list = draw_data->CmdLists[n];
        ImGui_ImplOrx_Render_CmdList(pstViewport, cmd_list, fb_width, fb_height);
        }
}



/*
void ImGui_ImplOrx_Render(void * pvViewport, ImDrawData* draw_data)
    {
    orxVIEWPORT * pstViewport = (orxVIEWPORT *)pvViewport;

    // flush orx arrays
    orxDisplay_SetVideoMode(NULL);

    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width == 0 || fb_height == 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // We are using the OpenGL fixed pipeline to make the example code simpler to read!
    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, vertex/texcoord/color pointers.
    GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    
    // Render command lists
    #define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
    for (int n = 0; n < draw_data->CmdListsCount; n++)
        {
        const ImDrawList * cmd_list = draw_data->CmdLists[n];
        const ImDrawVert * vtx_buffer = cmd_list->VtxBuffer.Data;
        const ImDrawIdx * idx_buffer = cmd_list->IdxBuffer.Data;

        glVertexPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, pos)));
        glTexCoordPointer(2, GL_FLOAT, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, uv)));
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(ImDrawVert), (const GLvoid*)((const char*)vtx_buffer + OFFSETOF(ImDrawVert, col)));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
            {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback)
                {
                pcmd->UserCallback(cmd_list, pcmd);
                }
            else
                {
                orxBITMAP * pstBitmap = (orxBITMAP *)pcmd->TextureId;
                glBindTexture(GL_TEXTURE_2D, (GLuint) *(uint32_t *)pstBitmap);
                glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer);
                }

            idx_buffer += pcmd->ElemCount;
            }
        }
#undef OFFSETOF

    // Restore modified state
    glBindTexture(GL_TEXTURE_2D, (GLuint)last_texture);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
    }

*/

//////////////////////////////////////////////////////////////////////////
static const char* ImGui_ImplOrx_GetClipboardText(void* user_data)
{
    return NULL;
}

//////////////////////////////////////////////////////////////////////////
static void ImGui_ImplOrx_SetClipboardText(void* user_data, const char* text)
{
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_MouseButtonCallback(int button, int action, int /*mods*/)
{
/*
    if (action == GLFW_PRESS && button >= 0 && button < 3)
        g_MousePressed[button] = true;
*/
}

//////////////////////////////////////////////////////////////////////////
void ImGui_ImplOrx_ScrollCallback(double /*xoffset*/, double yoffset)
{
/*
    g_MouseWheel += (float)yoffset; // Use fractional mouse wheel, 1.0 unit 5 lines.
*/
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
orxBITMAP * InitFontTexture(orxU32 u32Width, orxU32 u32Height)
    {
    orxU32      u32Index;
    orxTEXTURE *pstTexture;
    orxBITMAP  *pstBitmap;
    orxU8      *au8Data;
    orxU32      u32BitmapSize;

    // Creates background texture
    pstBitmap = orxDisplay_CreateBitmap(u32Width, u32Height);
    pstTexture = orxTexture_Create();
    orxTexture_LinkBitmap(pstTexture, pstBitmap, "Texture", orxTRUE);

    u32BitmapSize = u32Width * u32Height * sizeof(orxRGBA);
    // Allocates pixel buffer
    au8Data = (orxU8 *)orxMemory_Allocate(u32BitmapSize, orxMEMORY_TYPE_VIDEO);

    // For all pixels
    for (u32Index = 0; u32Index < (u32BitmapSize / 4); u32Index+=4)
        {
        // Stores pixel channels
        au8Data[u32Index] = au8Data[u32Index + 1] = au8Data[u32Index + 2] = 0x00;
        au8Data[u32Index + 3] = 0xFF;
        }

    // Updates bitmap content
    orxDisplay_SetBitmapData(pstBitmap, au8Data, u32BitmapSize);

    // Frees pixel buffer
    orxMemory_Free(au8Data);

    return pstBitmap;
    }

//////////////////////////////////////////////////////////////////////////
bool ImGui_ImplOrx_CreateDeviceObjects()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;

    // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. 
    // If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

    // Store our identifier
    g_FontTexture = InitFontTexture(width, height);
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

    // Start the frame
    ImGui::NewFrame();
}
