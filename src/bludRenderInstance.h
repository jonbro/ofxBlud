#pragma once
#include <vector>
// an object that keeps around a bludSpriteSheet, so that it can be rendered outside of the lua loop.
// this should allow the lua loop to not block the main thread for as long

#include "bludSpriteSheet.h"
#include "ofGraphics.h"

class bludRenderSingleton{
public:
    bludRenderSingleton(){
        hasSpriteSheet = false;
        currentBlend = 0;
    }
    void addAtStart(bludSpriteSheet *_sheet){
        vector<bludSpriteSheet*>::iterator it;
        it = sheets.begin();
        it = sheets.insert( it , _sheet );
    }
    void addSheet(bludSpriteSheet *_sheet){
        sheets.push_back(_sheet);
    }
    void removeSheet(bludSpriteSheet *_sheet){
        vector<bludSpriteSheet*>::iterator it;

        for ( it = sheets.begin(); it != sheets.end(); ){
            if( (*it) == _sheet ){
                it = sheets.erase(it);
            }else{
                ++it;
            }
        }
    }
    void render();
    vector<bludSpriteSheet*> sheets;
    bool hasSpriteSheet;
    bool alpha;
    int currentBlend;
};

class bludRenderInstance {
public:
	static bludRenderSingleton* getInstance();
private:
	static bludRenderSingleton *instance;
};

class bludRenderer{
public:
	static const char className[];
	static Lunar<bludRenderer>::RegType methods[];
	bludRenderer(lua_State *L) {
        renderer = bludRenderInstance::getInstance();
        renderer->hasSpriteSheet = false;
	}
    int setSpriteSheet(lua_State *L){
        bludSpriteSheet *s = Lunar<bludSpriteSheet>::check(L, 1);
        renderer->addSheet(s);
        return 1;
    }
    int removeSheet(lua_State *L){
        bludSpriteSheet *s = Lunar<bludSpriteSheet>::check(L, 1);
        renderer->removeSheet(s);
        return 1;
    }
    int addAtStart(lua_State *L){
        bludSpriteSheet *s = Lunar<bludSpriteSheet>::check(L, 1);
        renderer->addAtStart(s);
        return 1;
    }
private:
    bludRenderSingleton *renderer;
};