/****************************************************************************
**
** Copyright (C) 2005-2006 Uco Mesdag. All rights reserved.
**
** This file is part of "GT-8 FX FloorBoard".
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along
** with this program; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
**
****************************************************************************/

;NSIS Modern User Interface
;GT-8 FX FloorBoard Script
;Written by Uco Mesdag

  CRCCheck on
  SetCompressor /SOLID /FINAL LZMA
  SetCompress force
  SetDatablockOptimize on

  XPStyle on
  ShowInstDetails hide
  ShowUnInstDetails hide

;--------------------------------
;Include Modern UI

  !define LOGICLIB_SECTIONCMP
  !include "MUI.nsh"
  !include "logiclib.nsh"

;--------------------------------
;General

  ; Name and file
  !define PRODUCT_NAME "GT-8 Fx FloorBoard"
  !ifdef VERSION
    !define PRODUCT_VERSION ${VERSION} ; makensis.exe" /DVERSION=0.0a GT-8FxFloorBoard.nsi"
  !endif
  !ifndef OUT_PATH
    !define OUT_PATH "."
  !endif
  !define PRODUCT_PUBLISHER "Uco Mesdag"
  !define PRODUCT_WEB_SITE "http://fxfloorboard.sourceforge.net/"

;--------------------------------
;Variables

  Var MUI_TEMP
  Var bool

;--------------------------------
;Interface Configuration

  ; MUI Settings
  !define MUI_ABORTWARNING
  !define MUI_ICON "include\nsis_install.ico"
  !define MUI_UNICON "include\nsis_uninstall.ico"
  !define MUI_WELCOMEFINISHPAGE_BITMAP "include\nsis_welcome.bmp"
  !define MUI_UNWELCOMEFINISHPAGE_BITMAP "include\nsis_welcome.bmp"
  ;!define MUI_LICENSEPAGE_BGCOLOR CCFFCC
  
  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_RIGHT
  !define MUI_HEADERIMAGE_BITMAP "include\nsis.bmp"
  !define MUI_COMPONENTSPAGE_SMALLDESC
  
;--------------------------------
;Pages

  ; Welcome page
  !insertmacro MUI_PAGE_WELCOME
  ; License page
  !insertmacro MUI_PAGE_LICENSE "../license.txt"
  ; Components page
  !insertmacro MUI_PAGE_COMPONENTS
  ; Directory page
  !insertmacro MUI_PAGE_DIRECTORY
  ; Instfiles page
  !insertmacro MUI_PAGE_INSTFILES
  ; Finish page
  !define MUI_FINISHPAGE_RUN "$INSTDIR\${PRODUCT_NAME}"
  ;!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\readme.txt"
  !insertmacro MUI_PAGE_FINISH

  ; Uninstaller pages
  !insertmacro MUI_UNPAGE_WELCOME
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "French"
  
;--------------------------------
;Reserve Files

  ;If you are using solid compression, files that are required before
  ;the actual installation should be stored first in the data block,
  ;because this will make your installer start faster.

  !insertmacro MUI_RESERVEFILE_LANGDLL

;--------------------------------
;Local Variables

  ; Application name
  Name "${PRODUCT_NAME}"
  ; Executable name
  !ifdef PRODUCT_VERSION
     OutFile "${OUT_PATH}\${PRODUCT_NAME} v${PRODUCT_VERSION}.exe"
  !else
     OutFile "${OUT_PATH}\${PRODUCT_NAME}.exe"
  !endif
  ; Default installation folder
  InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
  ; Set branding text
  !ifdef PRODUCT_VERSION
     BrandingText "${PRODUCT_NAME} v${PRODUCT_VERSION}"
  !else
     BrandingText "${PRODUCT_NAME}"
  !endif

;--------------------------------
;Installer Sections

