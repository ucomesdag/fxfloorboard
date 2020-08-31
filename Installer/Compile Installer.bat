@ECHO OFF

title -=Compile Installer=-

:Version
ECHO Enter the version number [default: none]: (exmpl: 0.1a-build0001)
SET /P Version=

::IF "%Version%" == "" (GOTO Version)

CLS

:OutPath
echo Enter the path where you want to save the file [default: ./]:
set /p OutPath=

::IF "%OutPath%" == "" (GOTO OutPath)

CLS

IF NOT "%Version%" == "" (GOTO label1) ELSE (GOTO label3)

:label1 
IF "%OutPath%" == "" (GOTO label2)
ECHO Version is set to: %Version% and the file will be saved to: %OutPath%
ECHO.
PAUSE
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" /DVERSION="%Version%" /DOUT_PATH="%OutPath%" GT-8FxFloorBoard.nsi"
copy /Y /B "%OutPath%\GT-8 FX FloorBoard v%Version%.exe"  "%OutPath%\GT-8FxFloorBoard-v%Version%.exe"
del "%OutPath%\GT-8 FX FloorBoard v%Version%.exe"
GOTO result

:label2 
ECHO Version is set to: %Version%
ECHO.
PAUSE
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" /DVERSION="%Version%" GT-8FxFloorBoard.nsi"
copy /Y /B "GT-8 FX FloorBoard v%Version%.exe"  GT-8FxFloorBoard-v%Version%.exe
del "GT-8 FX FloorBoard v%Version%.exe"
GOTO result

:label3 
IF "%OutPath%" == "" (GOTO label4)
ECHO File will be saved to: %OutPath%
ECHO.
PAUSE
ECHO Compilling installer
@"C:\Program Files\NSIS\makensis.exe" /DOUT_PATH="%OutPath%" GT-8FxFloorBoard.nsi"
copy /Y /B "%OutPath%\GT-8 FX FloorBoard.exe"  "%OutPath%\GT-8FxFloorBoard.exe"
del "%OutPath%\GT-8 FX FloorBoard.exe"
GOTO result

:label4 
ECHO Compilling installer
ECHO.
@"C:\Program Files\NSIS\makensis.exe" GT-8FxFloorBoard.nsi"
copy /Y /B "GT-8 FX FloorBoard.exe"  GT-8FxFloorBoard.exe
del "GT-8 FX FloorBoard.exe"
GOTO result


:result

cls
IF NOT "%Version%" == "" (
	IF NOT "%OutPath%" == "" (
	echo Created installer: GT-8FxFloorBoard-v%Version%.exe in %OutPath%
	) ELSE (echo Created installer: GT-8FxFloorBoard-v%Version%.exe)
) ELSE (
	IF NOT "%OutPath%" == "" (
	echo Created installer: GT-8FxFloorBoard.exe in %OutPath%
	) ELSE (echo Created installer: GT-8FxFloorBoard.exe)
)
echo.
echo Finished
echo.
PAUSE