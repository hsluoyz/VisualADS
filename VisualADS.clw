; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VisualADS.h"
LastPage=0

ClassCount=8
Class1=CVisualADSApp
Class2=CVisualADSDoc
Class3=CVisualADSView
Class4=CMainFrame
Class12=COutputBar

ResourceCount=16
Resource1=IDR_MAINFRAME (Chinese (P.R.C.))
Resource2=IDR_THEME_MENU
Resource8=IDR_CONTEXT_MENU (English (U.S.))
Resource9=IDD_DIALOG2
Class5=CAboutDlg
Class6=CWorkspaceBar
Class7=CWorkspaceBar2
Resource10=IDR_CONTEXT_MENU (Chinese (P.R.C.))
Resource11=IDD_ABOUTBOX (Chinese (P.R.C.))
Resource12=IDR_PASTE_MENU (Chinese (P.R.C.))
Resource13=IDR_THEME_MENU (Chinese (P.R.C.))
Resource14="IDD_DIALOG1"
Resource15=IDD_ABOUTBOX
Resource3=IDR_CONTEXT_MENU
Resource4=IDR_PASTE_MENU
Resource5=IDR_MAINFRAME (English (U.S.))
Resource6=IDR_PASTE_MENU (English (U.S.))
Resource7=IDR_THEME_MENU (English (U.S.))
Class8=CVisualADSAboutDlg
Resource16=IDR_MAINFRAME

[CLS:CVisualADSApp]
Type=0
HeaderFile=VisualADS.h
ImplementationFile=VisualADS.cpp
Filter=W
LastObject=CVisualADSApp
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CVisualADSDoc]
Type=0
HeaderFile=VisualADSDoc.h
ImplementationFile=VisualADSDoc.cpp
Filter=M
LastObject=CVisualADSDoc
BaseClass=CFODoc
VirtualFilter=DC

[CLS:CVisualADSView]
Type=0
HeaderFile=VisualADSView.h
ImplementationFile=VisualADSView.cpp
Filter=W
BaseClass=CFODrawView
VirtualFilter=VWC
LastObject=CVisualADSView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=D
BaseClass=CBCGPFrameWnd
VirtualFilter=fWC
LastObject=ID_FILE_OPEN




[CLS:CAboutDlg]
Type=0
HeaderFile=VisualADS.cpp
ImplementationFile=VisualADS.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CVisualADSAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_COMPANY_URL,button,1342242816
Control6=IDC_ABOUT_EDIT,edit,1350637636

[CLS:CWorkspaceBar]
Type=0
HeaderFile=WorkspaceBar.h
ImplementationFile=WorkspaceBar.cpp
Filter=W

[CLS:CWorkspaceBar2]
Type=0
HeaderFile=WorkspaceBar2.h
ImplementationFile=WorkspaceBar2.cpp
Filter=W

[CLS:COutputBar]
Type=0
HeaderFile=OutputBar.h
ImplementationFile=OutputBar.cpp
Filter=W

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_STATUS_BAR
Command15=ID_VIEW_APPLOOK_2000
Command16=ID_VIEW_APPLOOK_XP
Command17=ID_VIEW_APPLOOK_2003
Command18=ID_VIEW_APPLOOK_VS2005
Command19=ID_VIEW_APPLOOK_WIN_XP
Command20=ID_VIEW_APPLOOK_2007
Command21=ID_VIEW_APPLOOK_2007_1
Command22=ID_VIEW_APPLOOK_2007_2
Command23=ID_VIEW_APPLOOK_2007_3
Command24=ID_VIEW_APPLOOK_VS2008
Command25=ID_VIEW_APPLOOK_VS2010
Command26=ID_VIEW_APPLOOK_2010_1
Command27=ID_VIEW_APPLOOK_2010_2
Command28=ID_VIEW_APPLOOK_2010_3
Command29=ID_VIEW_APPLOOK_WIN7
Command30=ID_APP_ABOUT
CommandCount=30

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[MNU:IDR_CONTEXT_MENU]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
CommandCount=3

[MNU:IDR_PASTE_MENU]
Type=1
Class=?
Command1=ID_EDIT_PASTE
Command2=ID_EDIT_PASTE_SPECIAL
CommandCount=2

