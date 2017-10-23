# ImGui for Orx Engine

ImGuiOrx is an integration of ImGui for the Orx Game Engine.

## Build ainvar/Orx

The ImGuiOrx VS2015 project needs an environment variable **ORX_PATH** that needs to point to the Orx base folder.

Currently, ORX_PATH is not to point to the regular version of Orx, but the ainvar version instead: (https://bitbucket.org/ainvar/orx). Clone this version first and set your ORX_PATH to this.

Run setup.bat in the root of the repo folder, if has not run automatically.

Open the ainvar/Orx solution in VS2015 and Build orx libraries for Debug, Profile & Release.

## Build ImGuiOrx

This library depends on ImGui (https://github.com/ocornut/imgui.git) hence a submodule has been used.

There's only a VS2015 project but you're kindly encouraged to create other projects files for other targets.

Ensure the submodules required by this repo are updated before building. To do so:
* cd into the imguiorx repo folder
* git submodule init
* git submodule update

Build in VS2015.

## Test application

Under **test** folder you can find a test application you can use to play with this integration.
It uses **scroll** (https://github.com/orx/scroll.git) and the repo has been added as submodule.

## License

This code is distributed under WTFPL License (http://www.wtfpl.net/)
Enjoy.
