/* batch shape render wrapper */

#pragma once
#include "ofxShapeBatchRenderer.h"

class bludShapeBatch{
public:
	static const char className[];
	static Lunar<bludShapeBatch>::RegType methods[];
	ofxShapeBatchRenderer *batch;
	bludShapeBatch(lua_State *L) {
		
		int perLayer  = 1000;
		if (lua_isnumber(L, 1)) {
			perLayer = luaL_checknumber(L, 1);
		}

		int layers  = 10;
		if (lua_isnumber(L, 2)) {
			layers = luaL_checknumber(L, 2);
		}
		
		// TODO: allow for different settings to be passed in
		batch = new ofxShapeBatchRenderer(SBR_TRIANGLE, perLayer, layers);
	}
	int addRect(lua_State *L){
		int layer  = 0;
		if (lua_isnumber(L, 6)) {
			layer = luaL_checknumber(L, 6);
		}
		
		batch->addRect(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), luaL_checknumber(L, 5), layer);
		return 1;
	}
	int addCircle(lua_State *L){
		int layer  = 0;
		if (lua_isnumber(L, 5)) {
			layer = luaL_checknumber(L, 5);
		}
		batch->addCircle(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4), layer);
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
		int alpha = 255;
		if (lua_isnumber(L, 4)) {
			alpha = luaL_checknumber(L, 4);
		}		
		batch->setColor(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), alpha);
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
	method(bludShapeBatch, addCircle),
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