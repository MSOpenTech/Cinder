//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "targetver.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

// Windows Header Files:
#include <windows.h>

#include <mfapi.h>
#include <mfidl.h>

#include <mfobjects.h>
#include <mfreadwrite.h>

#include <windows.media.h>

#include <string>
#include <map>

#include <wrl.h>
#include <wrl/ftm.h>

#include "ppltasks_winrt.h"
