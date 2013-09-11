#include "bludSpriteSheet.h"

const char bludSpriteSheet::className[] = "bludSpriteSheet";
const char bludSprite::className[] = "bludSprite";

Lunar<bludSprite>::RegType bludSprite::methods[] = {
	method(bludSprite, setTotalFrames),
	method(bludSprite, setFrameDuration),
	method(bludSprite, setIndex),
	method(bludSprite, setWidth),
	method(bludSprite, setHeight),
	method(bludSprite, getWidth),
	method(bludSprite, getHeight),
	method(bludSprite, setLoops),
	method(bludSprite, setFrameSkip),
	method(bludSprite, setTexX),
	method(bludSprite, setTexY),
	method(bludSprite, getTexX),
	method(bludSprite, getTexY),
	method(bludSprite, setSpriteX),
	method(bludSprite, setSpriteY),
	method(bludSprite, getSpriteX),
	method(bludSprite, getSpriteY),
	method(bludSprite, setTexWidth),
	method(bludSprite, setTexHeight),
	method(bludSprite, getTexWidth),
	method(bludSprite, getTexHeight),
	{0,0}
};

Lunar<bludSpriteSheet>::RegType bludSpriteSheet::methods[] = {
	method(bludSpriteSheet, loadTexture),
	method(bludSpriteSheet, setupTexture),
	method(bludSpriteSheet, clear),
	method(bludSpriteSheet, update),
	method(bludSpriteSheet, draw),
	method(bludSpriteSheet, addCenteredTile),
	method(bludSpriteSheet, addTile),
	method(bludSpriteSheet, addCenterRotatedTile),
	method(bludSpriteSheet, addCornerTile),
    method(bludSpriteSheet, addCornerColorTile),
    method(bludSpriteSheet, setBlendMode),
    method(bludSpriteSheet, setAlpha),
    method(bludSpriteSheet, setHasTexture),
    method(bludSpriteSheet, setSpriteLookupCallback),
	{0,0}
};

bludSpriteSheet::bludSpriteSheet(lua_State *L){
    texture = new LinearTexture();
    spriteRenderer = new ofxSpriteSheetRenderer(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
    blendMode = 0;
    alpha = true;
    hasTexture = true;
}

int bludSpriteSheet::loadTexture(lua_State *L){
    // defaults to loading nearest neighbor style
    int height = luaL_checknumber(L, 2);
    if (lua_isnumber(L, 3)) {
        height = luaL_checknumber(L, 3);
    }
    cout << "loading texture bludspritesheet" << endl;
    texture->loadTexture(luaL_checkstring(L, 1));
    cout << "after loading" << endl;
    spriteRenderer->loadTexture(texture);
    return 1;
}
int bludSpriteSheet::setupTexture(lua_State *L){
    spriteRenderer->loadTexture(texture);
    return 1;
}
int bludSpriteSheet::addTile(lua_State *L)  {
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

int bludSpriteSheet::addCornerColorTile(lua_State *L){
    bludSprite *s = Lunar<bludSprite>::check(L, 1);

    float layer = -1;
    if (lua_isnumber(L, 10)) {
        layer = luaL_checknumber(L, 10);
    }
    int flipDir = 0;
    if (lua_isnumber(L,11)) {
        flipDir = luaL_checknumber(L,11);
    }
    bludColor *c1 = Lunar<bludColor>::check(L, 12);
    bludColor *c2 = Lunar<bludColor>::check(L, 13);
    bludColor *c3 = Lunar<bludColor>::check(L, 14);
    bludColor *c4 = Lunar<bludColor>::check(L, 15);

    spriteRenderer->addCornerColorTile(&s->ani, ofPoint(luaL_checknumber(L, 2), luaL_checknumber(L, 3)), ofPoint(luaL_checknumber(L, 4), luaL_checknumber(L, 5)), ofPoint(luaL_checknumber(L, 6), luaL_checknumber(L, 7)), ofPoint(luaL_checknumber(L, 8), luaL_checknumber(L, 9)), layer, (flipDirection)flipDir, c1->color, c2->color, c3->color, c4->color);
    return 1;
}
