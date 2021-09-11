/*
 *  Copyright 2019-2021 Diligent Graphics LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF ANY PROPRIETARY RIGHTS.
 *
 *  In no event and under no legal theory, whether in tort (including negligence), 
 *  contract, or otherwise, unless required by applicable law (such as deliberate 
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental, 
 *  or consequential damages of any character arising as a result of this License or 
 *  out of the use or inability to use the software (including but not limited to damages 
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and 
 *  all other commercial damages or losses), even if such Contributor has been advised 
 *  of the possibility of such damages.
 */

#include <emscripten.h>
#include <emscripten/html5.h>

#include <memory>
#include <string>

#include "NativeAppBase.hpp"
#include "Timer.hpp"

std::unique_ptr<Diligent::NativeAppBase> g_pTheApp  = nullptr;
Diligent::Timer                          g_Timer    = {};
double                                   g_PrevTime = 0.0;

void EventLoopCallback()
{
    auto CurrTime    = g_Timer.GetElapsedTime();
    auto ElapsedTime = CurrTime - g_PrevTime;
    g_PrevTime       = CurrTime;

    if (g_pTheApp->IsReady())
    {
        g_pTheApp->Update(CurrTime, ElapsedTime);
        g_pTheApp->Render();
    }
}


EM_BOOL EventResizeCallback(int32_t EventType, const EmscriptenUiEvent* UIEvent, void* UserData)
{
    if (g_pTheApp->IsReady())
    {
        double Width  = 0;
        double Height = 0;
        emscripten_get_element_css_size("#canvas", &Width, &Height);
        g_pTheApp->WindowResize(static_cast<int32_t>(Width), static_cast<int32_t>(Height));
    }
    return true;
}


int main(int argc, char* argv[])
{
    g_pTheApp.reset(Diligent::CreateApplication());

    int32_t WindowWidth  = 0;
    int32_t WindowHeight = 0;

    std::string CmdLine = "-width 1920 -height 1280 show-ui";
    g_pTheApp->ProcessCommandLine(CmdLine.c_str());
    g_pTheApp->GetDesiredInitialWindowSize(WindowWidth, WindowHeight);

    emscripten_set_canvas_element_size("#canvas", WindowWidth, WindowHeight);
    g_pTheApp->OnWindowCreated("#canvas", WindowWidth, WindowHeight);

    emscripten_set_resize_callback("#canvas", nullptr, false, EventResizeCallback);
    emscripten_set_main_loop(EventLoopCallback, 0, true);

    g_pTheApp.reset();
}