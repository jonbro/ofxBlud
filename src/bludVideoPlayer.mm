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
    [theMovie prepareToPlay];  
    
    // > 3.2
    [theMovie respondsToSelector:@selector(loadState)];
    
    theMovie.scalingMode = MPMovieScalingModeAspectFill;
    [theMovie setFullscreen:TRUE animated:TRUE];
    theMovie.controlStyle = MPMovieControlStyleNone;
    
    theMovie.view.frame = CGRectMake(0, 0, size.width, size.height); 
    theMovie.view.backgroundColor = [UIColor clearColor];
    
    // Transform
    theMovie.view.transform = CGAffineTransformMakeRotation(0 * (M_PI/180.0f));
    theMovie.view.center = ofxiPhoneGetUIWindow().center;
    
    
    
    
    // setup the completion callback
    [[NSNotificationCenter defaultCenter] 
     addObserver:notifier
     selector:@selector(movieFinishedCallback:)
     name:MPMoviePlayerPlaybackDidFinishNotification
     object:theMovie];
    
    [theMovie play];
    [ofxiPhoneGetUIWindow() addSubview:theMovie.view];
    [ofxiPhoneGetUIWindow() makeKeyAndVisible];
    viewLoaded = true;
    return 1;
}