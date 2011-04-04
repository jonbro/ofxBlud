/* batch shape render wrapper */

#pragma once
#include "ofxShapeBatchRenderer.h"

class bludShapeBatch{
public:
	static const char className[];
	static Lunar<bludShapeBatch>::RegType methods[];
	ofxShapeBatchRenderer *batch;
	bludShapeBatch(lua_State *L) {
		// TODO: allow for different settings to be passed in
		batch = new ofxShapeBatchRenderer(SBR_TRIANGLE, 1000, 10);
	}
	int addRect(lua_State *L){
		batch->addRect(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5), 0);
		return 1;
	}
	int draw(lua_State *L){
		batch->draw();
		return 1;
	}
	int clear(lua_State *L){
		batch->clear();
		return 1;
	}
	int setColor(lua_State *L){
		batch->setColor(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
		return 1;
	}	
	~bludShapeBatch() {
		delete batch;
		printf("deleted shape batch (%p)\n", this);
	}
};

const char bludShapeBatch::className[] = "bludShapeBatch";

Lunar<bludShapeBatch>::RegType bludShapeBatch::methods[] = {
	method(bludShapeBatch, addRect),
	method(bludShapeBatch, draw),
	method(bludShapeBatch, clear),
	method(bludShapeBatch, setColor),
	{0,0}
};

/*
 bool addCircle(float x, float y, float z, float radius, int layer=DEFAULT_LAYER);
 bool addElipse(float x, float y, float z, float w, float h, int layer=DEFAULT_LAYER);
 
 bool addRect(float x, float y, float z, float w, float h, int layer=DEFAULT_LAYER);
 bool addCenteredRect(float x, float y, float z, float w, float h, int layer=DEFAULT_LAYER);
 
 //functional --
 void draw();
 void clear();
 
 //features --
 void setColor(int r, int g, int b, int a=255);
*/