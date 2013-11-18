#pragma once

#include "ofGraphics.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "ofAppRunner.h"
#include "lunar.h"
#include "bludLock.h"

class bludGraphics {
public:
	static const char className[];
	static Lunar<bludGraphics>::RegType methods[];
    static bool needsFixRes;
    static int width;
    static int height;
	bludGraphics(lua_State *L) {}
	int push (lua_State *L) {ofPushMatrix(); return 1;}
	int pop(lua_State *L)  {ofPopMatrix(); return 1;}

	int rotate(lua_State *L)  {ofRotate(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}
	int translate(lua_State *L)  {ofTranslate(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}
	int scale(lua_State *L)  {ofScale(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}
    int setBlendMode(lua_State *L){
        int blendNumber = luaL_checknumber(L, 1);
        ofBlendMode mode;
        switch (blendNumber) {
            case 1:
                mode = OF_BLENDMODE_ALPHA;
                break;
            case 2:
                mode = OF_BLENDMODE_ADD;
                break;
            case 3:
                mode = OF_BLENDMODE_SUBTRACT;
                break;
            case 4:
                mode = OF_BLENDMODE_MULTIPLY;
                break;
            case 5:
                mode = OF_BLENDMODE_SCREEN;
                break;

            default:
                mode = OF_BLENDMODE_DISABLED;
                break;
        }
        ofEnableBlendMode(mode);
        return 1;
    }
	int setColor(lua_State *L)  {
		int alpha = 255;
		if (lua_isnumber(L, 4)) {
			alpha = luaL_checknumber(L, 4);
		}
		ofSetColor(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), alpha);
		return 1;
	}
	int setBackground(lua_State *L)  {
		ofBackground(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3));
		return 1;
	}
	int setFrameRate(lua_State *L){
		ofSetFrameRate(luaL_checknumber(L, 1));
		cout << "current framerate: " << ofGetFrameRate() << endl;
		return 1;
	}
	int drawRect(lua_State *L)  {ofRect(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)); return 1;}
	int drawCircle(lua_State *L)  {ofCircle(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)); return 1;}
    int clear(lua_State *L){ofClear(0,0,0,0); return 1;}
	int getWidth(lua_State *L)  {lua_pushnumber(L, ofGetWidth()); return 1;}
	int getHeight(lua_State *L)  {lua_pushnumber(L, ofGetHeight()); return 1;}
	int getMillis(lua_State *L)  {lua_pushnumber(L, ofGetElapsedTimeMillis()); return 1;}
	int noise(lua_State *L){
		if (lua_isnumber(L, 4)) {
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4)));
		}else if (lua_isnumber(L, 3)) {
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3)));
		}else if (lua_isnumber(L, 2)) {
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1), luaL_checknumber(L, 2)));
		}else{
			lua_pushnumber(L, ofNoise(luaL_checknumber(L, 1)));
		}
		return 1;
	}
    int setWindowShape(lua_State *L){
        int w = luaL_checkinteger(L, 1);
        int h = luaL_checkinteger(L, 2);
        ofSetWindowShape(w, h);

//        width = luaL_checknumber(L, 1);
//        height = luaL_checknumber(L, 2);
//        needsFixRes = true;
        return 0;
    }
    int setFullscreen(lua_State *L){
        ofSetFullscreen(lua_toboolean(L, 1));
        return 0;
    }
    int exit(lua_State *L){
        // release the mutex and
        bludLock::getInstance()->unlock();
        ofExit();
        return 0;
    }
    int openURL(lua_State *L){
        ofLaunchBrowser(luaL_checkstring(L, 1));
        return 0;
    }
    int eraseMode(lua_State *L){
        #if defined TARGET_OF_IPHONE
                glBlendFuncSeparateOES(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA);
        #endif
        return 0;
    }
    int enableAlpha(lua_State *L){
        #if defined TARGET_OF_IPHONE
                glBlendFuncSeparateOES(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
        #endif
        return 0;
    }
	~bludGraphics() {}
};
