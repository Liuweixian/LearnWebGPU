//
//  AppDelegate.m
//  WebGPURenderGraph
//
//  Created by 刘伟贤 on 2022/5/24.
//

#import "AppDelegate.h"
#include <stdio.h>
#import <CoreVideo/CoreVideo.h>

@interface AppDelegate ()

@property (strong) IBOutlet NSWindow *window;
@end

CVDisplayLinkRef _displayLinkRef;
@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    // Insert code here to initialize your application
    CVDisplayLinkCreateWithActiveCGDisplays(&_displayLinkRef);
    CVDisplayLinkSetOutputCallback(_displayLinkRef, displayLinkRepaint, NULL);
}


- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}


- (BOOL)applicationSupportsSecureRestorableState:(NSApplication *)app {
    return YES;
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {
    CVDisplayLinkStart(_displayLinkRef);
}

- (void)applicationDidResignActive:(NSNotification *)notification {
    CVDisplayLinkStop(_displayLinkRef);
}

@end

static CVReturn displayLinkRepaint(CVDisplayLinkRef dispLink, const CVTimeStamp* inNow, const CVTimeStamp* inOutputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    return kCVReturnSuccess;
}
