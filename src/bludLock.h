#pragma once
#import "ofTypes.h"
class bludLock{
public:
	static ofMutex* getInstance();
private:
	static ofMutex *instance;
};
