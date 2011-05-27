#pragma once

// bring in the lua stuff
#include "lua.hpp"

#include "lunar.h"

// bring in the required OF stuff
#include "ofLog.h"
#include "ofUtils.h"
#include "ofEvents.h"
#include "bludMixer.h"

#include "tinyxml.h"

class ofxBlud{
	public:
		void setup();
		void draw(ofEventArgs &e);
		void update(ofEventArgs &e);
	
		std::string execute(std::string code); // returns an error if there is one
		std::string executeFile(std::string filename); // executes a file, returns error if there is one
	
		// event callbacks
		void mousePressed(ofMouseEventArgs &e);
		void mouseMoved(ofMouseEventArgs &e);
		void mouseDragged(ofMouseEventArgs &e);
		void mouseReleased(ofMouseEventArgs &e);
		
		void touchDown(ofTouchEventArgs &e);
		void touchMoved(ofTouchEventArgs &e);
		void touchUp(ofTouchEventArgs &e);
		void touchDoubleTap(ofTouchEventArgs &e);
		
		void audioRequested(ofAudioEventArgs &e);
		
	private:
		lua_State* luaVM;
		ofSoundMixer *mixer;
		ofMutex mutex;
};

void	RegisterLuaXML (lua_State *L);
static int LuaXML_ParseFile (lua_State *L);
