@echo off ^
 & mkdir build_release ^
 & mkdir build_release\objs ^
 & mkdir build_release\res ^
 & xcopy /s /y /d res\* build_release\res\* ^
 & call G:\DEV2020\buildtools\msvc\VC\Auxiliary\Build\vcvars64.bat ^
 & cl ^
 /std:c++latest ^
 /nologo ^
 /O2 ^
 /Oi ^
 /MT ^
 /D _UNICODE /D UNICODE /D _STATIC_CPPLIB /D _HAS_EXCEPTIONS=0 ^
 /Fo: build_release\objs\ ^
 /Fe: build_release\main_release.exe ^
 src\*.cpp ^
 user32.lib ^
 third\libs\vulkan-1.lib