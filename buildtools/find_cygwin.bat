chcp 65001 >nul
rem output
rem     CYGWIN_PATH     cygwinのbinフォルダ

set CYGWIN_PATH=%~dp0..\buildtools\cygwin64\bin
if exist "%CYGWIN_PATH%\cygwin1.dll" exit /b 0
set CYGWIN_PATH=C:\cygwin64\bin
if exist "%CYGWIN_PATH%\cygwin1.dll" exit /b 0
set CYGWIN_PATH=C:\cygwin\bin
if exist "%CYGWIN_PATH%\cygwin1.dll" exit /b 0
set CYGWIN_PATH=
exit /b 0