[MNU:IDR_THEME_MENU]
Type=1
Class=?
Command1=ID_VIEW_APPLOOK_2007
Command2=ID_VIEW_APPLOOK_2007_1
Command3=ID_VIEW_APPLOOK_2007_2
Command4=ID_VIEW_APPLOOK_2007_3
Command5=ID_VIEW_APPLOOK_2010_1
Command6=ID_VIEW_APPLOOK_2010_2
Command7=ID_VIEW_APPLOOK_2010_3
Command8=ID_VIEW_APPLOOK_WIN7
CommandCount=8

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_STATUS_BAR
Command15=ID_VIEW_APPLOOK_2000
Command16=ID_VIEW_APPLOOK_XP
Command17=ID_VIEW_APPLOOK_2003
Command18=ID_VIEW_APPLOOK_VS2005
Command19=ID_VIEW_APPLOOK_WIN_XP
Command20=ID_VIEW_APPLOOK_2007
Command21=ID_VIEW_APPLOOK_2007_1
Command22=ID_VIEW_APPLOOK_2007_2
Command23=ID_VIEW_APPLOOK_2007_3
Command24=ID_VIEW_APPLOOK_VS2008
Command25=ID_VIEW_APPLOOK_VS2010
Command26=ID_VIEW_APPLOOK_2010_1
Command27=ID_VIEW_APPLOOK_2010_2
Command28=ID_VIEW_APPLOOK_2010_3
Command29=ID_VIEW_APPLOOK_WIN7
Command30=ID_APP_ABOUT
CommandCount=30

[MNU:IDR_CONTEXT_MENU (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
CommandCount=3

[MNU:IDR_PASTE_MENU (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_PASTE
Command2=ID_EDIT_PASTE_SPECIAL
CommandCount=2

[MNU:IDR_THEME_MENU (English (U.S.))]
Type=1
Class=?
Command1=ID_VIEW_APPLOOK_2007
Command2=ID_VIEW_APPLOOK_2007_1
Command3=ID_VIEW_APPLOOK_2007_2
Command4=ID_VIEW_APPLOOK_2007_3
Command5=ID_VIEW_APPLOOK_2010_1
Command6=ID_VIEW_APPLOOK_2010_2
Command7=ID_VIEW_APPLOOK_2010_3
Command8=ID_VIEW_APPLOOK_WIN7
CommandCount=8

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (Chinese (P.R.C.))]
Type=1
Class=CVisualADSAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_COMPANY_URL,button,1342242816

[MNU:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_STATUS_BAR
Command15=ID_VIEW_APPLOOK_2000
Command16=ID_VIEW_APPLOOK_XP
Command17=ID_VIEW_APPLOOK_2003
Command18=ID_VIEW_APPLOOK_VS2005
Command19=ID_VIEW_APPLOOK_WIN_XP
Command20=ID_VIEW_APPLOOK_2007
Command21=ID_VIEW_APPLOOK_2007_1
Command22=ID_VIEW_APPLOOK_2007_2
Command23=ID_VIEW_APPLOOK_2007_3
Command24=ID_VIEW_APPLOOK_VS2008
Command25=ID_VIEW_APPLOOK_VS2010
Command26=ID_VIEW_APPLOOK_2010_1
Command27=ID_VIEW_APPLOOK_2010_2
Command28=ID_VIEW_APPLOOK_2010_3
Command29=ID_VIEW_APPLOOK_WIN7
Command30=ID_APP_ABOUT
CommandCount=30

[MNU:IDR_CONTEXT_MENU (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
CommandCount=3

[MNU:IDR_PASTE_MENU (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_EDIT_PASTE
Command2=ID_EDIT_PASTE_SPECIAL
CommandCount=2

[MNU:IDR_THEME_MENU (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_VIEW_APPLOOK_2007
Command2=ID_VIEW_APPLOOK_2007_1
Command3=ID_VIEW_APPLOOK_2007_2
Command4=ID_VIEW_APPLOOK_2007_3
Command5=ID_VIEW_APPLOOK_2010_1
Command6=ID_VIEW_APPLOOK_2010_2
Command7=ID_VIEW_APPLOOK_2010_3
Command8=ID_VIEW_APPLOOK_WIN7
CommandCount=8

[ACL:IDR_MAINFRAME (Chinese (P.R.C.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_DIALOG2]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:"IDD_DIALOG1"]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CVisualADSAboutDlg]
Type=0
HeaderFile=VisualADSAboutDlg.h
ImplementationFile=VisualADSAboutDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_COMPANY_URL
VirtualFilter=dWC

