set "compiler=g++"

set "app_name=gve-editor"

set "src=..\src"

set "build=..\build"
set "includes=-I..\SFML\include -I..\src\include"

set "compile=%src%\tools\editor.cpp %src%\fstream_opers.cpp %src%\controls.cpp %src%\node.cpp %src%\spatial.cpp %src%\viewport.cpp %src%\assets.cpp"

set "imgui_obj=imgui.o imgui_impl_opengl3.o imgui_draw.o imgui_tables.o imgui_widgets.o imgui-SFML.o"
set "myobj=editor.o fstream_opers.o controls.o node.o spatial.o viewport.o assets.o"

set "obj=%imgui_obj% %myobj%"

set "sfml_link=-L../SFML/lib -DSFML_STATIC -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio"

set "link=-static-libstdc++ -static-libgcc -lpthread -lopengl32 %sfml_link%"

%compiler% -std=c++17 -c %compile% %includes% -O2 -Wno-narrowing
%compiler% -std=c++17 -o "%build%/%app_name%" %obj% -static %link%
cd %build%
strip "%app_name%".exe
"%app_name%"
pause