Section $(SecMainName) SecMain
  SetOutPath "$INSTDIR"
  ; Files
  /*File /a "G:\Visual Studio 2005\Projects\GT-8 Fx FloorBoard\release\GT-8 Fx FloorBoard.exe"
  File /a "C:\Qt\4.3.0\bin\QtCore4.dll"
  File /a "C:\Qt\4.3.0\bin\QtGui4.dll"
  File /a "C:\Qt\4.3.0\bin\QtXml4.dll"
  File /a "C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86\Microsoft.VC80.CRT\Microsoft.VC80.CRT.manifest"
  File /a "C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86\Microsoft.VC80.CRT\msvcm80.dll"
  File /a "C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86\Microsoft.VC80.CRT\msvcp80.dll"
  File /a "C:\Program Files\Microsoft Visual Studio 8\VC\redist\x86\Microsoft.VC80.CRT\msvcr80.dll"
  File /r /x ".svn" /x "Desktop.ini" "G:\Visual Studio 2005\Projects\GT-8 Fx FloorBoard\Init Patches"*/
  ;File /a "..\release\GT-8FxFloorBoard.exe"
  ;File /a "dependencies\QT4\QtCore4.dll"
  ;File /a "dependencies\QT4\QtGui4.dll"
  ;File /a "dependencies\QT4\QtXml4.dll"
  ;File /a "dependencies\VS2005\Microsoft.VC80.CRT\Microsoft.VC80.CRT.manifest"
  ;File /a "dependencies\VS2005\Microsoft.VC80.CRT\msvcm80.dll"
  ;File /a "dependencies\VS2005\Microsoft.VC80.CRT\msvcp80.dll"
  ;File /a "dependencies\VS2005\Microsoft.VC80.CRT\msvcr80.dll"
  File "/oname=GT-8 Fx FloorBoard.exe" "GT-8FxFloorBoard.exe"
  File /r /x ".svn" /x "Desktop.ini" "..\Init Patches"
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

SectionGroup $(SecGroupShortcutName) SecGroupShortcut
  Section $(SecStartMenuName) SecStartMenu
    ; Create start menu shortcuts
    CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.exe"
    CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    WriteIniStr "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME} Webpage.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  SectionEnd
  Section $(SecDesktopShortCutName) SecDesktopShortCut
    ; Create desktop shortcuts
    CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.exe"
  SectionEnd
  Section $(SecQuickLaunchName) SecQuickLaunch
    ; Create quick launch shortcuts
    CreateShortCut "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" "$INSTDIR\${PRODUCT_NAME}.exe"
  SectionEnd
SectionGroupEnd

Function .onInit
  ; Language selection display
  !insertmacro MUI_LANGDLL_DISPLAY
  ; Set section to selected and readonly
  IntOp $0 ${SF_SELECTED} | ${SF_RO}
  SectionSetFlags ${SecMain} $0
  ; Allow only one installer instance
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "myMutex") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
      MessageBox MB_OK|MB_ICONEXCLAMATION $(InstallerWarning)
      Abort
FunctionEnd

Function .onSelChange
  ; Check if main is sellected
  ${Unless} ${SectionIsSelected} ${SecMain}
    StrCpy $bool "false"
  ${EndUnless}
  ${If} ${SectionIsSelected} ${SecMain}
    StrCpy $bool "true"
  ${EndIf}
  ; Enable/Disable next and options according to main sellection
  ${If} $bool == "false"
    !insertmacro UnSelectSection ${SecQuickLaunch}
    !insertmacro UnSelectSection ${SecDesktopShortCut}
    ; Disable Next button
    GetDlgItem $R0 $HWNDPARENT 1
    EnableWindow $R0 0
  ${ElseIf}  $bool == "true"
    ; Enable Next button
    GetDlgItem $R0 $HWNDPARENT 1
    EnableWindow $R0 1
  ${EndIf}
FunctionEnd

