========================================================================
       MICROSOFT FOUNDATION CLASS LIBRARY : TV_Tracers
========================================================================


AppWizard has created this TV_Tracers application for you.  This application
not only demonstrates the basics of using the Microsoft Foundation classes
but is also a starting point for writing your application.

This file contains a summary of what you will find in each of the files that
make up your TV_Tracers application.

TV_Tracers.dsp
    This file (the project file) contains information at the project level and
    is used to build a single project or subproject. Other users can share the
    project (.dsp) file, but they should export the makefiles locally.

TV_Tracers.h
    This is the main header file for the application.  It includes other
    project specific headers (including Resource.h) and declares the
    CTV_TracersApp application class.

TV_Tracers.cpp
    This is the main application source file that contains the application
    class CTV_TracersApp.

TV_Tracers.rc
    This is a listing of all of the Microsoft Windows resources that the
    program uses.  It includes the icons, bitmaps, and cursors that are stored
    in the RES subdirectory.  This file can be directly edited in Microsoft
	Visual C++.

TV_Tracers.clw
    This file contains information used by ClassWizard to edit existing
    classes or add new classes.  ClassWizard also uses this file to store
    information needed to create and edit message maps and dialog data
    maps and to create prototype member functions.

res\TV_Tracers.ico
    This is an icon file, which is used as the application's icon.  This
    icon is included by the main resource file TV_Tracers.rc.

res\TV_Tracers.rc2
    This file contains resources that are not edited by Microsoft 
	Visual C++.  You should place all resources not editable by
	the resource editor in this file.



/////////////////////////////////////////////////////////////////////////////

For the main frame window:

MainFrm.h, MainFrm.cpp
    These files contain the frame class CMainFrame, which is derived from
    CMDIFrameWnd and controls all MDI frame features.

res\Toolbar.bmp
    This bitmap file is used to create tiled images for the toolbar.
    The initial toolbar and status bar are constructed in the CMainFrame
    class. Edit this toolbar bitmap using the resource editor, and
    update the IDR_MAINFRAME TOOLBAR array in TV_Tracers.rc to add
    toolbar buttons.
/////////////////////////////////////////////////////////////////////////////

For the child frame window:

ChildFrm.h, ChildFrm.cpp
    These files define and implement the CChildFrame class, which
    supports the child windows in an MDI application.

/////////////////////////////////////////////////////////////////////////////

AppWizard creates one document type and one view:

TV_TracersDoc.h, TV_TracersDoc.cpp - the document
    These files contain your CTV_TracersDoc class.  Edit these files to
    add your special document data and to implement file saving and loading
    (via CTV_TracersDoc::Serialize).

TV_TracersView.h, TV_TracersView.cpp - the view of the document
    These files contain your CTV_TracersView class.
    CTV_TracersView objects are used to view CTV_TracersDoc objects.

res\TV_TracersDoc.ico
    This is an icon file, which is used as the icon for MDI child windows
    for the CTV_TracersDoc class.  This icon is included by the main
    resource file TV_Tracers.rc.

/////////////////////////////////////////////////////////////////////////////

Help Support:

hlp\TV_Tracers.hpj
    This file is the Help Project file used by the Help compiler to create
    your application's Help file.

hlp\*.bmp
    These are bitmap files required by the standard Help file topics for
    Microsoft Foundation Class Library standard commands.

hlp\*.rtf
    This file contains the standard help topics for standard MFC
    commands and screen objects.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named TV_Tracers.pch and a precompiled types file named StdAfx.obj.

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



За управление на Улично осветлрние
V0_1 поставено в Твърдица, но е грешно, не преобразува зададените времена в BCD

V0_2 е първо работно в Твърдица

V1_0 подредени са настройките

V1_1 добавен е Тест LQI проверява качеството на връзката по дървото на рутер.

Добавени Руски и Украински

V1_1 eng добавен е и английски, но се повреди украинския. да се използва V1_01_Ru_Uk_01

V1_2 оправениса грешките в Българския и Украинския (да се използва тази версия)

V1_3 добавени са геограваска дължина и ширина, тип на осветитля и забележка

V1_4 добаавен калкулатор за изгрев, залез


V2_0 добавен е втори таймер за вкл./изкл. заради висока цена на тока (светят само вечер и сутрин)
работи и с V1 на контролера (като се съгласва и на 4 байта по-кратко съобщение)
при tsConfigBorderRuter не се интерицсува за грешка при sModuleConfigV11.u8Channel == 0
преправяне в CHardwareCom зарди къси буфери и неизичакване на пълен пакет
!!! Jennic приема само до 400 байта

V3_0 Добавен електромер,
оправена грешка при изтриване на системния отчет търсеше и acount_devices
добавена е допълнителна диагностика на SIM, но може да работи и V1.1 на концентраторите
добавени са reset на SIM u8INT_resetGPRShours,u8INT_resetGPRSminuts разрешение за електромера u8EnableEnergyMeter,
но не са направени потрбителски интрфейси. Инсталиран в Твърдица с горните =0,
Закоментарени са:
//	CEnergyMeters CEM;
//	AddPage(&CEM); за да не се виждат електромерите
Някои неще не са преведени (сомо на английски са)
Добавен интерфейс за u8INT_resetGPRShours,u8INT_resetGPRSminuts разрешение за електромера u8EnableEnergyMeter
Преведен на Български.
След 3 минути се прекъсва връзката към рутерите
Преправени са много TIMEOUT за връзките
направен е PC сървър който се регистрира в рутерите и получава съобщения от тях SET_MY_IP_TO_CLIENT
Добавено е автоматично настройване на 1 таймер и записването му в рутерите в понед.,сряда,петък на обяд

V4_0 Прехвърлен е от VC6++ на Visual Studio 2019

Достъп в Средец:програма "Anydesk"; потребите "141532224"; парола "Parola321$"

да се преправи в HOST Ver. версиите на u32HostVersion от %2X на %d