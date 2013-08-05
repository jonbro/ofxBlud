/*
 *  bludLock.cpp
 *  spaceHero
 *
 *  Created by jonbroFERrealz on 6/1/11.
 *  Copyright 2011 Heavy Ephemera Industries. All rights reserved.
 *
 */


#include "bludlock.h"

ofMutex* bludLock::instance = NULL; 

ofMutex* bludLock::getInstance(){
	if (!instance)   // Only allow one instance of class to be generated.
		instance = new ofMutex();
	return instance;
}