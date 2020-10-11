@echo off ^
 & mkdir out\\Debug ^
 & mkdir out\\tmp ^
 & xcopy /s /y /d res\* out\\Debug\\res\* ^
 & Pushd G:\\DEV2020\\buildtools\\msys2\\mingw64\\bin ^
 & g++ ^
-I %2 ^
-L %5 ^
-Wall -Wextra ^
-Wno-missing-field-initializers ^
-Wno-unused-variable ^
-Wno-unused-parameter ^
-Wno-unused-but-set-variable ^
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