@echo off
goto Start

REM %1=Configuration %2=Platform %3=solution %4=directory
:Build_Sub
setlocal
pushd %4
REM msbuild qscintilla.sln /t:Build /p:Configuration=%1;Platform=%2 /nodeReuse:False
devenv %3.sln /build "%1|%2"
popd
endlocal
goto :EOF

:Build_32
setlocal
pushd .
call "%BB_VC_DIR%"\vcvarsall.bat x86
popd
call :Build_List Win32
endlocal
goto :EOF

:Build_64
setlocal
pushd .
call "%BB_VC_DIR%"\vcvarsall.bat amd64
popd
call :Build_List x64
endlocal
goto :EOF

REM %1=Platform
:Build_List
echo ********************************************
echo ****    ccLib
echo ********************************************
call :Build_Sub Debug_RTL %1 ccLib ".\"
call :Build_Sub Release_RTL %1 ccLib ".\"
call :Build_Sub Debug_Static %1 ccLib ".\"
call :Build_Sub Release_Static %1 ccLib ".\"
goto :EOF

REM ******************************************
REM ****    Start
REM ******************************************
:Start
setlocal
set BB_VC_DIR=F:\Vs2010\VC
call :Build_32
call :Build_64
popd
endlocal
