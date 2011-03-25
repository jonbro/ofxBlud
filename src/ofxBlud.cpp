#include "ofxBlud.h"

#include "bludImage.h"
#include "bludGraphics.h"
#include "bludSynth.h"
#include "bludAudioSync.h"
#include "bludSpriteSheet.h"

#include "blud_boot.h"

static void stackDump (lua_State *L) {
	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
				
			case LUA_TSTRING:  /* strings */
				printf("`%s'", lua_tostring(L, i));
				break;
				
			case LUA_TBOOLEAN:  /* booleans */
				printf(lua_toboolean(L, i) ? "true" : "false");
				break;
				
			case LUA_TNUMBER:  /* numbers */
				printf("%g", lua_tonumber(L, i));
				break;
				
			default:  /* other values */
				printf("%s", lua_typename(L, t));
				break;
				
        }
        printf("  ");  /* put a separator */
	}
	printf("\n");  /* end the listing */
}


void ofxBlud::setup(){

	// load the virtual machine
	luaVM = lua_open();
	if (NULL == luaVM)
	{
		ofLog(OF_LOG_ERROR, "Error Initializing lua");
	}
	ofSetLogLevel(OF_LOG_ERROR);
	// load the standard libs
	luaL_openlibs(luaVM);
	
	// load the wrappers
	Lunar<bludImage>::Register(luaVM);
	Lunar<bludGraphics>::Register(luaVM);
	Lunar<bludSynth>::Register(luaVM);
	Lunar<bludAudioSync>::Register(luaVM);
	Lunar<bludSprite>::Register(luaVM);
	Lunar<bludSpriteSheet>::Register(luaVM);
	
	// load the bootfile, which has placeholder for all the callbacks
	int error = luaL_dostring(luaVM, blud_boot);	
	if (error) {
		ofLog(OF_LOG_ERROR, "Blud Bootload failed");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
	
	// touch events
	ofAddListener(ofEvents.touchDown, this, &ofxBlud::touchDown);
	ofAddListener(ofEvents.touchUp, this, &ofxBlud::touchUp);
	ofAddListener(ofEvents.touchMoved, this, &ofxBlud::touchMoved);
	ofAddListener(ofEvents.touchDoubleTap, this, &ofxBlud::touchDoubleTap);
	
	// mouse events
	ofAddListener(ofEvents.mousePressed, this, &ofxBlud::mousePressed);
	ofAddListener(ofEvents.mouseReleased, this, &ofxBlud::mouseReleased);
	ofAddListener(ofEvents.mouseMoved, this, &ofxBlud::mouseMoved);
	ofAddListener(ofEvents.mouseDragged, this, &ofxBlud::mouseDragged);	
	
	// audio events
	ofAddListener(ofEvents.audioRequested, this, &ofxBlud::audioRequested);
	
	// sys events
	ofAddListener(ofEvents.draw, this, &ofxBlud::draw);
	ofAddListener(ofEvents.update, this, &ofxBlud::update);
	
	mixer = bludMixer::getInstance();
	ofEnableAlphaBlending();
}

void ofxBlud::draw(ofEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "draw"); /* function to be called */
	if(lua_pcall(luaVM, 0, 0, 0) != 0){
		ofLog(OF_LOG_ERROR, "Blud draw error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
	// need to pop once for global and once for the field.
	// the numbers and the calling of the function do not need to be called because they are automatically popped off the stack
	lua_pop(luaVM,1);
	mutex.unlock();
}

void ofxBlud::update(ofEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "update"); /* function to be called */
	lua_pushnumber(luaVM, ofGetElapsedTimeMillis());
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		ofLog(OF_LOG_ERROR, "Blud update error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,1);
	mutex.unlock();
}

string ofxBlud::execute(string code){
	int error = luaL_dostring(luaVM, code.c_str());
	if (error) {
		return lua_tostring(luaVM, -1);
	}
	return "";
}

string ofxBlud::executeFile(std::string filename){
	cout << ofToDataPath(filename).c_str() << endl;
	int error = luaL_dofile(luaVM, ofToDataPath(filename).c_str());
	if (error) {
		return lua_tostring(luaVM, -1);
	}
	return "";
}

// event callbacks
void ofxBlud::mousePressed(ofMouseEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "pressed"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	if(lua_pcall(luaVM, 2, 0, 0) != 0){
		printf("error in mouse.pressed: %s\n", lua_tostring(luaVM, -1));
	}
	mutex.unlock();
}
void ofxBlud::mouseMoved(ofMouseEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	if(lua_pcall(luaVM, 2, 0, 0) != 0){
		printf("error in mouse.moved: %s\n", lua_tostring(luaVM, -1));
	}
	mutex.unlock();
}
void ofxBlud::mouseDragged(ofMouseEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "dragged"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.dragged: %s\n", lua_tostring(luaVM, -1));
	}
	mutex.unlock();
}
void ofxBlud::mouseReleased(ofMouseEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "released"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.released: %s\n", lua_tostring(luaVM, -1));
	}	
	mutex.unlock();
}

// all of the touch events
// I wonder if all this stuff could be handled through a macro or something?
void ofxBlud::touchDown(ofTouchEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "down"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.down: %s\n", lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,4);
	mutex.unlock();
}
void ofxBlud::touchMoved(ofTouchEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.moved: %s\n", lua_tostring(luaVM, -1));
	}
	mutex.unlock();
}
void ofxBlud::touchUp(ofTouchEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "up"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.up: %s\n", lua_tostring(luaVM, -1));
	}
	lua_pop(luaVM,4);
//	stackDump(luaVM);
	mutex.unlock();
}
void ofxBlud::touchDoubleTap(ofTouchEventArgs &e){
	mutex.lock();
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "double_tap"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.double_tap: %s\n", lua_tostring(luaVM, -1));
	}
	mutex.unlock();
}

// does not pass audio processing through to lua
// manages triggering synths and feeding synth data to the output

// temporarily testing with noise
#include "ofMath.h"

void ofxBlud::audioRequested(ofAudioEventArgs &e){
	mutex.lock();
	// getting called
	mixer->audioRequested(e.buffer, e.bufferSize, e.nChannels);
	mutex.unlock();
}
