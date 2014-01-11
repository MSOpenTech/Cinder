Steps to upgrade project for Windows Store App in VC2013:

1. apply patch to:
/Cinder/boost/boost/config/compiler/visualc.hpp

--- HEAD
+++ Modified In Working Tree
@@ -310,8 +310,8 @@
 #endif
 
 //
-// last known and checked version is 1700 (VC11, aka 2011):
-#if (_MSC_VER > 1700)
+// last known and checked version is 1800 (VC13, aka 2013):
+#if (_MSC_VER > 1800)
 #  if defined(BOOST_ASSERT_CONFIG)
 #     error "Unknown compiler version - please run the configure tests and report the results"
 #  else


2. In the Cinder project, add the define: WINRT_XAML

3. solution settings:
c++
    addl include was: ..\..\..\include\winrt\boost;..\..\..\include;..\..\..\boost;%(AdditionalIncludeDirectories)
    preproc: _DEBUG;%(PreprocessorDefinitions);USE_DIRECTX
    not using precomp headers?
linker
    addl dep was: cinder.lib;%(AdditionalDependencies)
    addl libs was: ..\..\..\lib\winrt\$(Platform)\$(Configuration)\;  
    sb: ..\..\..\lib\winrt\$(PlatformTarget)\
Add Existing Project: cinder/winrt_vc2013
retarget for vc2013
Add Ref: cinder
retarget for vc2013
    nb.  copy in icons per manifest, different icons for each platform


