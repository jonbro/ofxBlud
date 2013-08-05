#pragma once
#include "ofTypes.h"
class bludLock{
public:
	static ofMutex* getInstance();
private:
	static ofMutex *instance;
};
