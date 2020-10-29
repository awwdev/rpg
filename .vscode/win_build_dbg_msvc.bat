@echo off ^
 & mkdir out\\tmp ^
 & xcopy /s /y /d res\* out\\res\* ^
 & call G:\DEV2020\buildtools\msvc\VC\Auxiliary\Build\vcvars64.bat ^
 & cl ^
/std:c++latest ^
/Od ^
/Zi ^
/nologo ^
/MTd ^
/GR ^
/W1 ^
/D _UNICODE /D UNICODE /D _STATIC_CPPLIB /D _HAS_EXCEPTIONS=0 ^
/Fo: out\\Debug\\Obj\\ ^
/Fe: out\\Debug\\main_debug.exe ^
/Fd: out\\Debug\\ ^
/I %2 ^
%4 ^
user32.lib ^
Shell32.lib ^
ext\libs\vulkan-1.lib ^