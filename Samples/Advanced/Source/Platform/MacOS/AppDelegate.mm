#import <CoreVideo/CoreVideo.h>
#import "AppDelegate.h"
#include <stdio.h>
#include "MacDawnGfxDevice.h"
#include "../../RenderGraph/GfxDevice.h"

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    CVDisplayLinkCreateWithActiveCGDisplays(&self->displayLinkRef);
    CVDisplayLinkSetOutputCallback(self->displayLinkRef, &displayLinkRepaint, nullptr);
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(closeWindow) name:NSWindowWillCloseNotification object:nil];
    
    CreateGfxDevice([&](GfxDevice*& pDevice){
        pDevice = new MacDawnGfxDevice(self.window);
    });
    
    GetGfxDevice()->Initialize();
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
    CVDisplayLinkStart(self->displayLinkRef);
}

- (void)applicationDidResignActive:(NSNotification *)notification {
}

- (void)closeWindow {
    CVDisplayLinkStop(self->displayLinkRef);
    [NSApp terminate:self];
}

@end

static CVReturn displayLinkRepaint(CVDisplayLinkRef dispLink, const CVTimeStamp* inNow, const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    return kCVReturnSuccess;
}
