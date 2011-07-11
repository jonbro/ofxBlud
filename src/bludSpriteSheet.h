#pragma once

#import "ofxSpriteSheetRenderer.h"
#import "lunar.h"
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
	int setSpriteX(lua_State *L)  {
		ani.sprite_x = luaL_checknumber(L, 1);
		return 1;
	}
	int setSpriteY(lua_State *L)  {
		ani.sprite_y = luaL_checknumber(L, 1);
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
	int setTexWidth(lua_State *L)  {
		ani.tex_w = luaL_checknumber(L, 1);
		return 1;
	}
	int setTexHeight(lua_State *L)  {
		ani.tex_h = luaL_checknumber(L, 1);
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
		//cout << "rendering" << endl;
		//printf("rendered: %i", (int)spriteRenderer->addCenterRotatedTile(&s->ani, x, y, layer, 1.0, (flipDirection)flipDir, scale, rot, r, g, b, alpha));
		spriteRenderer->addCenterRotatedTile(&s->ani, x, y, layer, 1.0, (flipDirection)flipDir, scale, rot, r, g, b, alpha);
		return 1;
	}
	int addTile(lua_State *L)  {
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
		int r = 255;
		if (lua_isnumber(L, 6)) {
			r = luaL_checknumber(L, 6);
		}
		int g = 255;
		if (lua_isnumber(L, 7)) {
			g = luaL_checknumber(L, 7);
		}
		int b = 255;
		if (lua_isnumber(L, 8)) {
			b = luaL_checknumber(L, 8);
		}
		int alpha = 255;
		if (lua_isnumber(L, 9)) {
			alpha = luaL_checknumber(L, 9);
		}
		
		//bool ofxSpriteSheetRenderer::addTile(animation_t* sprite, float x, float y, int layer, flipDirection f, int r, int g, int b, int alpha) {
		//bool addTile             (animation_t* sprite, float x, float y, int layer = -1, flipDirection f = F_NONE, int r=255, int g=255, int b=255, int alpha=255);

		spriteRenderer->addTile(&s->ani, x, y, layer, (flipDirection)flipDir, r, g, b, alpha);
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
	ofxSpriteSheetRenderer * spriteRenderer;
private:
};
