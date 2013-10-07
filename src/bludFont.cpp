#include "bludFont.h"
const char bludFont::className[] = "bludFont";
Lunar<bludFont>::RegType bludFont::methods[] = {
	method(bludFont, load),
	method(bludFont, draw),
	method(bludFont, getHeightWrap),
	method(bludFont, getWidth),
	method(bludFont, getHeight),
	{0,0}
};