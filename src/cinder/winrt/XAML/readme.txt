Notes for the CinderXAML template as of 1/10/2014

This template allows the developer to easily create Cinder applications that run inside a
DirectX window (called a "SwapChainPanel") in the XAML page.  It has been updated for
Windows 8.1.

Tested with basicAppDX sample on Win32 and Simulator.

Pending tests: Surface tablet device at Windows 8.1, more samples

Usage:

1.	Replace the file "basicAppDX.cpp" with your own Cinder application, following the usual the Cinder coding guidelines.

2.	Clean and build the solution.

Information:

*   If needed, the XAML content can be modified in the "Cinder XAML App pages", however "CinderPage" has been modified from
	the original WIndows Store XAML template.

*	Files in the "Generated Files" folder are dynamically written and changed; please do not touch.

*	Likely build failure causes:

	- please check the preprocessor include paths, in particular the number of ..\ expressions
	- please insure the vc2013_winrt\cinder.vcproj is added a build dependency (use Add Reference to do this)
	- please check that Boost has been patch as described in vc2013_winrt\readme.txt
	- please insure that this symbol is defined in both projects: WINRT_XAML

[eof]
