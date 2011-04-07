#pragma once

#include "ofxSpriteSheetRenderer.h"

// sprite
// wrapper for a struct that contains data about the current sprite

class bludSprite {
public:
	static const char className[];
	static Lunar<bludSprite>::RegType methods[];
	bludSprite(lua_State *L){
		// setup the defaults for the animation
		ani.index			= 0;
		ani.frame			= 0;
		ani.total_frames	= 1;
		ani.w				= 1;
		ani.h				= 1;
		ani.frame_duration	= 75;
		ani.next_tick		= 0;
		ani.loops			= -1;
		ani.final_index		= -1;
		ani.frame_skip		= 1;
	}
	int setTotalFrames(lua_State *L)  {
		ani.total_frames = luaL_checknumber(L, 1);
		return 1;
	}
	int setFrameDuration(lua_State *L)  {
		ani.frame_duration = luaL_checknumber(L, 1);
		return 1;
	}
	int setIndex(lua_State *L)  {
		ani.index = luaL_checknumber(L, 1);
		return 1;
	}
	int setWidth(lua_State *L)  {
		ani.w = luaL_checknumber(L, 1);
		return 1;
	}
	int setHeight(lua_State *L)  {
		ani.h = luaL_checknumber(L, 1);
		return 1;
	}
	int setLoops(lua_State *L)  {
		ani.loops = luaL_checknumber(L, 1);
		return 1;
	}
	int setFrameSkip(lua_State *L)  {
		ani.frame_skip = luaL_checknumber(L, 1);
		return 1;
	}
	~bludSprite(){
		// ofLog(OF_LOG_VERBOSE, "deleted sprite" << this << endl);
		// printf("deleted sprite (%p)\n", this);
	}
	animation_t ani;
};
const char bludSprite::className[] = "bludSprite";

Lunar<bludSprite>::RegType bludSprite::methods[] = {
	method(bludSprite, setTotalFrames),
	method(bludSprite, setFrameDuration),
	method(bludSprite, setIndex),
	method(bludSprite, setWidth),
	method(bludSprite, setHeight),
	method(bludSprite, setLoops),
	method(bludSprite, setFrameSkip),
	{0,0}
};

/// spriteSheet renderer. Only partially implemented
class bludSpriteSheet {
	
public:
	static const char className[];
	static Lunar<bludSpriteSheet>::RegType methods[];
	bludSpriteSheet(lua_State *L){
		spriteRenderer = new ofxSpriteSheetRenderer(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
	}
	int loadTexture(lua_State *L)  {
		// defaults to loading nearest neighbor style
		spriteRenderer->loadTexture(luaL_checkstring(L, 1), luaL_checknumber(L, 2), GL_NEAREST);
		return 1;
	}
	int clear(lua_State *L)  {
		spriteRenderer->clear();
		return 1;
	}
	int update(lua_State *L)  {
		spriteRenderer->update(luaL_checknumber(L, 1));
		return 1;
	}
	int draw(lua_State *L)  {
		spriteRenderer->draw();
		return 1;
	}
	// TODO: extend this to support all of the different features (rotation, tinting, etc)
	int addCenteredTile(lua_State *L)  {
		// need to pull out the user data that was passed in on the first parameter
		bludSprite *s = Lunar<bludSprite>::check(L, 1);
		spriteRenderer->addCenteredTile(&s->ani, luaL_checknumber(L, 2), luaL_checknumber(L, 3));
		return 1;
	}
	int addTile(lua_State *L)  {
		// need to pull out the user data that was passed in on the first parameter
		bludSprite *s = Lunar<bludSprite>::check(L, 1);
		spriteRenderer->addTile(&s->ani, luaL_checknumber(L, 2), luaL_checknumber(L, 3));
		return 1;
	}
//	int addCenteredRotatedTile(lua_State *L){
//		bludSprite *s = Lunar<bludSprite>::check(L, 1);		
//		spriteRenderer->addCenterRotatedTile(&s->ani, float x, float y, int layer = -1, float wh = 1, flipDirection f = F_NONE, float scale=1.0, int rot=0, int r=255, int g=255, int b=255, int alpha=255);
//		return 1;
//	}
	~bludSpriteSheet() {
		delete spriteRenderer;
		printf("deleted sprite sheet (%p)\n", this);
	}
private:
	ofxSpriteSheetRenderer * spriteRenderer;
};

const char bludSpriteSheet::className[] = "bludSpriteSheet";

Lunar<bludSpriteSheet>::RegType bludSpriteSheet::methods[] = {
	method(bludSpriteSheet, loadTexture),
	method(bludSpriteSheet, clear),
	method(bludSpriteSheet, update),
	method(bludSpriteSheet, draw),
	method(bludSpriteSheet, addCenteredTile),
	method(bludSpriteSheet, addTile),
	{0,0}
};