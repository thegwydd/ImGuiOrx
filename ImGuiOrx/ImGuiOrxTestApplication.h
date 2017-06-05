#pragma once
//! Includes

#include "OrxGuiApplication.h"
#include "ObjectHierarchy.h"

using namespace ImGui::Orx;

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
        ScrollObject *		m_CurrentScene;
        ScrollObject *		m_Soldier;
        bool                m_Show_test_window;
        bool                m_Show_another_window;
        ObjectHierarchy     m_ObjectHierarchy;


	};