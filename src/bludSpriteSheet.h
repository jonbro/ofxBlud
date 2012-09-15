#pragma once

#import "ofxSpriteSheetRenderer.h"
#import "lunar.h"
#import "bludColor.h"
#include "bludLock.h"
// sprite
// wrapper for a struct that contains data about the current sprite

class bludSprite {
public:
	static const char className[];
	static Lunar<bludSprite>::RegType methods[];
	bludSprite(){
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
		
		ani.tex_y = 0;					// how much offset from the top left of the sheet the texture is (no longer using the index for lookups)
		ani.tex_x = 0;					// doing it this way so that we can have differently sized textures
		ani.tex_w = 0;					// how big the texture is (on the sheet)
		ani.tex_h = 0;
		
		ani.sprite_x = 0;					// how far offset the display of the sprite should be from the requested display position (how much alpha got trimmed when packing the sprite)
		ani.sprite_y = 0;
		ani.spritesource_w = 0;			// the size of the sprite before the alpha trimming took place
		ani.spritesource_h = 0;			// used for doing rotations around the center of the sprite (maybe, used for nothing for now)		
		
	}
    // I think I need a copy constructor
    bludSprite(bludSprite &other){
        cout << "copying" << endl;
    }
	bludSprite(lua_State *L){
		bludSprite();
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
	int setTexX(lua_State *L)  {
		ani.tex_x = luaL_checknumber(L, 1);
		return 1;
	}
	int setTexY(lua_State *L)  {
		ani.tex_y = luaL_checknumber(L, 1);
		return 1;
	}
	int getTexX(lua_State *L)  {
		lua_pushnumber(L, ani.tex_x);
		return 1;
	}
	int getTexY(lua_State *L)  {
		lua_pushnumber(L, ani.tex_y);
		return 1;
	}
	int setSpriteX(lua_State *L)  {
		ani.sprite_x = luaL_checknumber(L, 1);
		return 1;
	}
	int setSpriteY(lua_State *L)  {
		ani.sprite_y = luaL_checknumber(L, 1);
		return 1;
	}
	int getSpriteX(lua_State *L)  {
		lua_pushnumber(L, ani.sprite_x);
		return 1;
	}
	int getSpriteY(lua_State *L)  {
		lua_pushnumber(L, ani.sprite_y);
		return 1;
	}
	int setWidth(lua_State *L)  {
		ani.w = luaL_checknumber(L, 1);
		return 1;
	}
	int getWidth(lua_State *L) {
		lua_pushnumber(L, ani.w);
		return 1;
	}
	int getHeight(lua_State *L) {
		lua_pushnumber(L, ani.h	);
		return 1;
	}
	int setHeight(lua_State *L)  {
		ani.h = luaL_checknumber(L, 1);
		return 1;
	}
	int setTexWidth(lua_State *L)  {
		ani.tex_w = luaL_checknumber(L, 1);
		return 1;
	}
	int setTexHeight(lua_State *L)  {
		ani.tex_h = luaL_checknumber(L, 1);
		return 1;
	}
	int getTexWidth(lua_State *L)  {
		lua_pushnumber(L, ani.tex_w	);
		return 1;
	}
	int getTexHeight(lua_State *L)  {
		lua_pushnumber(L, ani.tex_h	);
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

/// spriteSheet renderer. Only partially implemented
class bludSpriteSheet {
	
public:
	static const char className[];
	static Lunar<bludSpriteSheet>::RegType methods[];
	
    bludSpriteSheet(lua_State *L);
	int loadTexture(lua_State *L);
	int setupTexture(lua_State *L);
    int setAlpha(lua_State *L)  {
        alpha = lua_toboolean(L, 1);
        return 0;
    }
    int setBlendMode(lua_State *L)  {
        blendMode = luaL_checknumber(L, 1);
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
		float x = luaL_checknumber(L, 2);
		float y = luaL_checknumber(L, 3);

		float layer = -1;
		if (lua_isnumber(L, 4)) {
			layer = luaL_checknumber(L, 4);
		}
		
		int flipDir = 0;
		if (lua_isnumber(L, 5)) {
			flipDir = luaL_checknumber(L, 5);
		}

		float scale = 1;
		if (lua_isnumber(L, 6)) {
			scale = luaL_checknumber(L, 6);
		}
		int r = 255;
		if (lua_isnumber(L, 7)) {
			r = luaL_checknumber(L, 7);
		}
		int g = 255;
		if (lua_isnumber(L, 8)) {
			g = luaL_checknumber(L, 8);
		}
		int b = 255;
		if (lua_isnumber(L, 9)) {
			b = luaL_checknumber(L, 9);
		}
		int alpha = 255;
		if (lua_isnumber(L, 10)) {
			alpha = luaL_checknumber(L, 10);
		}
		
		spriteRenderer->addCenteredTile(&s->ani, x, y, layer, (flipDirection)flipDir, scale, r, g, b, alpha);
		return 1;
	}
	int addCenterRotatedTile(lua_State *L){
		// need to pull out the user data that was passed in on the first parameter
		bludSprite *s = Lunar<bludSprite>::check(L, 1);
		float x = luaL_checknumber(L, 2);
		float y = luaL_checknumber(L, 3);
		
		float layer = -1;
		if (lua_isnumber(L, 4)) {
			layer = luaL_checknumber(L, 4);
		}
		
		int flipDir = 0;
		if (lua_isnumber(L, 5)) {
			flipDir = luaL_checknumber(L, 5);
		}
		
		float scale = 1;
		if (lua_isnumber(L, 6)) {
			scale = luaL_checknumber(L, 6);
		}
		int rot = 0;
		if (lua_isnumber(L, 7)) {
			rot = luaL_checknumber(L, 7);
		}
		int r = 255;
		if (lua_isnumber(L, 8)) {
			r = luaL_checknumber(L, 8);
		}
		int g = 255;
		if (lua_isnumber(L, 9)) {
			g = luaL_checknumber(L, 9);
		}
		int b = 255;
		if (lua_isnumber(L, 10)) {
			b = luaL_checknumber(L, 10);
		}
		int alpha = 255;
		if (lua_isnumber(L, 11)) {
			alpha = luaL_checknumber(L, 11);
		}
		spriteRenderer->addCenterRotatedTile(&s->ani, x, y, layer, 1.0, (flipDirection)flipDir, scale, rot, r, g, b, alpha);
		return 1;
	}
	int addCornerTile(lua_State *L){
		bludSprite *s = Lunar<bludSprite>::check(L, 1);

		float layer = -1;
		if (lua_isnumber(L, 10)) {
			layer = luaL_checknumber(L, 10);
		}
		int flipDir = 0;
		if (lua_isnumber(L,11)) {
			flipDir = luaL_checknumber(L,11);
		}
		
		int r = 255;
		if (lua_isnumber(L, 12)) {
			r = luaL_checknumber(L, 12);
		}
		int g = 255;
		if (lua_isnumber(L, 13)) {
			g = luaL_checknumber(L, 13);
		}
		int b = 255;
		if (lua_isnumber(L, 14)) {
			b = luaL_checknumber(L, 14);
		}
		int alpha = 255;
		if (lua_isnumber(L, 15)) {
			alpha = luaL_checknumber(L, 15);
		}
		spriteRenderer->addCornerTile(&s->ani, ofPoint(luaL_checknumber(L, 2), luaL_checknumber(L, 3)), ofPoint(luaL_checknumber(L, 4), luaL_checknumber(L, 5)), ofPoint(luaL_checknumber(L, 6), luaL_checknumber(L, 7)), ofPoint(luaL_checknumber(L, 8), luaL_checknumber(L, 9)), layer, (flipDirection)flipDir, r, g, b, alpha);

		return 1;
	}
	int addCornerColorTile(lua_State *L);
	int addTile(lua_State *L);
    bool hasSprite(string key){
        lua_rawgeti(callbackState, LUA_REGISTRYINDEX, lookupCallback);
        lua_pushstring(callbackState, key.c_str());
        if(lua_pcall(callbackState, 1, 1, 0) != 0){
            ofLog(OF_LOG_ERROR, "sprite lookup error");
            ofLog(OF_LOG_ERROR, lua_tostring(callbackState, -1));
            return false;
        }
        bool rVal = lua_toboolean(callbackState, -1);
        lua_pop(callbackState, 1);
        return rVal;
    }
    bludSprite* getSprite(string key){
        lua_rawgeti(callbackState, LUA_REGISTRYINDEX, lookupCallback);
        lua_pushstring(callbackState, key.c_str());
        if(lua_pcall(callbackState, 1, 1, 0) != 0){
            ofLog(OF_LOG_ERROR, "sprite lookup error");
            ofLog(OF_LOG_ERROR, lua_tostring(callbackState, -1));
            return NULL;
        }
        bludSprite *rVal = Lunar<bludSprite>::check(callbackState, -1);
        lua_pop(callbackState, 1);
        return rVal;
    }
    int setSpriteLookupCallback(lua_State *l){
        if(lua_isfunction(l, 1)){
            // push the value of the function (that is at position 1), onto the top of the stack
            lua_pushvalue(l, 1);
            // store this stack position in the registry index
            lookupCallback = luaL_ref(l, LUA_REGISTRYINDEX);
            callbackState = l;
        }
        return 0;
    }
	~bludSpriteSheet() {
		delete spriteRenderer;
        delete texture;
		printf("deleted sprite sheet (%p)\n", this);
	}
    int lookupCallback;
    lua_State *callbackState;
    int blendMode;
    bool alpha;
    LinearTexture *texture;
	ofxSpriteSheetRenderer * spriteRenderer;
private:
};
