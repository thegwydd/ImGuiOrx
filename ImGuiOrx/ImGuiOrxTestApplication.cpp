//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "ImGuiOrxTestApplication.h"
#undef __SCROLL_IMPL__

#include "ImGui_ImplOrx.h"


//#ifndef __orxMSVC__
//////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
    {
    /* Inits and executes orx */
    ImGuiOrxTestApplication::GetInstance().Execute(argc, argv);
    return EXIT_SUCCESS;
    }
//#else  // __orxMSVC__
// Here's an example for a console-less program under windows with visual studio
// int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
// 	{
// 	/* Inits and executes orx */
// 	ImGuiOrxTestApplication::GetInstance().Execute();
// 
// 	// Done!
// 	return EXIT_SUCCESS;
// 	}
//#endif // __orxMSVC__




//////////////////////////////////////////////////////////////////////////
orxSTATUS orxFASTCALL ImGuiOrxTestApplication::StaticEventHandler(const orxEVENT *_pstEvent)
	{
	return ImGuiOrxTestApplication::GetInstance().HandleOrxEvent(_pstEvent);
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::Init ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;
    
    InitializeGuiSystem();
    InitializeEvents();
    BindObjects();
    InitializeScene();

	return result;
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::Run ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;

    ImGui_ImplOrx_NewFrame();

    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    ImGui::Text("Hello, world!"); 
/*
    static float f = 0.0f;
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", (float*)&clear_color);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window)
        {
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
*/

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

/*
	if (_pstEvent->eType == orxEVENT_TYPE_DISPLAY)
			UpdateCanvasSize();
*/

/*
	else
		m_GwenInput.ProcessMessage(_pstEvent);
*/
	
	return orxSTATUS_SUCCESS;
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
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::BindObjects()
	{
	}

