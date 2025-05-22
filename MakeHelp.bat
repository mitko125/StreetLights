@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by TV_TRACERS.HPJ. >"hlp\TV_Tracers.hm"
echo. >>"hlp\TV_Tracers.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\TV_Tracers.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\TV_Tracers.hm"
echo. >>"hlp\TV_Tracers.hm"
echo // Prompts (IDP_*) >>"hlp\TV_Tracers.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\TV_Tracers.hm"
echo. >>"hlp\TV_Tracers.hm"
echo // Resources (IDR_*) >>"hlp\TV_Tracers.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\TV_Tracers.hm"
echo. >>"hlp\TV_Tracers.hm"
echo // Dialogs (IDD_*) >>"hlp\TV_Tracers.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\TV_Tracers.hm"
echo. >>"hlp\TV_Tracers.hm"
echo // Frame Controls (IDW_*) >>"hlp\TV_Tracers.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\TV_Tracers.hm"
REM -- Make help for Project TV_TRACERS


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\TV_Tracers.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\TV_Tracers.hlp" goto :Error
if not exist "hlp\TV_Tracers.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\TV_Tracers.hlp" Debug
if exist Debug\nul copy "hlp\TV_Tracers.cnt" Debug
if exist Release\nul copy "hlp\TV_Tracers.hlp" Release
if exist Release\nul copy "hlp\TV_Tracers.cnt" Release
echo.
goto :done

:Error
echo hlp\TV_Tracers.hpj(1) : error: Problem encountered creating help file

:done
echo.
