@ECHO off
path = %~dp0..\..\%
pushd %path%
PAUSE
IF EXIST "%path%bin\" (
	del "%path%bin\" /f /s /q
	del "%path%bin-int\" /f /s /q
	rmdir "%path%bin\" /s /q
	rmdir "%path%bin-int\" /s /q
	)
PAUSE
IF EXIST "%path%\kLibrary\bin\" (
	del "%path%\kLibrary\bin\" /f /s /q
	del "%path%\kLibrary\bin-int\" /f /s /q
	rmdir "%path%\kLibrary\bin\" /s /q
	rmdir "%path%\kLibrary\bin-int\" /s /q
)
call Vendors\bin\premake\premake5.exe vs2019
popd
PAUSE