/*
 *  Copyright 2019-2021 Diligent Graphics LLC
 *  Copyright 2015-2019 Egor Yusov
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

#include <GLFW/glfw3.h>
#include <emscripten.h>
#include <memory>

#include "NativeAppBase.hpp"

std::unique_ptr<Diligent::NativeAppBase> g_pTheApp;

void EventLoopCallback()
{
    g_pTheApp->Update();
    g_pTheApp->Render();
    g_pTheApp->Present();
}

int main(int argc, char* argv[])
{
    const auto WINDOW_TITLE  = "Emscripten";
    const auto WINDOW_WIDTH  = 1920;
    const auto WINDOW_HEIGHT = 1280;

    glfwInit();
    std::unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)> pWindow(
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr),
        glfwDestroyWindow);

    g_pTheApp.reset(Diligent::CreateApplication());
    g_pTheApp->Initialize();
    emscripten_set_main_loop(EventLoopCallback, 0, true);
    g_pTheApp.reset();
    glfwTerminate();
}