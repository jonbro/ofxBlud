#include "testApp.h"
#include "bludImage.h"
#include "bludGraphics.h"

//--------------------------------------------------------------
void testApp::setup(){
	luaVM = lua_open();
	
	if (NULL == luaVM)
	{
		printf("Error Initializing lua\n");
	}
	
	luaL_openlibs(luaVM);
	Lunar<bludImage>::Register(luaVM);
	Lunar<bludGraphics>::Register(luaVM);
	
	int error = luaL_dofile(luaVM, ofToDataPath("boot.lua").c_str());
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}
			
	string s = "blud.bundle_root = '";
	s += ofToDataPath("");
	s += "'";
	error = luaL_dostring(luaVM, s.c_str());
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}
	
	
	error = luaL_dofile(luaVM, ofToDataPath("test_scripts/breakout/breakout.lua").c_str());
	if (error) {
		printf("%s\n", lua_tostring(luaVM, -1));
	}
	
	
	ofEnableAlphaBlending();
	
}

//--------------------------------------------------------------
void testApp::update(){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "update"); /* function to be called */
	lua_pushnumber(luaVM, ofGetElapsedTimeMillis()-lastTime);
	lastTime = ofGetElapsedTimeMillis();
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		printf("error in update: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::draw(){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "draw"); /* function to be called */
	if(lua_pcall(luaVM, 0, 0, 0) != 0){
		printf("error in draw: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "key"); /* function to be called */
	lua_getfield(luaVM, -1, "pressed"); /* function to be called */
	lua_pushnumber(luaVM, key);
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		printf("error in key pressed: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "key"); /* function to be called */
	lua_getfield(luaVM, -1, "released"); /* function to be called */
	lua_pushnumber(luaVM, key);
	if(lua_pcall(luaVM, 1, 0, 0) != 0){
		printf("error in key released: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "moved"); /* function to be called */
	lua_pushnumber(luaVM, x);
	lua_pushnumber(luaVM, y);
	if(lua_pcall(luaVM, 2, 0, 0) != 0){
		printf("error in pressed: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "dragged"); /* function to be called */
	lua_pushnumber(luaVM, x);
	lua_pushnumber(luaVM, y);
	lua_pushnumber(luaVM, button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in dragged: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "pressed"); /* function to be called */
	lua_pushnumber(luaVM, x);
	lua_pushnumber(luaVM, y);
	lua_pushnumber(luaVM, button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in pressed: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	lua_getglobal(luaVM, "blud");
	lua_getfield(luaVM, -1, "mouse"); /* function to be called */
	lua_getfield(luaVM, -1, "released"); /* function to be called */
	lua_pushnumber(luaVM, x);
	lua_pushnumber(luaVM, y);
	lua_pushnumber(luaVM, button);
	if(lua_pcall(luaVM, 3, 0, 0) != 0){
		printf("error in released: %s\n", lua_tostring(luaVM, -1));
	}	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
