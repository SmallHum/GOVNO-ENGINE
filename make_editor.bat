cmake -G Ninja -S src -B ../build -DCMAKE_POLICY_VERSION_MINIMUM="3.5"
cd ../build
Ninja
strip gve-editor.exe
gve-editor
pause