@echo off ^
 & mkdir Build\\Debug ^
 & xcopy /s /y /d res\* Build\\Debug\\res\* ^
 & Pushd G:\\DEV2020\\buildtools\\msys2\\mingw64\\bin ^
 & g++ ^
-I %2 ^
-L %5 ^
-Wall -Wpedantic -Wextra ^
-Wno-missing-field-initializers ^
-Wno-unused-variable ^
-Wunused-but-set-variable ^
-Wno-unused-parameter ^
-Wunused-but-set-variable ^
-g ^
-municode ^
-std=c++2a ^
-o %3 ^
%4 ^
-static ^
-lstdc++ ^
-lVkLayer_utils ^
-lvulkan-1 ^
 & popd