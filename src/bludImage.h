#pragma once
#include "ofImage.h"
#include "lunar.h"

class bludImage {
public:
	static const char className[];
	static Lunar<bludImage>::RegType methods[];
	ofImage _image;
	bludImage(lua_State *L) {
		_image.setImageType(OF_IMAGE_COLOR_ALPHA);
	}
	int load (lua_State *L) {
		return _image.loadImage(luaL_checkstring(L, 1));
	}
	int draw(lua_State *L) {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        _image.bind();
        _image.draw(luaL_checknumber(L, 1), luaL_checknumber(L, 2));
        return 0;
    }
	int drawScale(lua_State *L) { _image.draw(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 0; }

	int width(lua_State *L) { lua_pushnumber(L, _image.getWidth()); return 1; }
	int height(lua_State *L) { lua_pushnumber(L, _image.getHeight()); return 1; }
	
	~bludImage() { printf("deleted image (%p)\n", this); }
};