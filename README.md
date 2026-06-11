# TU Landmark – OpenGL Project

This project is a simple OpenGL-based application written in C++ to render a 2D scene representing one of Taif University's landmarks. The goal of the project is to practice basic OpenGL drawing, window handling, and rendering techniques using FreeGLUT.

---

## Overview
The application opens a graphical window and draws a custom 2D scene using basic OpenGL functions. The code is organized in a clean and readable structure, making it easy to modify or extend.

---

## Features
- 2D rendering using OpenGL  
- FreeGLUT window creation and event handling  
- Clean and simple project structure  
- Easy to build and run on Visual Studio  
- Suitable for learning and academic use  

---

## Requirements
To build and run the project, you need:

- Visual Studio 2022 or newer  
- OpenGL libraries  
- FreeGLUT (included in the project folder)  
- Windows 10 or later  

Make sure the file `freeglut.dll` is placed inside the Debug folder before running the application.

---

## How to Run
1. Open the solution file `CIT.sln` in Visual Studio.  
2. Build the project (Debug x64).  
3. Make sure `freeglut.dll` is located in: `x64/Debug/`
4. Run the project using **Start Debugging (F5)**.  
5. A window will appear displaying the OpenGL scene.

---



















## Project Structure
CIT/
 ├── CIT.sln
 ├── CIT.vcxproj
 ├── main.cpp
 ├── freeglut.dll
 ├── x64/
 │    └── Debug/
 │         ├── CIT.exe
 │         ├── CIT.pdb
 │         └── freeglut.dll
 └── CIT/
      └── x64/Debug/...





































---

## How It Works
The program initializes an OpenGL window using FreeGLUT and draws the scene using basic OpenGL commands such as:

- glBegin()  
- glVertex2f()  
- glColor3f()  

The drawing logic is handled inside the `display()` function, which is called every time the window refreshes.

---

## Author
**Mohammed Fawaz Al-Gorashi**  
Computer Science Student – Taif University  
GitHub: ABUsahraj

---

## License
This project is intended for academic and learning purposes.
