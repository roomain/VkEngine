@echo off
setlocal

if "%~1"=="" goto :usage
if "%~2"=="" goto :usage
if "%~3"=="" goto :usage

set "SOURCE_DIR=%~f1"
set "CONFIG=%~2"
set "PROJECT=%~3"
set "OPTIONS=%~4 %~5 %~6 %~7 %~8 %~9"

if /I "%CONFIG%"=="DEBUG" set "CONFIG=Debug"
if /I "%CONFIG%"=="RELEASE" set "CONFIG=Release"

if /I not "%CONFIG%"=="Debug" if /I not "%CONFIG%"=="Release" (
    echo Erreur : la configuration doit etre Debug ou Release.
    exit /b 1
)

if not exist "%SOURCE_DIR%\CMakeLists.txt" (
    echo Erreur : CMakeLists.txt introuvable dans "%SOURCE_DIR%".
    exit /b 1
)

for %%I in ("%SOURCE_DIR%") do set "PARENT_DIR=%%~dpI"

set "BUILD_DIR=%SOURCE_DIR%\%CONFIG%"
set "INSTALL_DIR=%PARENT_DIR%install_%PROJECT%\%CONFIG%"

echo.
echo Source        : "%SOURCE_DIR%"
echo Configuration : %CONFIG%
echo Projet        : %PROJECT%
echo Options       : %OPTIONS%
echo Build         : "%BUILD_DIR%"
echo Installation  : "%INSTALL_DIR%"
echo.

cmake -S "%SOURCE_DIR%" -B "%BUILD_DIR%" ^
    -G "Visual Studio 18 2026" -A x64 ^
    "-DCMAKE_INSTALL_PREFIX=%INSTALL_DIR%" ^
    %OPTIONS%

if errorlevel 1 (
    echo Erreur lors de la configuration CMake.
    exit /b 1
)

cmake --build "%BUILD_DIR%" --config "%CONFIG%" --parallel

if errorlevel 1 (
    echo Erreur lors de la compilation.
    exit /b 1
)

cmake --install "%BUILD_DIR%" --config "%CONFIG%"

if errorlevel 1 (
    echo Erreur lors de l'installation.
    exit /b 1
)

echo.
echo Compilation et installation terminees avec succes.
exit /b 0

:usage
echo Usage : %~nx0 "repertoire_source" ^<Debug^|Release^> ^<projet^> "[options CMake]"
echo Exemple :
echo %~nx0 ".\slang" Debug slang "-DBUILD_EXAMPLE=OFF -DENABLE_HLSL=OFF"
exit /b 1