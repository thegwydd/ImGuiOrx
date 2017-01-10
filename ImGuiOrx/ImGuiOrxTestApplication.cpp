//! Includes
// The following define/undef is done only once in the project. It should be
// done before including the interface of the class deriving from
// Scroll (as follows).
#define __SCROLL_IMPL__
#include "ImGuiOrxTestApplication.h"
#undef __SCROLL_IMPL__



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

	return result;
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::Run ()
	{
	orxSTATUS result = orxSTATUS_SUCCESS;
	
	return result;
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::Exit ()
	{
	}

//////////////////////////////////////////////////////////////////////////
orxSTATUS ImGuiOrxTestApplication::HandleOrxEvent(const orxEVENT *_pstEvent)
	{
/*
	if ((_pstEvent->eType == orxEVENT_TYPE_RENDER) && (_pstEvent->eID == orxRENDER_EVENT_STOP))
		m_pCanvas->RenderCanvas();

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
void ImGuiOrxTestApplication::InitializeEvents()
	{
/*
	//.. renderer
	orxEvent_AddHandler(orxEVENT_TYPE_RENDER, StaticEventHandler);
	// add event for resizing
	orxEvent_AddHandler(orxEVENT_TYPE_DISPLAY, StaticEventHandler);
*/
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::InitializeScene()
	{
	}

//////////////////////////////////////////////////////////////////////////
void ImGuiOrxTestApplication::BindObjects()
	{
	}

