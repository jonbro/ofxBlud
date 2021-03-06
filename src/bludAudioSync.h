#pragma once

#include "lunar.h"
#include "ofSoundUnit.h"
#include "bludMixer.h"
#include "bludLock.h"
#include "ofTypes.h"

/**
 * Singleton to manage all of the audio syncs
 */

class bludSyncContainer : public ofSoundSource {
public:
	bludSyncContainer(){
		mixer = bludMixer::getInstance();
		mutex = bludLock::getInstance();
		mixer->addInputFrom(this);
		counter = 0;
	};
	static	bludSyncContainer* getInstance(lua_State *Lua);
	void	audioRequested( float* buffer, int numFrames, int numChannels );
	string	getName(){ return "bludSyncContainer"; };
	void	addTrigger(int c, int r);
	void	removeTrigger(int c);
private:
	static bludSyncContainer *instance;
	ofSoundMixer	*mixer;
	// Inputs into the mixer, with volume and pan
	struct Trigger
	{
		int rate;
		int callback;
		Trigger( int c, int r )
		{
			rate = r;
			callback = c;
		}
	};
	vector<Trigger*> triggers;
	int counter;
	lua_State *L;
	ofMutex *mutex;
};

/**
 * interface to do sample accurate audio timing
 *
 * allows for a callback to be called with sample accurate timing
 * don't exactly know how to manage the thread safety of this... tdb
 */

class bludAudioSync{
public:
	static const char className[];
	static Lunar<bludAudioSync>::RegType methods[];
	bludAudioSync(lua_State *L){
		container = bludSyncContainer::getInstance(L);
		rate = luaL_checknumber(L, 1);
		// load the sync into the callback container
		mutex = bludLock::getInstance();
		cout << "rate: " << rate << endl;
	}
	int setCallback(lua_State *L){
		// check to make sure that we are about to pull a function off the stack
		if(lua_isfunction(L, 1)){
			// push the value of the function (that is at position 1), onto the top of the stack
			lua_pushvalue(L, 1);
			// store this stack position in the registry index
			callback = luaL_ref(L, LUA_REGISTRYINDEX);
			cout << "callback" << callback << endl;
			container->addTrigger(callback, rate);
		}
		return 0;
	}
	~bludAudioSync() {
		container->removeTrigger(callback);
		printf("deleted sync (%p)\n", this);
	}
private:
	int callback;	// the reference to the function that gets called in lua
	int rate;		// the number of samples between when it gets called.
	bludSyncContainer *container;
	ofMutex *mutex;
	//everything syncs to a 0 offset that is set at first run
};
