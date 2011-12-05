#pragma once

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "ofxiPhoneExtras.h"
#import "lunar.h"
#import "bludLock.h"

// forward declaration

class bludWebview;

// need a bare nsobject to grab the notifications about the video state

@interface bludWebviewController : UIViewController <UIWebViewDelegate>
{
	UIWebView *webView;
    bool hide;
    @public
        bludWebview *bwv;
}
-(id)init:(bludWebview *) _bwv file:(NSString *)toLoad;
@end


class bludWebview {
public:
	static const char className[];
	static Lunar<bludWebview>::RegType methods[];
	bludWebview(lua_State *L);
    int setCompletionCallback(lua_State *L){
		if(lua_isfunction(L, 1)){
			// push the value of the function (that is at position 1), onto the top of the stack
			lua_pushvalue(L, 1);
			// store this stack position in the registry index
			completionCallback = luaL_ref(L, LUA_REGISTRYINDEX);
			hasCompletionCallback = true;
		}
		return 1;
	}
    void callCompletion();
	~bludWebview() {
        // release the webview controller
        [web release];
	}
private:
    ofMutex *mutex;
    int completionCallback;
	bool hasCompletionCallback;
    lua_State *_L;
    bludWebviewController *web;
};