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
	{0,0}
};

bludSpriteSheet::bludSpriteSheet(lua_State *L){
    texture = new LinearTexture();
    spriteRenderer = new ofxSpriteSheetRenderer(luaL_checknumber(L, 1), luaL_checknumber(L, 2), luaL_checknumber(L, 3), luaL_checknumber(L, 4));
    blendMode = 0;
    alpha = true;
}

int bludSpriteSheet::loadTexture(lua_State *L){
    // defaults to loading nearest neighbor style
    int height = luaL_checknumber(L, 2);
    if (lua_isnumber(L, 3)) {
        height = luaL_checknumber(L, 3);
    }
    texture->loadTextureFromPFS(luaL_checkstring(L, 1));
    spriteRenderer->loadTexture(texture);
    return 1;
}
int bludSpriteSheet::setupTexture(lua_State *L){
    spriteRenderer->loadTexture(texture);
    return 1;
}