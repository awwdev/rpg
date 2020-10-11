@echo off ^
 & mkdir out\\Optimized ^
 & mkdir out\\tmp ^
 & xcopy /s /y /d res\* out\\Optimized\\res\* ^
 & Pushd G:\\DEV2020\\buildtools\\msys2\\mingw64\\bin ^
 & g++ ^
-I %2 ^
-L %5 ^
-Wall -Wpedantic -Wextra ^
-Ofast ^
-municode ^
-std=c++2a ^
-o %3 ^
%4 ^
-static ^
-lpthread ^
-lstdc++ ^
-lvulkan-1 ^
 & popd