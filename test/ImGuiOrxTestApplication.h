#pragma once
//! Includes

#include "OrxGuiApplication.h"

//////////////////////////////////////////////////////////////////////////
//! OrxScroll class
class ImGuiOrxTestApplication : public OrxGuiApplication<ImGuiOrxTestApplication>
	{
	public:
        ImGuiOrxTestApplication();
        ~ImGuiOrxTestApplication();

    protected: // Overrides
		//! Binds objects
		void				BindObjects();
        //! Initializes the scene
        void				InitializeScene();
        //! Main event handler
        orxSTATUS			OnOrxEvent(const orxEVENT *_pstEvent);
        //! Resizes the viewport
        void                ResizeViewport();
        //! Renders gui stuff
        void                RenderGui();

    private:
        bool                m_Show_test_window;
        bool                m_Show_another_window;
	};