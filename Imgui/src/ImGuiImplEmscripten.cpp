/*
 *  Copyright 2019-2021 Diligent Graphics LLC
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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

#include "imgui.h"
#include "ImGuiImplEmscripten.hpp"


namespace Diligent
{

ImGuiImplEmscripten::ImGuiImplEmscripten(IRenderDevice* pDevice,
                                         TEXTURE_FORMAT BackBufferFmt,
                                         TEXTURE_FORMAT DepthBufferFmt,
                                         Uint32         InitialVertexBufferSize,
                                         Uint32         InitialIndexBufferSize) :
    ImGuiImplDiligent(pDevice, BackBufferFmt, DepthBufferFmt, InitialVertexBufferSize, InitialIndexBufferSize)
{
    ImGuiIO& io            = ImGui::GetIO();
    io.BackendPlatformName = "Diligent-ImGuiImplEmscripten";
}

ImGuiImplEmscripten::~ImGuiImplEmscripten()
{
}

void ImGuiImplEmscripten::NewFrame(Uint32            RenderSurfaceWidth,
                                   Uint32            RenderSurfaceHeight,
                                   SURFACE_TRANSFORM SurfacePreTransform)
{
    auto now        = std::chrono::high_resolution_clock::now();
    auto elapsed_ns = now - m_LastTimestamp;
    m_LastTimestamp = now;
    auto& io        = ImGui::GetIO();
    io.DeltaTime    = static_cast<float>(elapsed_ns.count() / 1e+9);
    io.DisplaySize  = ImVec2(RenderSurfaceWidth, RenderSurfaceHeight);
    ImGuiImplDiligent::NewFrame(RenderSurfaceWidth, RenderSurfaceHeight, SurfacePreTransform);
}

void ImGuiImplEmscripten::Render(IDeviceContext* pCtx)
{
    ImGuiImplDiligent::Render(pCtx);
}

} // namespace Diligent
