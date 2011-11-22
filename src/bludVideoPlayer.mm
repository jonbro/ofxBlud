#include "bludVideoPlayer.h"

const char bludVideoPlayer::className[] = "bludVideoPlayer";

Lunar<bludVideoPlayer>::RegType bludVideoPlayer::methods[] = {
	method(bludVideoPlayer, load),
	method(bludVideoPlayer, setCompletionCallback),
	method(bludVideoPlayer, removeView),
	method(bludVideoPlayer, log),
	{0,0}
};

@implementation bludVideoPlayerNotifier

//--------------------------------------------------------------
- (id) init: (bludVideoPlayer *) _bvp
{
	if(self = [super init])
	{
		bvp = _bvp;
	}
	return self;
}
- (void)movieFinishedCallback:(NSNotification*)notification
{
	bvp->callCompletion();
	NSLog(@"movie complete");
}
@end

void bludVideoPlayer::callCompletion(){
    [theMovie pause];
    theMovie.initialPlaybackTime = -1.0;
    [theMovie stop];
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

int bludVideoPlayer::load(lua_State *L){
    CGSize size = [ofxiPhoneGetUIWindow() frame].size;
    
    NSURL *theURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String: luaL_checkstring(L, 1)]];
    //NSURL *theURL = [NSURL URLWithString:@"http://webapp-net.com/Demo/Media/sample_iPod.m4v"];  
    
    
    theMovie = [[MPMoviePlayerController alloc] initWithContentURL:theURL];
    NSLog(@"1.url retain count = %i", [theURL retainCount]);

    NSLog(@"1.movie retain count = %i", [theMovie retainCount]);
    [theMovie prepareToPlay];  
    NSLog(@"2.movie retain count = %i", [theMovie retainCount]);
    
    // > 3.2
    [theMovie respondsToSelector:@selector(loadState)];
    NSLog(@"3.movie retain count = %i", [theMovie retainCount]);
    
    theMovie.scalingMode = MPMovieScalingModeAspectFill;
    NSLog(@"4.movie retain count = %i", [theMovie retainCount]);
    [theMovie setFullscreen:TRUE animated:TRUE];
    NSLog(@"5.movie retain count = %i", [theMovie retainCount]);
    theMovie.controlStyle = MPMovieControlStyleNone;
    NSLog(@"6.movie retain count = %i", [theMovie retainCount]);
    
    theMovie.view.frame = CGRectMake(0, 0, size.height, size.width); 
    NSLog(@"7.movie retain count = %i", [theMovie retainCount]);
    theMovie.view.backgroundColor = [UIColor clearColor];
    NSLog(@"8.movie retain count = %i", [theMovie retainCount]);
    
    // Transform
    theMovie.view.transform = CGAffineTransformMakeRotation(-270.0f * (M_PI/180.0f));
    NSLog(@"9.movie retain count = %i", [theMovie retainCount]);
    theMovie.view.center = ofxiPhoneGetUIWindow().center;
    NSLog(@"10.movie retain count = %i", [theMovie retainCount]);
    
    
    
    
    // setup the completion callback
    [[NSNotificationCenter defaultCenter] 
     addObserver:notifier
     selector:@selector(movieFinishedCallback:)
     name:MPMoviePlayerPlaybackDidFinishNotification
     object:theMovie];
    
    [theMovie play];
    NSLog(@"11.movie retain count = %i", [theMovie retainCount]);
    [ofxiPhoneGetUIWindow() addSubview:theMovie.view];
    NSLog(@"12.movie retain count = %i", [theMovie retainCount]);
    [ofxiPhoneGetUIWindow() makeKeyAndVisible];
    NSLog(@"13.movie retain count = %i", [theMovie retainCount]);
    viewLoaded = true;
    return 1;
}