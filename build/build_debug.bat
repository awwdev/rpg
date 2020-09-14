@echo off ^
 & mkdir build\\debug ^
 & mkdir build\\debug\\res ^
 & xcopy /s /y /d res\* build\\debug\\res\* ^
 & Pushd G:\\DEV2020\\buildtools\\msys2\\mingw64\\bin ^
 & gcc ^
-I %2 ^
-L %5 ^
-Wall -Wpedantic -Wextra ^
-Wno-missing-field-initializers ^
-Wno-unused-variable ^
-Wno-unused-parameter ^
-g ^
-municode ^
-std=c++2a ^
-o %3 ^
%4 ^
-static ^
-lpthread ^
-lstdc++ ^
-lVkLayer_utils ^
-lvulkan-1 ^
 & popd