#pragma once

// bring in the lua stuff
extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

#include "lunar.h"

// bring in the required OF stuff
#include "ofLog.h"
#include "ofUtils.h"
#include "ofEvents.h"
#include "bludMixer.h"

class ofxBlud{
	public:
		void setup();
		void draw();
		void update();
	
		std::string execute(std::string code); // returns an error if there is one
		std::string executeFile(std::string filename); // executes a file, returns error if there is one
	
		// event callbacks
		void mouseMoved(ofMouseEventArgs &e);
		void mouseDragged(ofMouseEventArgs &e);
		void mousePressed(ofMouseEventArgs &e);
		void mouseReleased(ofMouseEventArgs &e);
		void audioRequested(ofAudioEventArgs &e);
		
	private:
		lua_State* luaVM;
		bludMixer *mixer;
};