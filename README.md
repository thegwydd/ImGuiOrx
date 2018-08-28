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

After cloning ImGuiOrx repository you need to run **./setup(.bat/.sh)** and let it create all project files you need.

ImGuiOrx library depends on ImGui (https://github.com/ocornut/imgui.git) hence a submodule has been used.

Ensure the submodules required by this repo are updated before building. To do so:
* cd into the imguiorx repo folder
* git submodule init
* git submodule update


### Build ImGuiOrx Library

Now you can move to **(ImGuiOrx folder)/build/(platform)** and select your favourite IDE folder and use project files to build ImGuiOrx.  
In the output folder, **(ImGuiOrx folder)/lib/static**, you can find produced static library.


## Test application

Under **(ImGuiOrx folder)/test** folder you can find a test application you can use to play with this integration.  
It will be built in the same time of ImGuiOrx Library.  
You can find produced executable file in **(ImGuiOrx folder)/bin/** folder.


#### Attention:  To run the test application you need to manually copy two files in the "*bin*" folder :   
- the proper orx dynamic library (.dll/.so)  
- **ImGuiOrxTestApplication.ini** that can be found in **(ImGuiOrx folder)/test**  

This issue will be solved on next release


***


## License

This code is distributed under WTFPL License (http://www.wtfpl.net/)
Enjoy and don't fuck about Whitesmiths coding style!
