Notes for CaptureApp using Cinder adapted to XAML as of 2/4/2014

Updated for Windows 8.1.

Information:

*	Files in the "Generated Files" folder are dynamically written and changed; please do not touch.

*	Likely build failure causes, please check the following:

	- preprocessor include paths, in particular the number of ..\ expressions
	- insure the vc2013_winrt\cinder.vcproj is added as a build dependency (use Add Reference to do this)
	- check that Boost has been patched as described in vc2013_winrt\readme.txt
	- insure that this symbol is defined in both projects: WINRT_XAML

[eof]
