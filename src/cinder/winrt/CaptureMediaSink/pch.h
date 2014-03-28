//
// pch.h
// Header for standard system include files.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers
#include <windows.h>

#include <mfapi.h>
#include <mfidl.h>

#include <mfobjects.h>
#include <mfreadwrite.h>
#include <mferror.h>

#include <ppltasks.h>
#include <collection.h>

// #include <roerrorapi.h>
// #include <windows.media.h>

// #include <algorithm>
// #include <windows.foundation.h>
// #include <collection.h>
// using namespace Windows::Foundation::Collections;
// using namespace Platform::Collections;

// #include <mftransform.h>

#if 0

#include <new>
#include <mfapi.h>
#include <mftransform.h>
#include <mfidl.h>
#include <mferror.h>
#include <strsafe.h>
#include <assert.h>

// Note: The Direct2D helper library is included for its 2D matrix operations.
#include <D2d1helper.h>

#include <wrl\implements.h>
#include <wrl\module.h>
#include <windows.media.h>
#endif
