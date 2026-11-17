#### this is the English version of the README | [версия README на русском](README.md)
# Project "Turret_2"
Project "Turret_2". A 2D game in the tower-defense genre. Distributed completely free of charge.
___
## General information
Inspirational sources for "Turret_2": "Mindustry", "Yorg.io", "Factorio".
The game graphics are based on SDL3 and OpenGL 4.6.
___
## Minimum system requirements
1) An Intel Core i3 / AMD FX processor with a clock speed of 2.5 GHz or higher (or an equivalent with SSE4.2 instruction set support).
2) 4 GB of RAM.
3) A graphics card or integrated graphics core with OpenGL 4.6 support: (NVIDIA GeForce GTX 750 Ti / GTX 950, AMD Radeon RX 360, Intel HD 530) or newer.
4) 25 MB of free disk space for the game itself, 512 MB for saves.
5) A 64-bit OS: Windows 10/11, Linux Ubuntu latest (there may be difficulties running on other Linux distributions; please report if you encounter errors).
___
## Libraries
All used libraries are located in the "libs" folder in pre-compiled form. Licenses for all used libraries are attached in "libs/licenses".
The decision to place libraries directly in the project repository was made in order to increase the reliability and simplicity of building the project.
___
## Interaction and assistance
The project's build instructions need to be refined. A clear, beginner-friendly guide with illustrations needs to be created.

### How to build the project locally?
#### Building in an IDE (using Visual Studio 2026 as an example)
1) Select the project folder and choose "Open with Visual Studio". Alternatively, open Visual Studio and open the project folder from within Visual Studio.
2) Visual Studio will automatically load "CMakePresets.json" and offer a choice of build configurations. To build the game with all optimizations and without integration tests, select "release". Click the build button (the green triangle labeled "Turret_2.exe").
3) Copy the "res" folder and the ".dll" files from "libs/MSVC_dll" (on Linux, the ".so" files from "libs/GCC_so") into the folder with the built game ("build/release").
4) The project is built. You can run the "Turret_2.exe" executable (or without the ".exe" extension on Linux). Everything should already work.

#### [Building under NIX](NIX.md)
#### Possible errors
You can leave messages about build errors or game bugs in the issues section.
