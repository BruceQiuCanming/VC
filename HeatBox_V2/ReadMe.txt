========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : HeatBox
========================================================================


AppWizard has created this HeatBox application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your HeatBox application.

HeatBox.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

HeatBox.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CHeatBoxApp application class.

HeatBox.cpp
    This is the main application source file that contains the application
    class CHeatBoxApp.

HeatBox.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

HeatBox.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\HeatBox.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file HeatBox.rc.

res\HeatBox.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.




/////////////////////////////////////////////////////////////////////////////

AppWizard creates one dialog class:

HeatBoxDlg.h, HeatBoxDlg.cpp - the dialog
    These files contain your CHeatBoxDlg class.  This class defines
    the behavior of your application's main dialog.  The dialog's
    template is in HeatBox.rc, which can be edited in Microsoft
	Visual C++.


/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named HeatBox.pch and a precompiled types file named StdAfx.obj.

Resource.h
    This is the standard header file, which defines new resource IDs.
    Microsoft Visual C++ reads and updates this file.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" to indicate parts of the source code you
should add to or customize.

If your application uses MFC in a shared DLL, and your application is 
in a language other than the operating system's current language, you
will need to copy the corresponding localized resources MFC42XXX.DLL
from the Microsoft Visual C++ CD-ROM onto the system or system32 directory,
and rename it to be MFCLOC.DLL.  ("XXX" stands for the language abbreviation.
For example, MFC42DEU.DLL contains resources translated to German.)  If you
don't do this, some of the UI elements of your application will remain in the
language of the operating system.

/////////////////////////////////////////////////////////////////////////////

本软件从2011年4月20日开始设计
作者： Bruce Qiu
电话： 013915838598
QQ:    228154900
E-Mail:cmqiu1969@163.com
http://www.cnblogs.com/cmqiu


2011-6-3 05:08上午
1 修改了拔板时的内存泄露，其会导致系统崩溃
2 拔板增加单选和多选功能
3 退出系统时，提示需要终止正在进行的测试
4 进入测试时，显示测试条件
5 强制终止测试时进行提示，由操作员进行再确认

2011-7-19 11:45
1 修改程序内存的泄漏
2 修改程序长期运行时死机的问题，也就是系统内存的泄漏