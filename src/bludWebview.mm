#include "bludWebview.h"

const char bludWebview::className[] = "bludWebview";

Lunar<bludWebview>::RegType bludWebview::methods[] = {
    method(bludWebview, setCompletionCallback),
	{0,0}
};

@implementation bludWebviewController

-(id)init:(bludWebview *) _bwv file:(NSString *)toLoad
{
	self = [super init];
    bwv = _bwv;
    CGSize size = [ofxiPhoneGetUIWindow() frame].size;
	webView = [[UIWebView alloc] initWithFrame:CGRectMake(0, 0, size.width, size.height)];
	NSString *path = [[NSBundle mainBundle] pathForResource:toLoad ofType:@"html" inDirectory:@"help"];
	NSFileHandle *readHandle = [NSFileHandle fileHandleForReadingAtPath:path];
	NSURL *baseURL = [NSURL fileURLWithPath:path];
	
	NSString *htmlString = [[NSString alloc] initWithData: 
							[readHandle readDataToEndOfFile] encoding:NSUTF8StringEncoding];
	webView.delegate = self;
	webView.scalesPageToFit = YES;
    webView.backgroundColor = [UIColor colorWithRed:0.968 green:0.960 blue:0.935 alpha:0.000];
    
	[webView loadHTMLString:htmlString baseURL:baseURL];
    [ofxiPhoneGetUIWindow() addSubview:webView];
    [ofxiPhoneGetUIWindow() makeKeyAndVisible];
	self->hide = false;
    
	return self;
}
- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
	NSString *requestString = [[request URL] absoluteString];
	NSArray *components = [requestString componentsSeparatedByString:@":"];
	if ([components count] > 1){
        if([(NSString *)[components objectAtIndex:0] isEqualToString:@"plb"]){
            // strip out the slashes
            NSArray *functionComponents = [[(NSString *)[components objectAtIndex:1] stringByTrimmingCharactersInSet:[NSCharacterSet characterSetWithCharactersInString:@"/"]] componentsSeparatedByString:@"/"];		
            if([(NSString *)[functionComponents objectAtIndex:0] isEqualToString:@"exitBrowser"]) 
            {
                [self->webView removeFromSuperview];
                bwv->callCompletion();
                return NO;
            }
        }else if([(NSString *)[components objectAtIndex:0] isEqualToString:@"safari"]){
            NSString *correctUrl = [NSString stringWithFormat:@"http:%@", [components objectAtIndex:1]];
            return ![[UIApplication sharedApplication] openURL:[NSURL URLWithString:correctUrl]]; 
        }
	}
	return YES; // Return YES to make sure regular navigation works as expected.
}
-(oneway void)release
{
    [webView release];
    [super release];
}
@end

bludWebview::bludWebview(lua_State *L){
    // instance a webview controller
    web = [[bludWebviewController alloc] init:this file:[NSString stringWithUTF8String:luaL_checkstring(L, 1)]];
    //        web->bwv = this;
    // attach its view to the main of view
    hasCompletionCallback = false;
    mutex = bludLock::getInstance();
    _L = L;
}

void bludWebview::callCompletion(){
    if(hasCompletionCallback){
        mutex->lock();
        lua_rawgeti( _L, LUA_REGISTRYINDEX, completionCallback );
        if(lua_pcall(_L, 0, 0, 0) != 0){
            ofLog(OF_LOG_ERROR, "video completion callback error");
            ofLog(OF_LOG_ERROR, lua_tostring(_L, -1));
        }			
        mutex->unlock();
    }
}
