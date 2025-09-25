C:\MinGW\mingw64\bin\g++.exe -std=c++17 -c src\main.cpp src\controls.cpp src\console.cpp src\node.cpp src\spatial.cpp src\graphics.cpp -ISFML\include -Isrc\include -O2
C:\MinGW\mingw64\bin\g++.exe -std=c++17 -o build\sfml-app main.o controls.o console.o node.o spatial.o graphics.o -static -static-libstdc++ -static-libgcc -lpthread -LSFML/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio -mwindows
cd build
strip sfml-app.exe
sfml-app.exe
pause