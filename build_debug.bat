@echo off ^
 & mkdir build_debug ^
 & mkdir build_debug\objs ^
 & mkdir build_debug\res ^
 & xcopy /s /y /d res\* build_debug\res\* ^
 & call G:\DEV2020\buildtools\msvc\VC\Auxiliary\Build\vcvars64.bat ^
 & cl ^
 /std:c++latest ^
 /Od ^
 /Zi ^
 /nologo ^
 /MTd ^
 /GR ^
 /W3 ^
 /D _UNICODE /D UNICODE /D _STATIC_CPPLIB /D _HAS_EXCEPTIONS=0 ^
 /Fo: build_debug\objs\ ^
 /Fe: build_debug\main_debug.exe ^
 /Fd: build_debug\ ^
 /I src\ ^
 src\*.cpp ^
 user32.lib ^
 Shell32.lib ^
 third\libs\vulkan-1.lib ^