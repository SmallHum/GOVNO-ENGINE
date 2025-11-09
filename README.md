# GOVNO-ENGINE
My attempt at video game engines using SFML and ImGui.
Unfortunately, there isn't much functionality. It's just my silly project for fun.

# COMPILATION
SFML and ImGui libraries are already in the repo.
All you need to do:
- have the gcc 14.2.0+ toolkit installed
- make sure that compiler folder is included in PATH
- ```make/make_imgui.bat```, to make the imgui objects
<<<<<<< HEAD
- ```make/make_editor.bat```, if you want to compile the editor
- ```make/make_app.bat```, if you want to compile the resulting app.

Good luck figuring out anything in my software.



# TODO:

## Real things

### Editor

- undo/redo MUST BE DONE ALREADY  
- refactor it entirely i guess  
- template fromAssets function that takes an asset map as an argument  


### Struct Sprite

animation object as a parameter, which takes an atlas and a vector of boxes, or a size of a single frame
  
## Planned structs:
  
### CollObj
  
Family of structs made for collision resolution.  
  
struct CollObj - object that can handle collision detection and resolution  
    ├ struct CollAABB - collision box  
    │  
    └ struct KinemObj - can also move (has vel, accel etc.)  
        └ struct RigidBody - lol what hell nah impossible  
=======
- ```make/make_editor.bat```, if you want to run the editor
- ```make/make_app.bat```, if you want to run the resulting app.

Good luck figuring out anything in my software.
>>>>>>> b379706289dfe5784ac27d079ebcbf5a1d0ba654
