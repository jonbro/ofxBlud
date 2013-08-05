#pragma once

#include <MediaPlayer/MediaPlayer.h>
#import "ofxiPhoneExtras.h"
#import "lunar.h"
#import "bludLock.h"

// forward declaration

class bludVideoPlayer;

// need a bare nsobject to grab the notifications about the video state

@interface bludVideoPlayerNotifier : NSObject

{
	bludVideoPlayer *bvp;
}
- (id) init: (bludVideoPlayer *) _bvp;
- (void)movieFinishedCallback:(NSNotification*)notification;
@end


class bludVideoPlayer {
public:
	static const char className[];
	static Lunar<bludVideoPlayer>::RegType methods[];
	bludVideoPlayer(lua_State *L);
	int log(lua_State *L){
		NSLog(@"%@", [NSString stringWithUTF8String:luaL_checkstring(L, 1)]);
		return 1;
	}
	int load (lua_State *L);
	int removeView(lua_State *L){
		if(viewLoaded){
			[theMovie.view removeFromSuperview];
        }
		return 1;
	}
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
	~bludVideoPlayer() {
		// i suspect that to properly cleanup, I need to pop the value of the callback off the stack
		// not going to do it now though, because how much memory could this possibly leak...
        cout << "removing the video player" << endl;
        if(viewLoaded){
            cout << "releasing notifier" << endl;
            [[NSNotificationCenter defaultCenter] 
             removeObserver:notifier
             name:MPMoviePlayerPlaybackDidFinishNotification
             object:theMovie];
            [notifier release];
            [theMovie release];
            theMovie = nil;
        }
	}
private:
	int completionCallback;
	bool hasCompletionCallback;
	MPMoviePlayerController* theMovie;
	bludVideoPlayerNotifier *notifier;
	lua_State *_L;
	bool viewLoaded;
    ofMutex *mutex;
};