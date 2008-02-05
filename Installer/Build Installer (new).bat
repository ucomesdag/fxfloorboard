@ECHO OFF

title -=Build Installer=-

:: -----------------------------------------------------------------------------------------
:Version
SET /P Version=Enter the version number:
IF "%Version%" == ""  (GOTO OutPath)
CLS

:: -----------------------------------------------------------------------------------------
:Revision
SET /P Revision=Enter the build/revision number:
IF "%Revision%" == "" (GOTO Revision)
CLS

:: -----------------------------------------------------------------------------------------
:OutPath
echo Enter the path where you want to save the file [default: ./]:
set /p OutPath=
::IF "%OutPath%" == "" (GOTO OutPath)
CLS

:: -----------------------------------------------------------------------------------------
IF NOT "%Version%" == "" (GOTO label1) ELSE (GOTO label3)

:: -----------------------------------------------------------------------------------------
:label1 
IF "%OutPath%" == "" (GOTO label2)
ECHO Version is set to: %Version%-build%Revision% and the file will be saved to: %OutPath%
ECHO.
PAUSE
GOTO compress


:: -----------------------------------------------------------------------------------------
:label2 
ECHO Version is set to: %Version%-build%Revision%
ECHO.
PAUSE
GOTO compress

:: -----------------------------------------------------------------------------------------
:label3 
IF "%OutPath%" == "" (GOTO label4)
ECHO File will be saved to: %OutPath%
ECHO.
PAUSE
GOTO compress

:: -----------------------------------------------------------------------------------------
:label4
ECHO.
GOTO compress


:: -----------------------------------------------------------------------------------------
:compress
del GT-8FxFloorBoard.exe
cls
ECHO Compressing EXE
@"C:\upx300w\upx.exe" --best -o "GT-8FxFloorBoard.exe" "..\Release\GT-8FxFloorBoard.exe"
CLS

:: -----------------------------------------------------------------------------------------
IF NOT "%Version%" == "" (GOTO label1sub) ELSE (GOTO label3sub)

:: -----------------------------------------------------------------------------------------
:label1sub 
IF "%OutPath%" == "" (GOTO label2sub)
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" /DVERSION="%Version%-build%Revision%" /DOUT_PATH="%OutPath%" GT-8FxFloorBoard.nsi"
copy /Y /B "%OutPath%\GT-8 FX FloorBoard v%Version%-build%Revision%.exe"  "%OutPath%\GT-8FxFloorBoard-v%Version%-build%Revision%.exe"
del "%OutPath%\GT-8 FX FloorBoard v%Version%-build%Revision%.exe"
GOTO result

:: -----------------------------------------------------------------------------------------
:label2sub 
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" /DVERSION="%Version%-build%Revision%" GT-8FxFloorBoard.nsi"
copy /Y /B "GT-8 FX FloorBoard v%Version%-build%Revision%.exe"  GT-8FxFloorBoard-v%Version%-build%Revision%.exe
del "GT-8 FX FloorBoard v%Version%-build%Revision%.exe"
GOTO result

:: -----------------------------------------------------------------------------------------
:label3sub 
IF "%OutPath%" == "" (GOTO label4sub)
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" /DOUT_PATH="%OutPath%" GT-8FxFloorBoard.nsi"
copy /Y /B "%OutPath%\GT-8 FX FloorBoard.exe"  "%OutPath%\GT-8FxFloorBoard.exe"
del "%OutPath%\GT-8 FX FloorBoard.exe"
GOTO result

:: -----------------------------------------------------------------------------------------
:label4sub 
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" GT-8FxFloorBoard.nsi"
copy /Y /B "GT-8 FX FloorBoard.exe"  GT-8FxFloorBoard.exe
del "GT-8 FX FloorBoard.exe"
GOTO result

:: -----------------------------------------------------------------------------------------
:result

cls
IF NOT "%Version%" == "" (
	IF NOT "%OutPath%" == "" (
	echo Created installer: GT-8FxFloorBoard-v%Version%-build%Revision%.exe in %OutPath%
	) ELSE (echo Created installer: GT-8FxFloorBoard-v%Version%-build%Revision%.exe)
) ELSE (
	IF NOT "%OutPath%" == "" (
	echo Created installer: GT-8FxFloorBoard.exe in %OutPath%
	) ELSE (echo Created installer: GT-8FxFloorBoard.exe)
)
echo.
echo Finished
echo.
PAUSE