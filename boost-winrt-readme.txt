
* please apply boost_submodule.patch to modify boost submodule for VC2013

* this is a minimal patch made to visualc.hpp to allow boost 1.53 to compile under vc2013

* vc2013 solution and project files placed in new vc2013 folder

* in project settings, C++/General, 
  verify that Platform Toolset is set to: Visual Studio 2013 (v120)
  (this is the default)

* status: Cinder builds with vc2013

