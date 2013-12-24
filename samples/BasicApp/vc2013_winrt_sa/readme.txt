This is a standalone version of "tiny cinder"
used to work out the Windows OS contract to the Application.

The outstanding issues to solve are:
* threading
* event handling
* sequencing
* simplify class structure within existing Cinder framework

All files will be integrated back into
full Cinder once these issues are solved.

This is an interim minimal subsystem that uses
only the source needed to create and run
the XAML DirectX test application "BasicApp"
Boost dependency is also minimal.

Structure:

cinder_src/	files selectively copied in from Cinder/src to build tiny Cinder
include/	header files from Cinder/include
project/	VC2013 project files, intermediate, and output
			files from the Windows Store XAML sample as work in process
			these are adapted and migrated into src and then Cinder

