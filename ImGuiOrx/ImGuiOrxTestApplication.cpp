//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "ImGuiOrxTestApplication.h"
#undef __SCROLL_IMPL__

#include "ImGui_ImplOrx.h"

#include "EnemyBug.h"
#include "Hero.h"
#include "Soldier.h"
#include "Walls.h"


#ifndef __orxMSVC__
//////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
    {
    /* Inits and executes orx */
    ImGuiOrxTestApplication::GetInstance().Execute(argc, argv);
    return EXIT_SUCCESS;
    }
#else  // __orxMSVC__
 int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
 	{
 	/* Inits and executes orx */
 	ImGuiOrxTestApplication::GetInstance().Execute();

 	// Done!
 	return EXIT_SUCCESS;
 	}
#endif // __orxMSVC__

//////////////////////////////////////////////////////////////////////////
orxSTATUS orxFASTCALL ImGuiOrxTestApplication::StaticEventHandler(const orxEVENT *_pstEvent)
	{
	return ImGuiOrxTestApplication::GetInstance().HandleOrxEvent(_pstEvent);
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::Init ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;
 
    m_CurrentScene = nullptr;
    m_Soldier = nullptr;

    InitializeGuiSystem();
    InitializeEvents();
    BindObjects();
    InitializeScene();

	return result;
	}

bool show_test_window = true;
bool show_another_window = false;

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::Run ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;

    ImGui_ImplOrx_NewFrame();

    ImVec4 clear_color = ImColor(114, 144, 154);

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    ImGui::Text("Hello, world!"); 
    static float f = 0.0f;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
        {
        ImGui::SetNextWindowPos(ImVec2());
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
        }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (show_test_window)
        {
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&show_test_window);
        }

    // Rendering
    ImGui::Render();

    return result;
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::Exit ()
	{
    ImGui_ImplOrx_Shutdown();
    }

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::HandleOrxEvent(const orxEVENT *_pstEvent)
	{
	if ((_pstEvent->eType == orxEVENT_TYPE_RENDER) && (_pstEvent->eID == orxRENDER_EVENT_STOP))
        ImGui_ImplOrx_Render(NULL, ImGui::GetDrawData());

	if (_pstEvent->eType == orxEVENT_TYPE_DISPLAY)
        ResizeViewport();

	return orxSTATUS_SUCCESS;
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::ResizeViewport()
    {
    orxFLOAT scr_w, scr_h;
    orxDisplay_GetScreenSize(&scr_w, &scr_h);

    orxFLOAT vwp_w, vwp_h;
    orxViewport_GetSize(GetMainViewport(), &vwp_w, &vwp_h);

    orxAABOX frustum;
    orxCamera_GetFrustum(GetMainCamera(), &frustum);
    
    orxVECTOR cam_pos;
    orxCamera_GetPosition(GetMainCamera(), &cam_pos);
    orxCamera_SetFrustum(GetMainCamera(), vwp_w, vwp_h, frustum.vTL.fZ, frustum.vBR.fZ);
    orxCamera_SetPosition(GetMainCamera(), &cam_pos);

    orxDEBUG_PRINT(orxDEBUG_LEVEL_LOG, "Viewport Size : %f, %f", vwp_w, vwp_h);
    }

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::InitializeGuiSystem()
    {
    // Setup ImGui binding
    ImGui_ImplOrx_Init();

    // Load Fonts
    // (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
    //ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
    //io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    }


//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::InitializeEvents()
	{
	//.. renderer
    orxEvent_AddHandler(orxEVENT_TYPE_RENDER, StaticEventHandler);
    orxEvent_AddHandler(orxEVENT_TYPE_VIEWPORT, StaticEventHandler);
	// add event for resizing
	orxEvent_AddHandler(orxEVENT_TYPE_DISPLAY, StaticEventHandler);
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::InitializeScene()
	{
    // create the scene
    orxConfig_Load("ImGuiOrx.ini");
    orxConfig_Load("Level1.ini");

    m_CurrentScene = CreateObject("Walls");

    // create objects from level1
    m_Soldier = CreateObject("Soldier");
   
    // an enemies of course...
    for (orxU32 i = 0; i < 5; i++)
        {
        ScrollObject * pObj = CreateObject("O-EnemyBug");
        const orxCHAR * pszName = orxObject_GetName(pObj->GetOrxObject());
        }
    }

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::BindObjects()
	{
    ScrollBindObject<EnemyBug>("O-EnemyBug");
    ScrollBindObject<Hero>("O-Hero");
    ScrollBindObject<Soldier>("Soldier");
    ScrollBindObject<Walls>("Walls");
    }

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::PrintSections()
    {
    orxU32 u32SectionCount = orxConfig_GetSectionCounter();
    for (orxU32 u32SectionIndex = 0; u32SectionIndex < u32SectionCount; u32SectionIndex++)
        {
        const orxCHAR * strCurrentSection = orxConfig_GetSection(u32SectionIndex);
        const orxCHAR * strCurrentSectionOrigin = orxConfig_GetOrigin(strCurrentSection);

        orxConfig_PushSection(strCurrentSection);

        orxU32 u32KeyCount = orxConfig_GetKeyCounter();
        for (orxU32 u32KeyIndex = 0; u32KeyIndex < u32KeyCount; u32KeyIndex++)
            {
            const orxCHAR * strCurrentKey = orxConfig_GetKey(u32KeyIndex);
            if (orxConfig_IsInheritedValue(strCurrentKey) == orxTRUE)
                {
                const orxCHAR * strCurrentKeyParent = orxConfig_GetValueSource(strCurrentKey);
                }
            }

        orxConfig_PopSection();

        }
    }