;--------------------------------
;Language strings

  ; English translation
  LangString InstallerWarning ${LANG_ENGLISH} "The installer is already running."
  LangString SecMainName ${LANG_ENGLISH} "Main files"
  LangString SecGroupShortcutName ${LANG_ENGLISH} "Shortcuts"
  LangString SecStartMenuName ${LANG_ENGLISH} "Start menu"
  LangString SecDesktopShortCutName ${LANG_ENGLISH} "Desktop"
  LangString SecQuickLaunchName ${LANG_ENGLISH} "Quick launch"
  LangString DESC_SecMain ${LANG_ENGLISH} "Copy the ${PRODUCT_NAME} main files into the desired folder."
  LangString DESC_SecGroupShortcut ${LANG_ENGLISH} "Creates additional shortcuts."
  LangString DESC_SecStartMenu ${LANG_ENGLISH} "Creates shortcuts in the start menu."
  LangString DESC_SecDesktopShortCut ${LANG_ENGLISH} "Creates shortcuts in the desktop."
  LangString DESC_SecQuickLaunch ${LANG_ENGLISH} "Creates shortcuts in the quick launch."
  ; French translation
  LangString InstallerWarning ${LANG_FRENCH} "Le program d'installation est deja lancer."
  LangString SecMainName ${LANG_FRENCH} "Fichiers principale"
  LangString SecGroupShortcutName ${LANG_FRENCH} "Raccourci"
  LangString SecStartMenuName ${LANG_FRENCH} "Menu démarrer"
  LangString SecDesktopShortCutName ${LANG_FRENCH} "Bureau"
  LangString SecQuickLaunchName ${LANG_FRENCH} "Lancement rapide"
  LangString DESC_SecMain ${LANG_FRENCH} "Copie le fichier principale de ${PRODUCT_NAME} dans le répertoire désiré."
  LangString DESC_SecGroupShortcut ${LANG_FRENCH} "Crée des raccourci suplementaire."
  LangString DESC_SecStartMenu ${LANG_FRENCH} "Crée des raccourci dans menu démarrer."
  LangString DESC_SecDesktopShortCut ${LANG_FRENCH} "Crée des raccourci dans bureau."
  LangString DESC_SecQuickLaunch ${LANG_FRENCH} "Crée des raccourci dans lancement rapide."
  ; Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecGroupShortcut} $(DESC_SecGroupShortcut)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecStartMenu} $(DESC_SecStartMenu)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecQuickLaunch} $(DESC_SecQuickLaunch)
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDesktopShortCut} $(DESC_SecDesktopShortCut)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"
  ; Files
  Delete "$INSTDIR\${PRODUCT_NAME}.exe"
  Delete "$INSTDIR\Uninstall.exe"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\Microsoft.VC80.CRT.manifest"
  Delete "$INSTDIR\msvcm80.dll"
  Delete "$INSTDIR\msvcp80.dll"
  Delete "$INSTDIR\msvcr80.dll"
  Delete "$INSTDIR\Init Patches\*"
  RMDir "$INSTDIR\Init Patches"
  Delete "$INSTDIR\*"  
  RMDir "$INSTDIR"
  ; Delete start menu entry's
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME} Webpage.url"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\*"
  ; Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\${PRODUCT_NAME}"
  startMenuDeleteLoop:
	ClearErrors
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    IfErrors startMenuDeleteLoopDone
    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:
  ; Delete desktop shortcut
   IfFileExists "$DESKTOP\${PRODUCT_NAME}.lnk" 0 +2
    Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  ; Delete quick launch shortcut
  IfFileExists "$QUICKLAUNCH\${PRODUCT_NAME}.lnk" 0 +2
    Delete "$QUICKLAUNCH\${PRODUCT_NAME}.lnk"
SectionEnd

Function un.onInit
  !insertmacro MUI_LANGDLL_DISPLAY
  ; Set section to selected and readonly
  IntOp $0 ${SF_SELECTED} | ${SF_RO}
  SectionSetFlags ${SecMain} $0
  ; Allow only one installer instance
  System::Call 'kernel32::CreateMutexA(i 0, i 0, t "myMutex") i .r1 ?e'
    Pop $R0
    StrCmp $R0 0 +3
      MessageBox MB_OK|MB_ICONEXCLAMATION $(InstallerWarning)
      Abort
FunctionEnd