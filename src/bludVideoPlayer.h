#pragma once

#include <MediaPlayer/MediaPlayer.h>
#import "ofxiPhoneExtras.h"
#import "lunar.h"

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
	bludVideoPlayer(lua_State *L) {
		notifier = [[bludVideoPlayerNotifier alloc] init:this];
		hasCompletionCallback = false;
		_L = L;
		viewLoaded = false;
	}
	int log(lua_State *L){
		NSLog(@"%@", [NSString stringWithUTF8String:luaL_checkstring(L, 1)]);
		return 1;
	}
	int load (lua_State *L) {
		
		CGSize size = [ofxiPhoneGetUIWindow() frame].size;
		
		NSURL *theURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String: luaL_checkstring(L, 1)]];
		//NSURL *theURL = [NSURL URLWithString:@"http://webapp-net.com/Demo/Media/sample_iPod.m4v"];  
		
		
		theMovie = [[MPMoviePlayerController alloc] initWithContentURL:theURL];
		
		[theMovie prepareToPlay];  
		
		// > 3.2
		[theMovie respondsToSelector:@selector(loadState)];
		
		theMovie.scalingMode = MPMovieScalingModeAspectFill;
		[theMovie setFullscreen:TRUE animated:TRUE];
		theMovie.controlStyle = MPMovieControlStyleNone;
		
		theMovie.view.frame = CGRectMake(0, 0, size.height, size.width); 
		theMovie.view.backgroundColor = [UIColor clearColor];
		
		// Transform
		theMovie.view.transform = CGAffineTransformMakeRotation(-270.0f * (M_PI/180.0f));
		theMovie.view.center = ofxiPhoneGetUIWindow().center;
		
		
		
		
		// setup the completion callback
		[[NSNotificationCenter defaultCenter] 
		 addObserver:notifier
		 selector:@selector(movieFinishedCallback:)
		 name:MPMoviePlayerPlaybackDidFinishNotification
		 object:nil];
		
		[theMovie play];
		[ofxiPhoneGetUIWindow() addSubview:theMovie.view];
		[ofxiPhoneGetUIWindow() makeKeyAndVisible];
		viewLoaded = true;
		return 1;
	}
	int removeView(lua_State *L){
		if(viewLoaded)
			[theMovie.view removeFromSuperview];
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
	void callCompletion(){
		if(hasCompletionCallback){
			lua_rawgeti( _L, LUA_REGISTRYINDEX, completionCallback );
			if(lua_pcall(_L, 0, 0, 0) != 0){
				ofLog(OF_LOG_ERROR, "video completion callback error");
				ofLog(OF_LOG_ERROR, lua_tostring(_L, -1));
			}			
		}
	}
	~bludVideoPlayer() {
		// i suspect that to properly cleanup, I need to pop the value of the callback off the stack
		// not going to do it now though, because how much memory could this possibly leak...
		
	}
private:
	int completionCallback;
	bool hasCompletionCallback;
	MPMoviePlayerController* theMovie;
	bludVideoPlayerNotifier *notifier;
	lua_State *_L;
	bool viewLoaded;
};