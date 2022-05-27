#import <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    CVDisplayLinkRef displayLinkRef;
}
@property(strong) IBOutlet NSWindow *window;
@end

static CVReturn displayLinkRepaint(CVDisplayLinkRef dispLink, const CVTimeStamp *inNow, const CVTimeStamp *inOutputTime, CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *displayLinkContext);
