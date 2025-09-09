C:\MinGW\mingw64\bin\g++.exe -std=c++17 -c src\main.cpp -ISFML\include -Isrc\include -O2
C:\MinGW\mingw64\bin\g++.exe -std=c++17 -o build\sfml-app main.o -static -static-libstdc++ -static-libgcc -lpthread -LSFML/lib -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio
cd build
strip sfml-app.exe
sfml-app.exe
pause