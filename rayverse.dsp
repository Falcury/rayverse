# Microsoft Developer Studio Project File - Name="rayverse" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=rayverse - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "rayverse.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "rayverse.mak" CFG="rayverse - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "rayverse - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "rayverse - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "rayverse - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /TP /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib winmm.lib /nologo /entry:"mainCRTStartup" /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "rayverse - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /TP /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib winmm.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "rayverse - Win32 Release"
# Name "rayverse - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\action.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\affiche.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\araignee.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\audiocd.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\basic.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\bat.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\bbmont.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\blacktoon.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\blocs.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\blood.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\bonus.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\box.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\breakout.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\cage.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\clown.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\collision.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\compress.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\cymbal.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\dark.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\data.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\display.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\effect.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\engine.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\fee.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\frame.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\guetteur.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\hifi.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\hybrid.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\input.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\intrinsics.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\jumelle.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\linux_main.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\linux_opengl.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\linux_sound.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\load.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\lowlevel.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\luciole.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\map.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mathutil.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\menu.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\meredenis.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\misc_source.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\mite.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\moskito.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\moteur.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\neige.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ngawe.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\objinit.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\objupdate.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\pcmain.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\pmama.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\poing.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\poisson.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\rand.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ray.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\rayverse.c
# End Source File
# Begin Source File

SOURCE=.\render.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\save.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\saxo.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\screen.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\skops.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sound.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\special.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\stb_vorbis.c
# End Source File
# Begin Source File

SOURCE=.\stoneman.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\sysutils.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\trumpet.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ufo.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\win32_main.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\win32_opengl.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\win32_sound.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\worldmap.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\proto.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
