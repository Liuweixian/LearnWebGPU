#import <CoreVideo/CoreVideo.h>
#import "AppDelegate.h"
#include <stdio.h>
#include "MacDawnGfxDevice.h"
#include "../../RenderGraph/GfxDevice.h"
#include "../../RenderGraph/RenderGraphPlayer.h"
#include "../../Graph/SimpleRenderGraph.h"
#include "../../FileSystem/Resources.h"

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
    
    GetResources()->SetRootDirectory([[[NSBundle mainBundle] resourcePath] UTF8String]);
    
    RenderGraphPlayer *pRDGPlayer = GetRDGPlayer();
    // Create RenderObject into RenderGraphPlayer
    RenderObject *pRenderObject = pRDGPlayer->AddRenderObject<RenderObject>("Triganle");
    // Create Material for RenderObject
    pRenderObject->CreateMaterial<RenderMaterial>(RenderPassIdx::DrawObjectPass);
    RenderMesh *pMesh = pRenderObject->CreateMesh<RenderMesh>();
    // Create VBO for RenderObject
    RenderBuffer *pRenderVertexBuffer = pMesh->CreateVertexBuffer<RenderBuffer>();
    //(x,y,r,g,b)
    float vertexData[15] = {
        -0.8f, -0.8f, 0.0f, 0.0f, 1.0f, // BL
         0.8f, -0.8f, 0.0f, 1.0f, 0.0f, // BR
        -0.0f,  0.8f, 1.0f, 0.0f, 0.0f, // top
    };
    pRenderVertexBuffer->SetData<float>(15, vertexData);
    // Create IBO for RenderObject
    RenderBuffer *pRenderIndexBuffer = pMesh->CreateIndexBuffer<RenderBuffer>();
    uint16_t indexData[4] = {0, 1, 2, 0};
    pRenderIndexBuffer->SetData<uint16_t>(4, indexData);

    pRDGPlayer->RunGraph<SimpleRenderGraph>();
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
