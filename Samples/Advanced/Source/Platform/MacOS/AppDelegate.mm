#import <CoreVideo/CoreVideo.h>
#import "AppDelegate.h"
#include <stdio.h>
#include "MacDawnGfxDevice.h"
#include "../../RenderGraph/GfxDevice.h"
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../Graph/DrawTriangleRenderGraph/DrawTriangleRenderGraph.h"
#include "../../FileSystem/FileSystem.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    CVDisplayLinkCreateWithActiveCGDisplays(&self->displayLinkRef);
    CVDisplayLinkSetOutputCallback(self->displayLinkRef, &displayLinkRepaint, (__bridge void*)self);
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closeWindow) name:NSWindowWillCloseNotification object:nil];
    CVDisplayLinkStart(self->displayLinkRef);
    
    CreateGfxDevice([&](GfxDevice*& pDevice){
        pDevice = new MacDawnGfxDevice(self.window);
    });
    
    GetFileSystem()->SetRootDirectory([[[NSBundle mainBundle] resourcePath] UTF8String]);
    
    RenderGraphPlayer *pRDGPlayer = GetRDGPlayer();
    pRDGPlayer->RunGraph<DrawTriangleRenderGraph>();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
}

- (void)applicationDidResignActive:(NSNotification *)notification {
}

- (void)closeWindow {
    CVDisplayLinkStop(self->displayLinkRef);
    [NSApp terminate:self];
}

- (void)rendering{
    bool looping = GetRDGPlayer()->RenderLoop();
    if (!looping)
        CVDisplayLinkStop(self->displayLinkRef);
}

@end

static CVReturn displayLinkRepaint(CVDisplayLinkRef dispLink, const CVTimeStamp* inNow, const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    AppDelegate* pWindow = (__bridge AppDelegate*)displayLinkContext;
    [pWindow performSelectorOnMainThread:@selector(rendering) withObject:nil waitUntilDone:YES];
    return kCVReturnSuccess;
}
