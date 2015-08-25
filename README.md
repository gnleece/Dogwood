#Dogwood

A work-in-progress game engine in OpenGL.

![alt tag](https://lh3.googleusercontent.com/pQ_lW_9z3a-8k1aSF0ot7yEhMIX4b8UVhA3A7eHZjUI=w1616-h874-no)

##Engine

The engine itself it located under [Dogwood\Engine](../master/Engine). This project builds static libraries that are included in the Editor and Game projects.

####Scene Files
Scenes are represented as XML files. Here's an [example scene](../master/Game/Assets/Scenes/Scene0.xml).

####Model Loading
The engine currently supports OBJ models only.

To create a compatible model in Blender, select exactly these export options:
> * Apply Modifiers
* Include Normals
* Include UVs
* Triangulate Faces
* Objects as OBJ Objects

To get UVs for the object, open the Object Data section in the right side menu, and add a new UV Map to the UV Maps section.

To get smooth shading, select "Smooth" before export.


##Editor

The editor is located under [Dogwood\Editor](../master/Editor). The editor allows you to load a scene file and manipulate it. The editor currently supports GameObject creation, deletion, renaming, and reparenting. It also allows you to edit the transform (position/rotation/scale) of each GameObject.


##Game

A sample game is located under [Dogwood\Game](../master/Game). Currently it's just a simple scene with some primitive shapes.
