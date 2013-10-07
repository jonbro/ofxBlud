#include "bludGraphics.h"

const char bludGraphics::className[] = "bludGraphics";
int bludGraphics::width = 800;
int bludGraphics::height = 600;
bool bludGraphics::needsFixRes = false;
Lunar<bludGraphics>::RegType bludGraphics::methods[] = {
	method(bludGraphics, push),
	method(bludGraphics, pop),
	method(bludGraphics, rotate),
	method(bludGraphics, translate),
	method(bludGraphics, scale),
	method(bludGraphics, setColor),
	method(bludGraphics, setBackground),
	method(bludGraphics, setFrameRate),	
	method(bludGraphics, drawRect),
	method(bludGraphics, drawCircle),
    method(bludGraphics, clear),
	method(bludGraphics, getWidth),
	method(bludGraphics, getHeight),
	method(bludGraphics, getMillis),
    method(bludGraphics, enableAlpha),
    method(bludGraphics, setBlendMode),
	method(bludGraphics, noise),
    method(bludGraphics, openURL),
    method(bludGraphics, eraseMode),
    method(bludGraphics, setWindowShape),
    method(bludGraphics, setFullscreen),
	{0,0}
};