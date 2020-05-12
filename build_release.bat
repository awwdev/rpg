@echo off ^
 & mkdir build_release ^
 & mkdir build_release\objs ^
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
 user32.lib