#pragma once

#include "ofTrueTypeFont.h"
#include "lunar.h"

class bludFont{
public:
	ofTrueTypeFont font;
	static const char className[];
	static Lunar<bludFont>::RegType methods[];
	bludFont(lua_State *L) {}

	int load(lua_State *L){
		bool antiAlias = true;
		if ( lua_isboolean( L, 3) ){
			antiAlias = lua_toboolean( L, 3 );
		}
		font.loadFont(luaL_checkstring(L, 1), luaL_checknumber(L, 2), antiAlias);
		return 1;
	}
	int draw (lua_State *L) {
		// check to see if the text to draw includes a width...
		// this is for building text boxes and auto wrapping text (this is the dumbest way to do this ever)
		float width = -1;
		if(lua_isnumber(L, 4)){
			width = lua_tonumber(L, 4);
		}
		
		float line_height = 20;
		if(lua_isnumber(L, 5)){
			line_height = lua_tonumber(L, 5);
		}
		
		string textString = luaL_checkstring(L, 1);
		float x = luaL_checknumber(L, 2);
		float y = luaL_checknumber(L, 3);
		int line_y_pos = 0;
		
		if(width > 0){
			size_t start_pos = 0;
			size_t end_pos = 0;
			size_t test_end_pos = 0;
			string line_test = "";
			
			while (end_pos < textString.length()) {
				// move the test end pos forward
				test_end_pos = textString.find_first_of(" ", test_end_pos+1);
				line_test = textString.substr(start_pos, test_end_pos-start_pos);
				// if we are under the width
				if(font.stringWidth(line_test)<width){
					// commit the end position and loop it again
					end_pos = test_end_pos;
				}else {
					
					// if we are over the width
					// then print the line to the screen
					font.drawString(textString.substr(start_pos, end_pos-start_pos), x, y+line_y_pos);

					// move the start position forward, and unwind the end test
					end_pos++;
					start_pos = end_pos;
					test_end_pos--;
					// increase the line height
					line_y_pos += line_height;
				}
			}			
			font.drawString(textString.substr(start_pos, end_pos-start_pos), x, y+line_y_pos);
		}else {
			font.drawString(textString, x, y);
		}
		return 1;
	}
	int getWidth(lua_State *L){
		lua_pushnumber(L, font.stringWidth(luaL_checkstring(L, 1))); return 1;
	}
	int getHeight(lua_State *L){
		lua_pushnumber(L, font.stringHeight(luaL_checkstring(L, 1))); return 1;
	}
	int getHeightWrap(lua_State *L){
		// check to see if the text to draw includes a width...
		// this is for building text boxes and auto wrapping text (this is the dumbest way to do this ever)
		float width = -1;
		if(lua_isnumber(L, 4)){
			width = lua_tonumber(L, 4);
		}
		
		float line_height = 20;
		if(lua_isnumber(L, 5)){
			line_height = lua_tonumber(L, 5);
		}
		
		string textString = luaL_checkstring(L, 1);
		float x = luaL_checknumber(L, 2);
		float y = luaL_checknumber(L, 3);
		int line_y_pos = 0;
		
		if(width > 0){
			size_t start_pos = 0;
			size_t end_pos = 0;
			size_t test_end_pos = 0;
			string line_test = "";
			
			while (end_pos < textString.length()) {
				// move the test end pos forward
				test_end_pos = textString.find_first_of(" ", test_end_pos+1);
				line_test = textString.substr(start_pos, test_end_pos-start_pos);
				// if we are under the width
				if(font.stringWidth(line_test)<width){
					// commit the end position and loop it again
					end_pos = test_end_pos;
				}else {
					
					// if we are over the width
					// then print the line to the screen
					//font.drawString(textString.substr(start_pos, end_pos-start_pos), x, y+line_y_pos);
					
					// move the start position forward, and unwind the end test
					end_pos++;
					start_pos = end_pos;
					test_end_pos--;
					// increase the line height
					line_y_pos += line_height;
				}
			}
			lua_pushnumber(L, y+line_y_pos); return 1;
		}else {
			lua_pushnumber(L, font.stringHeight(luaL_checkstring(L, 1))); return 1;
		}
		return 1;		
	}
};