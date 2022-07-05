# MeshViewer
## Simple Mesh Viewer for .off Files written with OpenGL

<img src="https://serving.photos.photobox.com/65497905d11e873f28b27f98ff907a2ef69b753518162bae9d6587cd8be55879ecb4a5cf.jpg" width="500" height="281" />
<img src="https://serving.photos.photobox.com/60373113259aa42c1abd6d5715ced81da85c461d9c43cc2e9fe7db20f6a221cb2c987243.jpg" width="500" height="281" />
<img src="https://serving.photos.photobox.com/48472398199d615c167925f8ceb5da1dce06a8a6c32a704bfcf546f6cb113283dae174f9.jpg" width="500" height="281" />

## Features

- Camera can be translate with scrolling the wheel button.
- Camera can also rotate by pressing and moving the mouse.
- Key N will bring up to screen next model.
- Key P will bring up to screen previous model
- You can change view mode by pressing V E F keys
- You can rotate model by pressing arrow keys

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
