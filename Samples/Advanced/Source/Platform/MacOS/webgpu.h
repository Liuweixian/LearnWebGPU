/**
 * \file webgpu.h
 * WebGPU/Dawn wrapper.
 */
#pragma once

#include <webgpu/webgpu.h>
#import <AppKit/NSWindow.h>

namespace webgpu
{
    WGPUDevice create(NSWindow* window, WGPUBackendType type = WGPUBackendType_Force32);

    WGPUSwapChain createSwapChain(WGPUDevice device);

    WGPUTextureFormat getSwapChainFormat(WGPUDevice device);
}
