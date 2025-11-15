cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(PROJECT_NAME gve-editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(${PROJECT_NAME})

include(FetchContent)

#					SFML
FetchContent_Declare(
    sfml
    GIT_REPOSITORY https://github.com/SFML/SFML
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(sfml)

#					ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.91.9
)
FetchContent_MakeAvailable(imgui)

#					ImGui-SFML
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG v3.0
)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO ON)

FetchContent_MakeAvailable(imgui-sfml)

add_executable(${PROJECT_NAME} 
	tools/editor.cpp 
	
	viewport.cpp 
	assets.cpp 
	struct_loader.cpp 
	
	label.cpp 
	aabb.cpp 
	spatial.cpp 
	node.cpp 

	fstream_opers.cpp 
	controls.cpp
	)

target_link_libraries(${PROJECT_NAME} PUBLIC SFML::Graphics SFML::Window SFML::System SFML::Audio)
target_link_libraries(${PROJECT_NAME} PUBLIC ImGui-SFML::ImGui-SFML)
target_include_directories(${PROJECT_NAME} PUBLIC 
	"include"
	"../../build/_deps/sfml-src/include"
	"../../build/_deps/imgui-src" 
	"../../build/_deps/imgui-sfml-src" 
	)