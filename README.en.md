# Project Turret_2 [русская версия README](README.md)
A 2D game in the tower-defense genre. Distributed completely free of charge.
___
## General Information
Inspirational sources for Turret_2": "Mindustry", "Yorg.io", "Factorio". 
The game graphics are based on SDL3.
___
## Libraries
All used libraries are located in the "libs" folder in pre-compiled form. Licenses for all used libraries are attached in "libs/licenses".
The decision to include libraries directly in the project repository was made to increase reliability and ease of project build process.
___
## Interaction and Assistance
Active assistance is not required at this time. Expect changes regarding this part. 

### How to build the project locally?
#### Windows (MSVC, Visual Studio)
1) The ".sln" and other auxiliary Visual Studio files are already present in this repository. They have relative paths configured for all libraries.
The libraries themselves are already compiled and located in the "libs" folder according to the specified paths. To open the project, simply run "Turret_2.sln".
2) Start the project build (green triangle labeled "Local Windows Debugger"). A "build" folder will appear, containing "Release_win_x64"/"Debug_win_x64" depending on the build settings. At this point, Visual Studio will issue an error message regarding missing dll files.
3) Copy the "res" folder and ".dll" files from "libs/MSVC_dll" into the "Release_win_x64"/"Debug_win_x64" folder.
4) The project is built. You can run the "Turret_2.exe" executable file. It should work.
#### Linux (GCC, CMake)
1) Go to the "cmake_proj" folder.
2) Hover the mouse cursor over an empty space inside the folder. Right-click.
3) Select "Open in Terminal" from the menu that appears.
4) Enter "cmake ." in the terminal. Terminal commands should be entered without quotes. Wait for the command to complete.
5) Enter "cmake --build ." in the terminal. Wait for the command to complete.
6) Copy the "res" folder and ".so" files from "libs/GCC_so" into the "cmake_proj/build" folder.
7) The project is built. You can run the "Turret_2" executable file. It should work.
#### Possible Errors
You can leave messages about build errors or game bugs in the "Issues" section.
