C:\MinGW\mingw64\bin\g++.exe -std=c++17 -c src\main.cpp src\controls.cpp src\console.cpp src\node.cpp src\spatial.cpp src\viewport.cpp src\assets.cpp -ISFML\include -Isrc\include -O2
C:\MinGW\mingw64\bin\g++.exe -DSFML_STATIC -std=c++17 -o build\gve_app imgui.o imgui_impl_opengl3.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui-SFML.o main.o controls.o console.o node.o spatial.o viewport.o assets.o -static -static-libstdc++ -static-libgcc -lpthread -L"SFML/lib" -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -lopengl32 -mwindows
cd build
strip gve_app.exe
gve_app.exe
pause