/*
 *  bludImage.h
 *  iPhoneEmptyExample
 *
 *  Created by jonbroFERrealz on 12/5/10.
 *  Copyright 2010 Heavy Ephemera Industries. All rights reserved.
 *
 */

#include "ofMain.h"
#import "defines.h"

class bludGraphics {
public:
	static const char className[];
	static Lunar<bludGraphics>::RegType methods[];
	bludGraphics(lua_State *L) {}

	int push (lua_State *L) {ofPushMatrix(); return 1;}
	int pop(lua_State *L)  {ofPopMatrix(); return 1;}

	int rotate(lua_State *L)  {ofRotate(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}
	int translate(lua_State *L)  {ofTranslate(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}

	int setColor(lua_State *L)  {ofSetColor(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}
	int drawRect(lua_State *L)  {ofRect(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}

	int getWidth(lua_State *L)  {lua_pushnumber(L, ofGetWidth()); return 1;}
	int getHeight(lua_State *L)  {lua_pushnumber(L, ofGetHeight()); return 1;}
	
	~bludGraphics() { printf("deleted graphics (%p)\n", this); }
};

const char bludGraphics::className[] = "bludGraphics";

Lunar<bludGraphics>::RegType bludGraphics::methods[] = {
	method(bludGraphics, push),
	method(bludGraphics, pop),
	method(bludGraphics, rotate),
	method(bludGraphics, translate),
	method(bludGraphics, setColor),
	method(bludGraphics, drawRect),
	method(bludGraphics, getWidth),
	method(bludGraphics, getHeight),
	{0,0}
};