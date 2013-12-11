This project was created as follows:

ERRORS - WORK IN PROCESS

1. Create a new project in VC2013 using the template:
Windows Store, type DirectX, named basicApp

2. settings:

elim link error: ?
BOOST_LIB_TOOLSET vc110

General
    Output:	    $(SolutionDir)$(Configuration)\
    Intermediate:   $(Configuration)\
    Platform:	    VC2013 (v12)
Project
    Application (.exe)
    Use Unicode Character Set (not available in VC2013)

C++
    ..\..\..\include;..\..\..\boost;%(AdditionalIncludeDirectories)
    WIN32;_DEBUG;_WINDOWS;%(PreprocessorDefinitions)
    CONFLICT:	was Multi-threaded Debug (/MTd), now MdD
    Opt: Disabled (/Od)
    Min Rebuild: Yes (/Gm)
    Not Using Precompiled Headers (temp)
Linker
    Yes (/INCREMENTAL)
    ..\..\..\lib\msw\$(PlatformTarget)

. remove the generated App and AppMain .cpp and .h files from the project
. remove Content and Common files

. Modified to follow Cinder framework

basicAppDX_wip.cpp

WORK IN PROGRESS

current errors:

1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: bool __thiscall cinder::app::Window::isFullScreen(void)const " (?isFullScreen@Window@app@cinder@@QBE_NXZ) referenced in function "public: bool __thiscall cinder::app::App::isFullScreen(void)const " (?isFullScreen@App@app@cinder@@QBE_NXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: void __thiscall cinder::app::Window::setFullScreen(bool,struct cinder::app::FullScreenOptions const &)" (?setFullScreen@Window@app@cinder@@QAEX_NABUFullScreenOptions@23@@Z) referenced in function "public: void __thiscall cinder::app::App::setFullScreen(bool,struct cinder::app::FullScreenOptions const &)" (?setFullScreen@App@app@cinder@@QAEX_NABUFullScreenOptions@23@@Z)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: void __thiscall cinder::app::Window::setPos(class cinder::Vec2<int> const &)const " (?setPos@Window@app@cinder@@QBEXABV?$Vec2@H@3@@Z) referenced in function "public: virtual void __thiscall cinder::app::App::setWindowPos(class cinder::Vec2<int> const &)" (?setWindowPos@App@app@cinder@@UAEXABV?$Vec2@H@3@@Z)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual void __thiscall cinder::app::App::privateSetup__(void)" (?privateSetup__@App@app@cinder@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual void __thiscall cinder::app::App::privateUpdate__(void)" (?privateUpdate__@App@app@cinder@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "protected: static void __cdecl cinder::app::App::prepareLaunch(void)" (?prepareLaunch@App@app@cinder@@KAXXZ) referenced in function "public: static void __cdecl cinder::app::AppBasic::prepareLaunch(void)" (?prepareLaunch@AppBasic@app@cinder@@SAXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "protected: static void __cdecl cinder::app::App::cleanupLaunch(void)" (?cleanupLaunch@App@app@cinder@@KAXXZ) referenced in function "public: static void __cdecl cinder::app::AppBasic::cleanupLaunch(void)" (?cleanupLaunch@AppBasic@app@cinder@@SAXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: __thiscall cinder::app::AppBasic::AppBasic(void)" (??0AppBasic@app@cinder@@QAE@XZ) referenced in function "public: __thiscall BasicApp::BasicApp(void)" (??0BasicApp@@QAE@XZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: virtual __thiscall cinder::app::AppBasic::~AppBasic(void)" (??1AppBasic@app@cinder@@UAE@XZ) referenced in function __unwindfunclet$??0BasicApp@@QAE@XZ$0
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual float __thiscall cinder::app::AppBasic::getFrameRate(void)const " (?getFrameRate@AppBasic@app@cinder@@UBEMXZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual void __thiscall cinder::app::AppBasic::setFrameRate(float)" (?setFrameRate@AppBasic@app@cinder@@UAEXM@Z)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual void __thiscall cinder::app::AppBasic::disableFrameRate(void)" (?disableFrameRate@AppBasic@app@cinder@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual bool __thiscall cinder::app::AppBasic::isFrameRateEnabled(void)const " (?isFrameRateEnabled@AppBasic@app@cinder@@UBE_NXZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual void __thiscall cinder::app::AppBasic::quit(void)" (?quit@AppBasic@app@cinder@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual class std::tr2::sys::basic_path<class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >,struct std::tr2::sys::path_traits> __thiscall cinder::app::AppBasic::getAppPath(void)const " (?getAppPath@AppBasic@app@cinder@@UBE?AV?$basic_path@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@Upath_traits@sys@tr2@2@@sys@tr2@std@@XZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual class std::shared_ptr<class cinder::app::Window> __thiscall cinder::app::AppBasic::getWindow(void)const " (?getWindow@AppBasic@app@cinder@@UBE?AV?$shared_ptr@VWindow@app@cinder@@@std@@XZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual unsigned int __thiscall cinder::app::AppBasic::getNumWindows(void)const " (?getNumWindows@AppBasic@app@cinder@@UBEIXZ)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual class std::shared_ptr<class cinder::app::Window> __thiscall cinder::app::AppBasic::getWindowIndex(unsigned int)const " (?getWindowIndex@AppBasic@app@cinder@@UBE?AV?$shared_ptr@VWindow@app@cinder@@@std@@I@Z)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual class std::shared_ptr<class cinder::app::Window> __thiscall cinder::app::AppBasic::getForegroundWindow(void)const " (?getForegroundWindow@AppBasic@app@cinder@@UBE?AV?$shared_ptr@VWindow@app@cinder@@@std@@XZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: static void __cdecl cinder::app::AppBasic::executeLaunch(class cinder::app::AppBasic *,class std::shared_ptr<class cinder::app::Renderer>,char const *)" (?executeLaunch@AppBasic@app@cinder@@SAXPAV123@V?$shared_ptr@VRenderer@app@cinder@@@std@@PBD@Z) referenced in function "?main@@YAHP$01$AAV?$Array@P$AAVString@Platform@@$00@Platform@@@Z" (?main@@YAHP$01$AAV?$Array@P$AAVString@Platform@@$00@Platform@@@Z)
1>basicAppDX_wip.obj : error LNK2001: unresolved external symbol "public: virtual void __thiscall cinder::app::AppBasic::launch(char const *,int,char * const * const)" (?launch@AppBasic@app@cinder@@UAEXPBDHQBQAD@Z)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "public: __thiscall cinder::app::RendererDx::RendererDx(int)" (??0RendererDx@app@cinder@@QAE@H@Z) referenced in function "?main@@YAHP$01$AAV?$Array@P$AAVString@Platform@@$00@Platform@@@Z" (?main@@YAHP$01$AAV?$Array@P$AAVString@Platform@@$00@Platform@@@Z)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "void __cdecl cinder::dx::clear(class cinder::ColorAT<float> const &,bool)" (?clear@dx@cinder@@YAXABV?$ColorAT@M@2@_N@Z) referenced in function "public: virtual void __thiscall BasicApp::draw(void)" (?draw@BasicApp@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "void __cdecl cinder::dx::begin(unsigned int)" (?begin@dx@cinder@@YAXI@Z) referenced in function "public: virtual void __thiscall BasicApp::draw(void)" (?draw@BasicApp@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "void __cdecl cinder::dx::end(void)" (?end@dx@cinder@@YAXXZ) referenced in function "public: virtual void __thiscall BasicApp::draw(void)" (?draw@BasicApp@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "void __cdecl cinder::dx::vertex(class cinder::Vec2<float> const &)" (?vertex@dx@cinder@@YAXABV?$Vec2@M@2@@Z) referenced in function "public: virtual void __thiscall BasicApp::draw(void)" (?draw@BasicApp@@UAEXXZ)
1>basicAppDX_wip.obj : error LNK2019: unresolved external symbol "void __cdecl cinder::dx::color(float,float,float)" (?color@dx@cinder@@YAXMMM@Z) referenced in function "public: virtual void __thiscall BasicApp::draw(void)" (?draw@BasicApp@@UAEXXZ)
1>C:\Users\daver_000\Documents\GitHub\Cinder\samples\BasicApp\vc2013_win81_dx\Debug\basicApp\basicApp.exe : fatal error LNK1120: 27 unresolved externals


1>libboost_filesystem-vc110-mt-sgd-1_53.lib(path_traits.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_filesystem-vc110-mt-sgd-1_53.lib(path.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_filesystem-vc110-mt-sgd-1_53.lib(operations.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_filesystem-vc110-mt-sgd-1_53.lib(codecvt_error_category.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_filesystem-vc110-mt-sgd-1_53.lib(windows_file_codecvt.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_system-vc110-mt-sgd-1_53.lib(error_code.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_thread-vc110-mt-sgd-1_53.lib(thread.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>libboost_thread-vc110-mt-sgd-1_53.lib(tss_pe.obj) : error LNK2038: mismatch detected for '_MSC_VER': value '1700' doesn't match value '1800' in basicApp.obj
1>C:\Users\daver_000\Documents\GitHub\Cinder\samples\BasicApp\vc2012\Debug\basicApp.exe : fatal error LNK1319: 8 mismatches detected
