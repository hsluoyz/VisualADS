# Microsoft Developer Studio Project File - Name="VisualADS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=VisualADS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VisualADS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VisualADS.mak" CFG="VisualADS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VisualADS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "VisualADS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VisualADS - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "VisualADS - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "VisualADS - Win32 Release"
# Name "VisualADS - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ADCNDPGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\ADComputer.cpp
# End Source File
# Begin Source File

SOURCE=.\ADDirectorySearch.cpp
# End Source File
# Begin Source File

SOURCE=.\ADDomain.cpp
# End Source File
# Begin Source File

SOURCE=.\ADDotReader.cpp
# End Source File
# Begin Source File

SOURCE=.\ADE.cpp
# End Source File
# Begin Source File

SOURCE=.\ADGraphGenerator.cpp
# End Source File
# Begin Source File

SOURCE=.\ADGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\ADObject.cpp
# End Source File
# Begin Source File

SOURCE=.\ADOU.CPP
# End Source File
# Begin Source File

SOURCE=.\ADPermissionSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\ADShareFolder.cpp
# End Source File
# Begin Source File

SOURCE=.\ADUser.cpp
# End Source File
# Begin Source File

SOURCE=.\ADWMISearch.cpp
# End Source File
# Begin Source File

SOURCE=.\CPing.cpp
# End Source File
# Begin Source File

SOURCE=.\DLInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\DLRacerInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\FOHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\FOPGDIPlusHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\FOPGDIPlusProxy.cpp
# End Source File
# Begin Source File

SOURCE=.\Hook.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MyMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\MyRibbonEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MyShape.cpp
# End Source File
# Begin Source File

SOURCE=.\OutputBar.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertiesViewBar.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketComm.cpp
# End Source File
# Begin Source File

SOURCE=.\SocketManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "VisualADS - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "VisualADS - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VisualADS.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualADS.rc
# End Source File
# Begin Source File

SOURCE=.\VisualADSAboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualADSDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualADSView.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.cpp
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar2.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ADCNDPGenerator.h
# End Source File
# Begin Source File

SOURCE=.\ADComputer.h
# End Source File
# Begin Source File

SOURCE=.\ADDirectorySearch.h
# End Source File
# Begin Source File

SOURCE=.\ADDomain.h
# End Source File
# Begin Source File

SOURCE=.\ADDotReader.h
# End Source File
# Begin Source File

SOURCE=.\ADE.h
# End Source File
# Begin Source File

SOURCE=.\ADGraphGenerator.h
# End Source File
# Begin Source File

SOURCE=.\ADGroup.h
# End Source File
# Begin Source File

SOURCE=.\ADObject.h
# End Source File
# Begin Source File

SOURCE=.\ADOU.H
# End Source File
# Begin Source File

SOURCE=.\ADPermissionSearch.h
# End Source File
# Begin Source File

SOURCE=.\ADShareFolder.h
# End Source File
# Begin Source File

SOURCE=.\ADUser.h
# End Source File
# Begin Source File

SOURCE=.\ADWMISearch.h
# End Source File
# Begin Source File

SOURCE=.\CPing.h
# End Source File
# Begin Source File

SOURCE=.\DirUtils.h
# End Source File
# Begin Source File

SOURCE=.\DLInterface.h
# End Source File
# Begin Source File

SOURCE=.\DLRacerInterface.h
# End Source File
# Begin Source File

SOURCE=.\DoWithAPIHook.h
# End Source File
# Begin Source File

SOURCE=.\FOHyperLink.h
# End Source File
# Begin Source File

SOURCE=.\Hook.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MessageHook.h
# End Source File
# Begin Source File

SOURCE=.\MyMessageBox.h
# End Source File
# Begin Source File

SOURCE=.\MyRibbonEdit.h
# End Source File
# Begin Source File

SOURCE=.\MyShape.h
# End Source File
# Begin Source File

SOURCE=.\OutputBar.h
# End Source File
# Begin Source File

SOURCE=.\PropertiesViewBar.h
# End Source File
# Begin Source File

SOURCE="D:\Program Files\Microsoft SDK\include\PropIdl.h"
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SocketComm.h
# End Source File
# Begin Source File

SOURCE=.\SocketManager.h
# End Source File
# Begin Source File

SOURCE=.\SplashWindow.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stdstring.h
# End Source File
# Begin Source File

SOURCE=.\StringA.h
# End Source File
# Begin Source File

SOURCE=.\StringW.h
# End Source File
# Begin Source File

SOURCE=.\VisualADS.h
# End Source File
# Begin Source File

SOURCE=.\VisualADSAboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\VisualADSDoc.h
# End Source File
# Begin Source File

SOURCE=.\VisualADSView.h
# End Source File
# Begin Source File

SOURCE=.\WinGraphviz.h
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar.h
# End Source File
# Begin Source File

SOURCE=.\WorkspaceBar2.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bin00001.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00002.bin
# End Source File
# Begin Source File

SOURCE=.\res\bin00003.bin
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\compsres.bin
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor2.cur
# End Source File
# Begin Source File

SOURCE=.\icon\icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\icon\icon54x54.ico
# End Source File
# Begin Source File

SOURCE=.\res\idb_ribb.bin
# End Source File
# Begin Source File

SOURCE=.\res\Splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\splash\Splash_old.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar256.bmp
# End Source File
# Begin Source File

SOURCE=.\icon\VisualADS.ico
# End Source File
# Begin Source File

SOURCE=.\res\VisualADS.ico
# End Source File
# Begin Source File

SOURCE=.\res\VisualADS.rc2
# End Source File
# Begin Source File

SOURCE=.\res\VisualADSDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\workspace.bmp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Source File

SOURCE=.\symbols\1\big_computer.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\big_computer.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\big_computer.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\1\computer.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\computer.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\computer.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\computer_frame.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\frame\computer_frame.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\1\domain.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\domain.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\domain.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\1\group.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\group.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\group.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\frame\group_frame.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\group_frame.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\1\ou.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\ou.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\ou.sid
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\symbols\1\resource.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\resource.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\resource.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\frame\resource_frame.sid
# End Source File
# Begin Source File

SOURCE=.\symbols\resource_frame.sid
# End Source File
# Begin Source File

SOURCE=.\res\ribbonfilelarge.png
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\ribbonfilesmall.png
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\ribbonhomelarge.png
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\ribbonhomesmall.png
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\ribbonicons.png
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\res\ribbonmain.png
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\symbols\1\user.sid
# End Source File
# Begin Source File

SOURCE=".\symbols\new(gradient_without_shadow)\user.sid"
# End Source File
# Begin Source File

SOURCE=.\symbols\user.sid
# End Source File
# Begin Source File

SOURCE=.\xptheme.xml
# End Source File
# End Target
# End Project
