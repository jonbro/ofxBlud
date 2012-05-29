#pragma once

// bring in the lua stuff
#include "lua.hpp"
// bring in the lua stuff
//extern "C" {
//#include "lua.h"
//#include "lauxlib.h"
//#include "lualib.h"
//}

#include "lunar.h"

// bring in the required OF stuff
#include "ofLog.h"
#include "ofUtils.h"
#include "ofEvents.h"
#import "bludLock.h"
#include "bludRenderInstance.h"

#include "tinyxml.h"

int luaErrorHandler(lua_State *L);

class ofxBlud{
	public:
		void setup();
		void draw(ofEventArgs &e);
		void update(ofEventArgs &e);
		void exit(ofEventArgs &e);
	
		std::string execute(std::string code); // returns an error if there is one
		std::string executeFile(std::string filename); // executes a file, returns error if there is one
	
		// event callbacks
		void keyPressed(ofKeyEventArgs &e);
		void keyReleased(ofKeyEventArgs &e);
	
		void mousePressed(ofMouseEventArgs &e);
		void mouseMoved(ofMouseEventArgs &e);
		void mouseDragged(ofMouseEventArgs &e);
		void mouseReleased(ofMouseEventArgs &e);
		
		void touchDown(ofTouchEventArgs &e);
		void touchMoved(ofTouchEventArgs &e);
		void touchUp(ofTouchEventArgs &e);
		void touchDoubleTap(ofTouchEventArgs &e);
		
		void audioRequested(ofAudioEventArgs &e);
		lua_State* luaVM;
        bludRenderSingleton *renderer;

	private:
		ofMutex *mutex;
		ofMesh mesh;
        ofFbo			*fbo;
        ofTexture		texScreen;
        ofColor         feedbackColor;
        int             errorHandle;
};

void	RegisterLuaXML (lua_State *L);
static int LuaXML_ParseFile (lua_State *L);
