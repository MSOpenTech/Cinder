// The copyright in this software is being made available under the BSD License, included below. 
// This software may be subject to other third party and contributor rights, including patent rights, 
// and no such rights are granted under this license.
//
// Copyright (c) 2014, Microsoft Open Technologies, Inc. 
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// - Neither the name of Microsoft Open Technologies, Inc. nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, 
// INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#ifndef __INPUT_EVENT__
#define __INPUT_EVENT__

#include <agile.h>


namespace cinder { namespace app {

class CinderMain;


enum PointerEventType
{
    PointerPressed,
    PointerMoved,
    PointerReleased,
};

enum XamlKeyEvent
{
    KeyDown,
    KeyUp
};


class InputEvent
{
public:
    InputEvent() {};
    virtual ~InputEvent() {};
    virtual void execute(CinderMain* cinder) = 0;
};

class PointerEvent : public InputEvent
{
public:
    PointerEvent(PointerEventType type, Windows::UI::Core::PointerEventArgs^ args);
    virtual void execute(CinderMain* cinder);
    ~PointerEvent() {};

private:
    PointerEventType m_type;
    Platform::Agile<Windows::UI::Core::PointerEventArgs> m_args;
};

class KeyboardEvent : public InputEvent

{
public:
    KeyboardEvent(XamlKeyEvent type, Windows::UI::Core::KeyEventArgs^ args);
    virtual void execute(CinderMain* cinder);
    ~KeyboardEvent() {};

private:
    XamlKeyEvent m_type;
    Platform::Agile<Windows::UI::Core::KeyEventArgs^> m_args;
};


}
}
#endif // #ifndef __INPUT_EVENT__

