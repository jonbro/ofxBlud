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

class bludImage {
	lua_Number m_balance;
public:
	static const char className[];
	static Luna<bludImage>::RegType methods[];
	ofImage _image;
	bludImage(lua_State *L) {
		_image.setImageType(OF_IMAGE_COLOR_ALPHA);
	}
	int load (lua_State *L) {
		return _image.loadImage(luaL_checkstring(L, 1));
	}
	int draw(lua_State *L) { _image.draw(luaL_checknumber(L, 1), luaL_checknumber(L, 2)); return 0; }
	~bludImage() { printf("deleted image (%p)\n", this); }
};

const char bludImage::className[] = "bludImage";

Luna<bludImage>::RegType bludImage::methods[] = {
	method(bludImage, load),
	method(bludImage, draw),
	{0,0}
};