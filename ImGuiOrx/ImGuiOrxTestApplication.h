#pragma once
//! Includes

extern "C" {
#include <orx.h>
    }

#define __NO_SCROLLED__
#include "Scroll/Scroll.h"

#include <vector>

//! OrxScroll class
class ImGuiOrxTestApplication : public Scroll<ImGuiOrxTestApplication>
	{
	public:

	private: // Overrides
		//! Initialize the program
		virtual orxSTATUS	Init();
		//! Callback called every frame
		virtual orxSTATUS	Run();
		//! Exit the program
		virtual void		Exit();
		//! Binds objects
		void				BindObjects();

	private: // Overrides
        //! Initializes Gui system
        void				InitializeGuiSystem();
        //! Initializes ORX events to pass to input system
        void				InitializeEvents();
        //! Initializes the scene
        void				InitializeScene();
        //! Main event handler
        orxSTATUS			HandleOrxEvent(const orxEVENT *_pstEvent);
        // Resizes the viewport
        void                ResizeViewport();
        

	private: // Statics
		static orxSTATUS orxFASTCALL StaticEventHandler(const orxEVENT *_pstEvent);

	};