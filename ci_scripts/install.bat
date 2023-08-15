echo %~dp0\install.bat

cd /d %~dp0

setlocal
if "%CMAKE_COMMAND%" == "" (
   call find_cmake.bat
)

"%CMAKE_COMMAND%" -P ../buildtools/install_cygwin.cmake

if exist c:\msys64\usr\bin\pacman.exe (
  c:\msys64\usr\bin\pacman.exe  -S --noconfirm --needed cmake
)

rem ここで cd しても移動されない
rem install.bat を呼び出した側で戻してもらう
rem cd ..
