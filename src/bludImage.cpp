#include "bludImage.h"

const char bludImage::className[] = "bludImage";

Lunar<bludImage>::RegType bludImage::methods[] = {
	method(bludImage, load),
	method(bludImage, draw),
	method(bludImage, drawScale),
	method(bludImage, width),
	method(bludImage, height),
	{0,0}
};