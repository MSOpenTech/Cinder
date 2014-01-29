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


#include "app/winrt/xaml/InputEvent.h"
#include "app/winrt/xaml/CinderMain.h"

namespace cinder {
    namespace app {

#if 0
        PointerEvent::PointerEvent(PointerEventType type, Windows::UI::Core::PointerEventArgs^ args)
            : m_type(type), m_args(args)
        {

        }


        void PointerEvent::execute(CinderMain* cinder)
        {
            switch (m_type)
            {
            case PointerEventType::PointerPressed:
                cinder->OnPointerPressed(m_args.Get());
                break;
            case PointerEventType::PointerMoved:
                cinder->OnPointerMoved(m_args.Get());
                break;
            case PointerEventType::PointerReleased:
                cinder->OnPointerReleased(m_args.Get());
                break;
            }
        }
#endif

        KeyboardEvent::KeyboardEvent(XamlKeyEvent type, Windows::UI::Core::KeyEventArgs^ args)
            : m_type(type), m_args(args)
        {

        }

        void KeyboardEvent::execute(CinderMain* cinder)
        {
            switch (m_type)
            {
            case XamlKeyEvent::KeyDown:
                cinder->ProcessOnKeyDown(m_args.Get());
                break;
            case XamlKeyEvent::KeyUp:
                cinder->ProcessOnKeyDown(m_args.Get());
                break;
            }
        }
    }
}


