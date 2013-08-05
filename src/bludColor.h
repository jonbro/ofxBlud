#pragma once

#include "lunar.h"
#include "ofColor.h"

class bludColor {
public:
	static const char className[];
	static Lunar<bludColor>::RegType methods[];
    ofColor color;
	bludColor(lua_State *L) {
        set(L);
    }
    int get(lua_State *L){
        lua_pushnumber(L, color.r);
        lua_pushnumber(L, color.g);
        lua_pushnumber(L, color.b);
        lua_pushnumber(L, color.a);
        return 4;
    }
    int set(lua_State *L){
        int r = luaL_checknumber(L, 1);
        int g = luaL_checknumber(L, 2);
        int b = luaL_checknumber(L, 3);
        int a = 255;
        if(lua_isnumber(L, 4)){
            a = luaL_checknumber(L, 4);
        }
        color.set(r, g, b, a);        
        return 1;
    }
	~bludColor() {}
};