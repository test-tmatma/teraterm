setlocal
rem PATH=c:\cygwin\bin
PATH=c:\cygwin64\bin
dir  c:\cygwin64\bin
uname -a > build_info.txt
echo make -j cygterm+-i686
make -j cygterm+-i686
echo make -j cygterm+-x86_64
make -j cygterm+-x86_64
file cygterm+-i686/*.exe >> build_info.txt
file cygterm+-x86_64/*.exe >> build_info.txt
pause
