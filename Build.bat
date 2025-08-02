@echo off
setlocal

REM setup environment
set target=
set build_dir=Build\Bat
set build_tool="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM setup build tool
echo Running: %build_tool%
call %build_tool%

REM check if build directory exists
if not exist "%build_dir%" (
    mkdir "%build_dir%"
)

REM check target argument
if not "%~1" == "" (
    set target=--target %~1%
)

REM Ninjaビルド用にジェネレーター指定を追加
set command=cmake -G "Ninja" -S . -B "%build_dir%"
if "%~1" == "setup" (
    echo Running: %command%
    %command%
    goto :END
)

set command=cmake --build "%build_dir%" %target%
echo Running: %command%
%command%

:END
endlocal