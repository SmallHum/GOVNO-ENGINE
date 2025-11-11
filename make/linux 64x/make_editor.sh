compiler='g++'

app_name='gve-editor'

build="build/linux 64x"
includes='-ISFML-LINUX/include -Isrc/include'

compile="src/tools/editor.cpp \
	src/viewport.cpp \
	src/assets.cpp \
	src/struct_loader.cpp \
	src/label.cpp \
	src/aabb.cpp \
	src/spatial.cpp \
	src/node.cpp \
	src/controls.cpp"

imgui_obj="imgui.o \
	imgui_stdlib.o \
	imgui_impl_opengl3.o \
	imgui_draw.o \
	imgui_tables.o \
	imgui_widgets.o \
	imgui-SFML.o"

myobj="editor.o \
	viewport.o \
	assets.o \
	struct_loader.o \
	label.o \
	aabb.o \
	spatial.o \
	node.o \
	controls.o"

sfl="SFML-LINUX/lib/libsfml"

sfml_link="$sfl-graphics-s.a $sfl-window-s.a $sfl-audio-s.a $sfl-system-s.a"

link="-lX11 -lXrandr -lXcursor -lXinerama -lXi \
	-lglut -lGLU -lGL -lfreetype -lopenal -ludev \
	-lvorbis -lvorbisenc -lvorbisfile -logg -lFLAC \
	-lpthread -ldl -static-libstdc++ -static-libgcc"

g++ -c $compile $includes -Wno-narrowing
g++ -o "$build/$app_name" $myobj $imgui_obj $sfml_link $link
cd "$build"
"$app_name"
