set "compiler=g++"

set "app_name=gve-app"

set "src=..\..\src"

set "build=..\..\build\windows 64x"
set "includes=-I..\..\SFML\include -I..\..\src\include"

set "compile=%src%\main.cpp %src%\viewport.cpp %src%\assets.cpp %src%\struct_loader.cpp %src%\label.cpp %src%\aabb.cpp %src%\spatial.cpp %src%\node.cpp %src%\controls.cpp"

set "myobj=main.o viewport.o assets.o struct_loader.o label.o aabb.o spatial.o node.o controls.o"

set "obj=%myobj%"

set "sfml_link=-L../../SFML/lib -DSFML_STATIC -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio"

set "link=-static-libstdc++ -static-libgcc -lpthread -lopengl32 %sfml_link%"

%compiler% -std=c++17 -fexec-charset=cp1251 -c %compile% %includes% -O2 -Wno-narrowing
%compiler% -std=c++17 -o "%build%/%app_name%" %obj% -static %link%
cd %build%
strip "%app_name%"
"%app_name%"
pause