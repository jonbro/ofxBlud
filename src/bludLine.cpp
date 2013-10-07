#include "bludLine.h"
#include "ofGLRenderer.h"
const char bludLine::className[] = "bludLine";

Lunar<bludLine>::RegType bludLine::methods[] = {
	method(bludLine, drawLine),
	{0,0}
};