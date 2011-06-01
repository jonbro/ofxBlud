#include "bludAudioSync.h"

const char bludAudioSync::className[] = "bludAudioSync";

Lunar<bludAudioSync>::RegType bludAudioSync::methods[] = {
	method(bludAudioSync, setCallback),
	{0,0}
};

bludSyncContainer* bludSyncContainer::instance = NULL; 

bludSyncContainer* bludSyncContainer::getInstance(lua_State *Lua){
	if (!instance){   // Only allow one instance of class to be generated.		
		instance = new bludSyncContainer();
		instance->L = Lua;
		// connect this input to the mixer so that we can get triggers properly
	}
	return instance;	
}

void bludSyncContainer::audioRequested( float* buffer, int numFrames, int numChannels ){
	for (int i=0; i<numFrames; i++) {
		counter++;
		for ( int i=0; i<(int)triggers.size(); i++ ) {
			if (counter%triggers[i]->rate == 0) {
				// trigger the callback
				mutex->lock();
				lua_rawgeti( L, LUA_REGISTRYINDEX, triggers[i]->callback );
				if(lua_pcall(L, 0, 0, 0) != 0){
					ofLog(OF_LOG_ERROR, "Blud audio sync error");
					ofLog(OF_LOG_ERROR, lua_tostring(L, -1));
				}
				mutex->unlock();
			}
		}
	}
}

void bludSyncContainer::addTrigger(int c, int r){
	triggers.push_back( new Trigger( c, r ) );
}

void bludSyncContainer::removeTrigger(int c){
	// search the triggers and delete the one we don't want
	for ( int i=0; i<(int)triggers.size(); i++ ) {
		if (triggers[i]->callback == c) {
			triggers.erase(triggers.begin()+i);
			break;
		}
	}
}
