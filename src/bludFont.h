#pragma once

#include "ofTrueTypeFont.h"

class bludFont{
public:
	ofTrueTypeFont font;
	static const char className[];
	static Lunar<bludFont>::RegType methods[];
	bludFont(lua_State *L) {}

	int load(lua_State *L){
		printf("loading");
		bool antiAlias = true;
		if ( lua_isboolean( L, 3) ){
			antiAlias = lua_toboolean( L, 3 );
		}
		font.loadFont(luaL_checkstring(L, 1), luaL_checknumber(L, 2), antiAlias);
		return 1;
	}
	int draw (lua_State *L) {
		font.drawString(luaL_checkstring(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
		return 1;
	}	
};
const char bludFont::className[] = "bludFont";
Lunar<bludFont>::RegType bludFont::methods[] = {
	method(bludFont, load),
	method(bludFont, draw),
	{0,0}
};