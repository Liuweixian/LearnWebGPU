//
//  AppDelegate.h
//  WebGPURenderGraph
//
//  Created by 刘伟贤 on 2022/5/24.
//

#import <Cocoa/Cocoa.h>
#include "webgpu.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>


@end

static CVReturn displayLinkRepaint(CVDisplayLinkRef dispLink, const CVTimeStamp* inNow, const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext);

static void createPipelineAndBuffers();
static WGPUShaderModule createShader(const char* const code);
static void redraw();
