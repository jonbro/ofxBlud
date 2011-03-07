#include "ofxBlud.h"

#define method(class, name) {#name, &class::name}
#include "bludImage.h"
#include "bludGraphics.h"
#include "bludSynth.h"

#include "blud_boot.h"


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
	
	mixer = bludMixer::getInstance();
}

void ofxBlud::draw(){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "draw"); /* function to be called */
	if(lua_pcall(luaVM, 0, 0, 0) != 0){
		ofLog(OF_LOG_ERROR, "Blud draw error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
}

void ofxBlud::update(){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "update"); /* function to be called */
	lua_pushnumber(luaVM, ofGetElapsedTimeMillis());
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		ofLog(OF_LOG_ERROR, "Blud update error");
		ofLog(OF_LOG_ERROR, lua_tostring(luaVM, -1));
	}
}

string ofxBlud::execute(string code){
	int error = luaL_dostring(luaVM, code.c_str());
	if (error) {
		return lua_tostring(luaVM, -1);
	}
	return "";
}

std::string ofxBlud::executeFile(std::string filename){
	cout << ofToDataPath(filename).c_str() << endl;
	int error = luaL_dofile(luaVM, ofToDataPath(filename).c_str());
	if (error) {
		return lua_tostring(luaVM, -1);
	}
	return "";
}

// event callbacks
void ofxBlud::mousePressed(ofMouseEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "pressed"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	if(lua_pcall(luaVM, 2, 0, 0) != 0){
		printf("error in mouse.pressed: %s\n", lua_tostring(luaVM, -1));
	}
}
void ofxBlud::mouseMoved(ofMouseEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	if(lua_pcall(luaVM, 2, 0, 0) != 0){
		printf("error in mouse.moved: %s\n", lua_tostring(luaVM, -1));
	}
}
void ofxBlud::mouseDragged(ofMouseEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "dragged"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.dragged: %s\n", lua_tostring(luaVM, -1));
	}
}
void ofxBlud::mouseReleased(ofMouseEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "released"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in mouse.released: %s\n", lua_tostring(luaVM, -1));
	}	
}

// all of the touch events
// I wonder if all this stuff could be handled through a macro or something?
void ofxBlud::touchDown(ofTouchEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "down"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.down: %s\n", lua_tostring(luaVM, -1));
	}
}
void ofxBlud::touchMoved(ofTouchEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.moved: %s\n", lua_tostring(luaVM, -1));
	}
}
void ofxBlud::touchUp(ofTouchEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "up"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.up: %s\n", lua_tostring(luaVM, -1));
	}
}
void ofxBlud::touchDoubleTap(ofTouchEventArgs &e){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "touch"); /* function to be called */
	lua_getfield(luaVM, -1, "double_tap"); /* function to be called */
	lua_pushnumber(luaVM, e.x);
	lua_pushnumber(luaVM, e.y);
	lua_pushnumber(luaVM, e.id);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in touch.double_tap: %s\n", lua_tostring(luaVM, -1));
	}
}

// does not pass audio processing through to lua
// manages triggering synths and feeding synth data to the output

// temporarily testing with noise
#include "ofMath.h"

void ofxBlud::audioRequested(ofAudioEventArgs &e){
	mixer->audioRequested(e.buffer, e.bufferSize, e.nChannels);
}
