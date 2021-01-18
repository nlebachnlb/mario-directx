# Super Mario Bros. 3 (NES) with DirectX
* OS: Windows 10
* Graphic Libary: DirectX 9
* Language(s): C/C++
## Introduction
My project in the subject "Introduction to Game Development" at my university using DirectX 9 library. The requirement is to manually build a **Game Framework** and make SMB3 based on it instead of using Game Engine. Our game framework must be optimized for 2D Plaformer game (performance). 
## Tools
* Map editor: Tiled Map Editor
* Data reader: TinyXML library for reading XML
* Image editor: Adobe Photoshop
* Data generation for Grid: Grid data generator (my own tool written in NodeJS, see usage in GridTool/)
## Technique
* SweptAABB: for detecting collisions of physical movement
* AABB: for detecting overlapped collisions
* Raycast: for asynchronous line collision checking calls
* Grid: for space partitioning
## Control
### Main Menu
* Select options: Up/Down arrow key
* Decision: Enter/Return key
### Overworld Map
* Movement (select level): Arrow keys (left, right, up, down)
* Enter level: "S" key
### In Game
* Ground movement: Left/Right arrow
* Run: Hold "A" key and move
* Jump: Press "S" for low jump, hold "S" for high jump (longer, higher)
* Duck (crouch): Down arrow
* Warp to Pipes: An Arrow key on a respective direction (left, right, up, down)
