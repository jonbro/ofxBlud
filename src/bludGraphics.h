#pragma once

#include "ofGraphics.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "ofAppRunner.h"

class bludGraphics {
public:
	static const char className[];
	static Lunar<bludGraphics>::RegType methods[];
	bludGraphics(lua_State *L) {}

	int push (lua_State *L) {ofPushMatrix(); return 1;}
	int pop(lua_State *L)  {ofPopMatrix(); return 1;}

	int rotate(lua_State *L)  {ofRotate(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}
	int translate(lua_State *L)  {ofTranslate(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}
	int scale(lua_State *L)  {ofScale(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}

	int setColor(lua_State *L)  {
		int alpha = 255;
		if (lua_isnumber(L, 4)) {
			alpha = luaL_checknumber(L, 4);
		}				
		ofSetColor(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), alpha);
		return 1;
	}
	int setFrameRate(lua_State *L){
		ofSetFrameRate(luaL_checknumber(L, 1));
		cout << "current framerate: " << ofGetFrameRate() << endl;
		return 1;
	}
	int drawRect(lua_State *L)  {ofRect(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}
	int drawCircle(lua_State *L)  {ofCircle(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}
    int clear(lua_State *L){ofClear(0,0,0,0); return 1;}
	int getWidth(lua_State *L)  {lua_pushnumber(L, ofGetWidth()); return 1;}
	int getHeight(lua_State *L)  {lua_pushnumber(L, ofGetHeight()); return 1;}
	int getMillis(lua_State *L)  {lua_pushnumber(L, ofGetElapsedTimeMillis()); return 1;}
	int noise(lua_State *L){
		if (lua_isnumber(L, 4)) {
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
		}else if (lua_isnumber(L, 3)) {
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
		}else if (lua_isnumber(L, 2)) {
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
		}else{
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1)));
		}
		return 1;
	}
    int openURL(lua_State *L){
        [[UIApplication sharedApplication] openURL:[NSURL URLWithString: [[[[NSString alloc] initWithCString:luaL_checkstring(L, 1)] stringByAddingPercentEscapesUsingEncoding: NSASCIIStringEncoding] autorelease]]];
        return 1;
    }
    int enableAlpha(lua_State *L){glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);return 1;}
	~bludGraphics() {}
};

const char bludGraphics::className[] = "bludGraphics";

Lunar<bludGraphics>::RegType bludGraphics::methods[] = {
	method(bludGraphics, push),
	method(bludGraphics, pop),
	method(bludGraphics, rotate),
	method(bludGraphics, translate),
	method(bludGraphics, scale),
	method(bludGraphics, setColor),
	method(bludGraphics, setFrameRate),	
	method(bludGraphics, drawRect),
	method(bludGraphics, drawCircle),
    method(bludGraphics, clear),
	method(bludGraphics, getWidth),
	method(bludGraphics, getHeight),
	method(bludGraphics, getMillis),
    method(bludGraphics, enableAlpha),
	method(bludGraphics, noise),
    method(bludGraphics, openURL),
	{0,0}
};