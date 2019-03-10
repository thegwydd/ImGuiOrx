# ImGuiOrx v0.1.0 -  ImGui for Orx Engine

ImGuiOrx is an integration of ImGui for the Orx Game Engine.

***

## Building ImGui Orx

The library depends on:
- Orx Game Engine library
- ImGui library

Before building ImGuiOrx, you need to perform few steps.

### Build Orx Game Engine

Please, clone Orx Game Engine from here (https://github.com/orx/orx) and follow instructions on how to build it.  
If you're a Linux/OSx user, please take also a look here (http://orx-project.org/wiki/en/tutorials/main#linux).

When you've done with cloning, setupping and building, please check if in your environment variables you have one named ORX that points to the Orx Game Engine folder.

### Setup ImGuiOrx

ImGuiOrx library depends on ImGui (https://github.com/ocornut/imgui.git) hence a submodule has been used.
When cloning the repository, please use --recursive option in order to allow submodules creation.

* git clone https://github.com/thegwydd/ImGuiOrx.git --recursive

After cloning ImGuiOrx repository you need to run **./setup(.bat/.sh)** and let it create all project files you need.


### Build ImGuiOrx Library

Now you can move to **(ImGuiOrx folder)/build/(platform)** and select your favourite IDE folder and use project files to build ImGuiOrx.  
In the output folder, **(ImGuiOrx folder)/lib/static**, you can find produced static library.

For example, with gmake (mingw32):

```
mingw32-make imgui_orx config=debug
mingw32-make imgui_orx config=release
```


Produces lib/static/libimgui_orx.a and lib/static/libimgui_orxd.a

### IMPORTANT for Visual Studio users:
When you load the solution you need to right-click on the solution and then "Retarget Solution".

***

## Test application

Under **(ImGuiOrx folder)/test** folder you can find a test application you can use to play with this integration.  
It will be built in the same time of ImGuiOrx Library.  
You can find produced executable file in **(ImGuiOrx folder)/bin/** folder.

***

## How to use ImGuiOrx Library

Depending to the IDE you're using, you need to specify, in your project, where the compiler will find needed header files.
In addition to Orx Game Engine header folder, you need to specify two more folders:

- **(ImGuiOrx folder)/include**
- **(ImGuiOrx folder)/imgui**

To allow your linker to link the proper library you need to specify static library folder where ImGuiOrx static library can be found:
 -**(ImGuiOrx folder)/lib/static**

You need to link at least two libraries in order to build your project:
- imgui_orx
- orx

**In your test project, the *orx* library must be the last one to be linked, hence, it must be put after imgui_orx library.**


As you can see in **(ImGuiOrx folder)/test/main.cpp**, you need to include:

- ImGui_Orx.h
- imgui.h
- orx.h

If you take a look at the example file, you can see in *ImGuiOrx_Init()* there's ImGui context initialization.
After the initialization, Orx will continuously call *ImGuiOrx_Run()*, the place where we tell ImGui what controls we want to draw (see ImGui tutorials)
To render all of ImGui controls, we need our application to react *orxEVENT_TYPE_RENDER* events with *orxRENDER_EVENT_STOP* id.  
That's needed because we want to render ImGui stuff on top of other game stuff.

To better understand how to use ImGui, please take a look at **(ImGuiOrx folder)/imgui/imgui_demo.cpp** as it exposes almost all features ImGui implements.

## More details
For a walkthrough on how to set up your own ImGui based project, see http://orx-project.org/wiki/en/tutorials/imgui

***


## License

This code is distributed under WTFPL License (http://www.wtfpl.net/)  
Enjoy and don't fuck about my Whitesmiths coding style!
