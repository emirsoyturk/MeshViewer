# MeshViewer
## Simple Mesh Viewer for .off Files written with OpenGL

## Features

- Camera can be translate with scrolling the wheel button.
- Camera can also rotate by pressing and moving the mouse.
- Key N will bring up to screen next model.
- Key P will bring up to screen previous model
- You can move model by pressing x-X, y-Y and z-Z
- You can scale model by pressing s-S

## Languages

Written using C++, freeglut and OpenGL

## Running

For compile in ubuntu

```
g++ -o MeshViever Main.cpp Camera.cpp Model.cpp InitShader.cpp -lGL -lGLU -lglut -lGLEW
```

To run

```
./MeshViewer *.off
```
