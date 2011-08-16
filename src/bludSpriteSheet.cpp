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
	method(bludSprite, setSpriteX),
	method(bludSprite, setSpriteY),
	method(bludSprite, setTexWidth),
	method(bludSprite, setTexHeight),
	{0,0}
};

Lunar<bludSpriteSheet>::RegType bludSpriteSheet::methods[] = {
	method(bludSpriteSheet, loadTexture),
	method(bludSpriteSheet, clear),
	method(bludSpriteSheet, update),
	method(bludSpriteSheet, draw),
	method(bludSpriteSheet, addCenteredTile),
	method(bludSpriteSheet, addTile),
	method(bludSpriteSheet, addCenterRotatedTile),
	method(bludSpriteSheet, addCornerTile),
	{0,0}
};