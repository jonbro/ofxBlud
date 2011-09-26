#include "bludVideoPlayer.h"

const char bludVideoPlayer::className[] = "bludVideoPlayer";

Lunar<bludVideoPlayer>::RegType bludVideoPlayer::methods[] = {
	method(bludVideoPlayer, load),
	method(bludVideoPlayer, setCompletionCallback),
	method(bludVideoPlayer, removeView),
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