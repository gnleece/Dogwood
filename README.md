# Dogwood

A work-in-progress game engine in OpenGL.

![Dogwood Editor screenshot](https://lh3.googleusercontent.com/pQ_lW_9z3a-8k1aSF0ot7yEhMIX4b8UVhA3A7eHZjUI=w1616-h874-no)

For a fun gallery of screenshots showing the engine's progress over the years, check out [this album](https://photos.google.com/album/AF1QipNiVXrhrLjHQlZzFbLOllLvEPshBfpDqXxtWBc_).

## Engine

The engine itself it located under [Dogwood/Engine](../master/Engine). This project builds static libraries that are included in the Editor and Game projects.

#### Physics

The engine includes a work-in-progress physics engine, adapted from Game Physics Engine Development by Ian Millington. Currently there is support for sphere colliders and box colliders (and collision detection is implemented for sphere-sphere, sphere-box, and box-box collisions). Collision resolution is currently super buggy, but sort of works in very limited situations. See [Dogwood/Engine/Include/Physics](../master/Engine/Include/Physics).

#### Scene Files
Scenes are represented as XML files. Here's an [example scene](../master/Game/Assets/Scenes/Scene0.xml).

#### Model Loading
The engine currently supports OBJ models only.

To create a compatible model in Blender, select exactly these export options:
> * Apply Modifiers
> * Include Normals
> * Include UVs
> * Triangulate Faces
> * Objects as OBJ Objects

To get UVs for the object, open the Object Data section in the right side menu, and add a new UV Map to the UV Maps section.

To get smooth shading, select "Smooth" before export.


## Editor

The editor is located under [Dogwood/Editor](../master/Editor). The editor allows you to load a scene file and manipulate it. The editor currently supports GameObject creation, deletion, renaming, and reparenting. It also allows you to edit the transform (position/rotation/scale) of each GameObject.


## Game

A sample game is located under [Dogwood/Game](../master/Game). Currently it's just a simple scene with some primitive shapes.


## Feature Roadmap

For a super rough feature roadmap, TODO list, bug list, and collection of general thoughts/learnings about the project, see [TODO.txt](../master/TODO.txt)
