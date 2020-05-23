@echo off ^
 & mkdir build_debug ^
 & mkdir build_debug\objs ^
 & call G:\DEV2020\buildtools\msvc\VC\Auxiliary\Build\vcvars64.bat ^
 & cl ^
 /std:c++latest ^
 /Od ^
 /Zi ^
 /nologo ^
 /MTd ^
 /D _UNICODE /D UNICODE /D _STATIC_CPPLIB /D _HAS_EXCEPTIONS=0 ^
 /Fo: build_debug\objs\ ^
 /Fe: build_debug\main_debug.exe ^
 /Fd: build_debug\ ^
 src\*.cpp ^
 user32.lib ^
 third\libs\vulkan-1.lib ^