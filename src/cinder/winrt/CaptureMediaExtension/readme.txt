The CaptureMediaExtension project was created from a blend of code from the MSDN article at:

http://msdn.microsoft.com/en-us/library/jj872764.aspx

and the demo at:

http://code.msdn.microsoft.com/windowsapps/Media-extensions-sample-7b466096

There seem to be some discrepancies between the two, and several modifications
had to be made to get a working plugin, including handling this
important safety tip about GUID link problems:

http://support.microsoft.com/kb/130869/en-us

Notes:

* Grayscale demo was renamed as Capture (in many places)

* nb. The outer namespace must match the name of the DLL (see the IDL)

	see:
	Creating Windows Runtime Components in C++
	http://msdn.microsoft.com/en-us/library/windows/apps/hh441569.aspx


* It may be necessary to add the following to the Package.appxmanifest file in a test or main app:
	(was NOT needed in C# XAML test app, but WAS needed in C++ Cinder XAML test app)

  </Capabilities>

  <Extensions>
    <Extension Category="windows.activatableClass.inProcessServer">
      <InProcessServer>
        <Path>CaptureMediaExtension.dll</Path>
        <ActivatableClass ActivatableClassId="CaptureMediaExtension.CaptureEffect" ThreadingModel="both" />
      </InProcessServer>
    </Extension>
  </Extensions>

</Package>

* ILDASM can be used to view the contents of a WINMD file:  (this is a .NET tool)

	C:\Program Files (x86)\Microsoft SDKs\Windows\v8.0A\bin\NETFX 4.0 Tools\ildasm.exe
	(or open Visual Studio Command prompt)

command to locate the winmd file:
	dir *.winmd /b/s

