# GOVNO-ENGINE
My attempt at video game engines using SFML and ImGui.
Unfortunately, there isn't much functionality. It's just my silly project for fun.

# COMPILATION
SFML and ImGui libraries are already in the repo.
All you need to do:
- have the gcc 14.2.0+ toolkit installed
- make sure that compiler folder is included in PATH
- ```make/make_imgui.bat```, to make the imgui objects
- ```make/make_editor.bat```, if you want to run the editor
- ```make/make_editor.bat```, if you want to run the resulting app.

Good luck figuring out anything in my software.



# TODO:

## Real things

### Struct Label

[] test struct GVEFont
[] finish label.cpp
[] test srtuct Label

## Planned structs

### CollObj

Family of structs made for collision resolution.

struct CollObj - object that can handle collision detection and resolution
    ├ struct CollAABB - collision box
    │
    └ struct KinemObj - can also move (has vel, accel etc.)
        └ struct RigidBody - lol what hell nah impossible


        АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